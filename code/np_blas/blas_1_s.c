
#include "../../../code/dispatcher/dispatcher/common.h"
#include "../../../code/dispatcher/dispatcher/common_spu.h"
#include "simdmath.h"

#define __ZEROS {0,0,0,0}
#define __ONES {1,1,1,1}
const vector float vZEROS = __ZEROS;
const vector float vONES  = __ONES;

unsigned int _GetIndex( unsigned int x, unsigned int y, unsigned int numcols )
{
     return ( x + y * numcols );
}

void _GetAsync( unsigned int ea, unsigned int size, void *buffer, unsigned int tag )
{
     while( size > __PROG_GET_MAX_SIZE )
     {
	  spu_mfcdma32( buffer, ea, __PROG_GET_MAX_SIZE, tag, MFC_GET_CMD );
	  buffer += __PROG_GET_MAX_SIZE;
	  ea     += __PROG_GET_MAX_SIZE;
	  size   -= __PROG_GET_MAX_SIZE;
     }
     
     if( size == 0 )
     {
	  return;
     }
     size += ( 16 - size % 16 );
     spu_mfcdma32( buffer, ea, size, tag, MFC_GET_CMD );
}

void _PutAsync( unsigned int ea, unsigned int size, void *buffer, unsigned int tag )
{
  while( size > __PROG_GET_MAX_SIZE )
  {
      spu_mfcdma32( buffer, ea, __PROG_GET_MAX_SIZE, tag, MFC_PUT_CMD );
      buffer += __PROG_GET_MAX_SIZE;
      ea     += __PROG_GET_MAX_SIZE;
      size   -= __PROG_GET_MAX_SIZE;
  }
  
  if( size == 0 )
  {
       return;
  }
  size += ( 16 - size % 16 );
  spu_mfcdma32( buffer, ea, size, tag, MFC_PUT_CMD );
}

void _Wait( unsigned int tag )
{
     spu_writech( MFC_WrTagMask, 1 << tag );
     spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void _GetPyArrayMetaData( PyArrayObject *ea, void *mem, unsigned int tag )
{
     spu_mfcdma32( mem, (unsigned int)ea, sizeof( PyArrayObject ), tag, MFC_GET_CMD );
     
     // Do we need to wait???
     spu_writech( MFC_WrTagMask, 1 << tag );
     spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void _GetBlocksArray( PyArrayObject *pyobj, char **mem )
{
     spu_mfcdma32( mem, (unsigned int)pyobj->blockData, 4 * pyobj->numberOfBlocks, 1, MFC_GET_CMD );
     
     // Do we need to wait???
     spu_writech( MFC_WrTagMask, 1 << 1 );
     spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void _GetBlock( char **blocks, char *block, unsigned int blockid, unsigned int numelements, unsigned int tag )
{
     _GetAsync( (unsigned int)blocks[blockid], 4 * numelements, block, tag );
//     spu_mfcdma32( block, (unsigned int)blocks[blockid], 4 * numelements, tag, MFC_GET_CMD );

     // Do we need to wait???
//     spu_writech( MFC_WrTagMask, 1 << tag );
//     spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void _SaveScalar( PyArrayObject *pyobj, void *value )
{
//  spu_mfcdma32( buffer, ea, size, tag, MFC_PUT_CMD );
}

float sdot( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
     // Make room for python meta data
     PyArrayObject *o1 = (PyArrayObject *)info->LS_shaderMemory;
     PyArrayObject *o2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
//     PyArrayObject *s1 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject ) * 2;

     // Get meta data
     _GetPyArrayMetaData( myop->obj[0], o1, 0 );
     _GetPyArrayMetaData( myop->obj[1], o2, 0 );
//     _GetPyArrayMetaData( myop->scalar[0], s1, 0 );

     // Make room for data block arrays
     char **blocks1    = (char**)(o2 + sizeof( PyArrayObject ));
     char **blocks2    = (char**)(blocks1 + 4 * o1->numberOfBlocks);
//     char **sblock     = (char**)(blocks2 + 4 * o1->numberOfBlocks);

     // Get block arrays
     _GetBlocksArray( o1, blocks1 );
     _GetBlocksArray( o2, blocks2 );
//     _GetBlocksArray( s1, sblock );

     unsigned int idx = 0, nextidx;

     // Get blocks
     _GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, o1->blockSizes, idx );
     _GetBlock( blocks2, info->LS_blockDataArea[1], SPE_id, o2->blockSizes, idx );

     float res = 0;

     unsigned int i, j;

     for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
     {
	  nextidx = idx ^ 1;
	  _GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, o1->blockSizes, nextidx );
	  _GetBlock( blocks2, info->LS_blockDataArea[1+nextidx*3], i, o2->blockSizes, nextidx );

	  _Wait( idx );
	  for( j = 0 ; j < o1->blockSizes ; j++ )
	  {
	       res += ((float*)info->LS_blockDataArea[0+idx*3])[j] + ((float*)info->LS_blockDataArea[1+idx*3])[j];
	  }
	  idx = nextidx;
     }
     _Wait( idx );

     for( j = 0 ; j < o1->blockSizes ; j++ )
     {
	  res += ((float*)info->LS_blockDataArea[0+idx*3])[j] + ((float*)info->LS_blockDataArea[1+idx*3])[j];
//	  res += *(float*)info->LS_blockDataArea[0+idx*3] + *(float*)info->LS_blockDataArea[1+idx*3];
     }

     spu_mfcdma32( &res, EA_result, 4, 0, MFC_PUT_CMD );
     _Wait( 0 );

     funcs->printfloat( res );
     
     return 1.0f;
}


float snrm2( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
     // Make room for python meta data
     PyArrayObject *o1 = (PyArrayObject *)info->LS_shaderMemory;

     // Get meta data
     _GetPyArrayMetaData( myop->obj[0], o1, 0 );

     // Make room for data block arrays
     char **blocks1    = (char**)(o1 + sizeof( PyArrayObject ));

     // Get block arrays
     _GetBlocksArray( o1, blocks1 );

     unsigned int idx = 0, nextidx;

     // Get blocks
     _GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, o1->blockSizes, idx );

     float res = 0;

     unsigned int i, j;

     for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
     {
	  nextidx = idx ^ 1;
	  _GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, o1->blockSizes, nextidx );

	  _Wait( idx );
	  for( j = 0 ; j < o1->blockSizes ; j++ )
	  {
	       res += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[0+idx*3])[j];
//	       res += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[0+idx*3])[j];
	  }
	  idx = nextidx;
     }
     _Wait( idx );

     for( j = 0 ; j < o1->blockSizes ; j++ )
     {
	  res += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[0+idx*3])[j];
//	  res += *(float*)info->LS_blockDataArea[0+idx*3] * *(float*)info->LS_blockDataArea[0+idx*3];
     }

     spu_mfcdma32( &res, EA_result, 4, 0, MFC_PUT_CMD );
     _Wait( 0 );

     funcs->printfloat( res );
     
     return 1.0f;
}

void sdotv( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
     // Make room for python meta data
     PyArrayObject *o1 = (PyArrayObject *)info->LS_shaderMemory;
     PyArrayObject *o2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
//     PyArrayObject *s1 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject ) * 2;

     // Get meta data
     _GetPyArrayMetaData( myop->obj[0], o1, 0 );
     _GetPyArrayMetaData( myop->obj[1], o2, 0 );
//     _GetPyArrayMetaData( myop->scalar[0], s1, 0 );

     // Make room for data block arrays
     char **blocks1    = (char**)(o2 + sizeof( PyArrayObject ));
     char **blocks2    = (char**)(blocks1 + 4 * o1->numberOfBlocks);
//     char **sblock     = (char**)(blocks2 + 4 * o1->numberOfBlocks);

     // Get block arrays
     _GetBlocksArray( o1, blocks1 );
     _GetBlocksArray( o2, blocks2 );
//     _GetBlocksArray( s1, sblock );

     unsigned int idx = 0, nextidx;

     // Get blocks
     _GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, o1->blockSizes, idx );
     _GetBlock( blocks2, info->LS_blockDataArea[1], SPE_id, o2->blockSizes, idx );
     vector float *row1, *row2;
     vector float res = {0,0,0,0};

     float r = 0;

     unsigned int i, j;

     for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
     {
	  nextidx = idx ^ 1;
	  _GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, o1->blockSizes, nextidx );
	  _GetBlock( blocks2, info->LS_blockDataArea[1+nextidx*3], i, o2->blockSizes, nextidx );

	  _Wait( idx );
	  row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
	  row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
	  for( j = 0 ; j < o1->blockSizes ; j++ )
	  {
//	       res += ((float*)info->LS_blockDataArea[0+idx*3])[j] + ((float*)info->LS_blockDataArea[1+idx*3])[j];
	       res += spu_mul(row1[j], row1[j]);
	  }
	  idx = nextidx;
     }
     _Wait( idx );

     for( j = 0 ; j < o1->blockSizes ; j++ )
     {
//	  res += ((float*)info->LS_blockDataArea[0+idx*3])[j] + ((float*)info->LS_blockDataArea[1+idx*3])[j];
//	  res += *(float*)info->LS_blockDataArea[0+idx*3] + *(float*)info->LS_blockDataArea[1+idx*3];
	  res += spu_mul(row1[j], row1[j]);
     }

     r  = spu_extract(res,0);
     r += spu_extract(res,1);
     r += spu_extract(res,2);
     r += spu_extract(res,3);

     spu_mfcdma32( &r, EA_result, 4, 0, MFC_PUT_CMD );
     _Wait( 0 );

//     funcs->printfloat( r );
     
}

float snrm2v( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
     // Make room for python meta data
     PyArrayObject *o1 = (PyArrayObject *)info->LS_shaderMemory;

     // Get meta data
     _GetPyArrayMetaData( myop->obj[0], o1, 0 );

     // Make room for data block arrays
     char **blocks1    = (char**)(o1 + sizeof( PyArrayObject ));

     // Get block arrays
     _GetBlocksArray( o1, blocks1 );

     unsigned int idx = 0, nextidx;

     // Get blocks
     _GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, o1->blockSizes, idx );
     vector float *row1;
     vector float res = {0,0,0,0};

     float r = 0;

     unsigned int i, j;

     for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
     {
	  nextidx = idx ^ 1;
	  _GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, o1->blockSizes, nextidx );

	  _Wait( idx );
	  row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
	  for( j = 0 ; j < o1->blockSizes / 4 ; j++ )
	  {
	       //    res += *(float*)info->LS_blockDataArea[0+idx*3] * *(float*)info->LS_blockDataArea[0+idx*3];
	       res += spu_mul(row1[j], row1[j]);
	  }
	  idx = nextidx;
     }
     _Wait( idx );

     for( j = 0 ; j < o1->blockSizes / 4 ; j++ )
     {
//	  res += *(float*)info->LS_blockDataArea[0+idx*3] * *(float*)info->LS_blockDataArea[0+idx*3];
	  res += spu_mul(row1[j], row1[j]);
     }

     r  = spu_extract(res,0);
     r += spu_extract(res,1);
     r += spu_extract(res,2);
     r += spu_extract(res,3);

     funcs->printfloat( r );

     spu_mfcdma32( &r, EA_result, 4, 0, MFC_PUT_CMD );
     _Wait( 0 );

     
     return 1.0f;
}
