
#include "../../../code/dispatcher/dispatcher/common.h"
#include "../../../code/dispatcher/dispatcher/common_spu.h"
#include <simdmath.h>
#include <fabsf4.h>

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
	size = ( size + 15 ) &~ 15;
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
	size = ( size + 15 ) &~ 15;
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
	//unsigned int size = 4 * pyobj->numberOfBlocks + ( 16 - ( 4 * pyobj->numberOfBlocks % 16 ) );
	unsigned int size = ( ( 4 * pyobj->numberOfBlocks + 15 ) &~ 15 );
	spu_mfcdma32( mem, (unsigned int)pyobj->blockData, size, 1, MFC_GET_CMD );

	// Do we need to wait???
	spu_writech( MFC_WrTagMask, 1 << 1 );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

// atidx and numidxs must be divisable by four!
void _GetBlocksArrayPart( PyArrayObject *pyobj, char **mem, unsigned int atidx, unsigned int numidxs, unsigned int tag )
{
	spu_mfcdma32( mem, (unsigned int)(pyobj->blockData)+(atidx*4), (numidxs*4), tag, MFC_GET_CMD );

	// Do we need to wait???
	spu_writech( MFC_WrTagMask, 1 << tag );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

// + 15 ) & ~15
void _GetBlocksArray2( PyArrayObject *pyobj, char **mem, void (*addr)(unsigned int), void (*uint)(unsigned int) )
{
	unsigned int size = ( ( 4 * pyobj->numberOfBlocks + 15 ) &~ 15 );
	addr(mem);
	addr((unsigned int)pyobj->blockData);
	uint(size);
	spu_mfcdma32( mem, (unsigned int)pyobj->blockData, size, 1, MFC_GET_CMD );

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

unsigned int _FindPadding( PyArrayObject *pyobj )
{
	return pyobj->blockSize * pyobj->blockSize - pyobj->paddingx;
}

#define _GET_NUM_BLOCKS 256
void sdotv4( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *o1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *o2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], o1, 0 );
	_GetPyArrayMetaData( myop->obj[1], o2, 0 );


	// Make room for data block arrays
	char **blocks1[2];
	char **blocks2[2];
	blocks1[0] = (char**)(o2 + sizeof( PyArrayObject ));
	blocks1[1] = (char**)(blocks1[0] + 4 * _GET_NUM_BLOCKS );
	blocks2[0] = (char**)(blocks1[1] + 4 * _GET_NUM_BLOCKS );
	blocks2[1] = (char**)(blocks2[0] + 4 * _GET_NUM_BLOCKS );

	unsigned int idx = 0, nextidx;
	unsigned int blc = 0, nextblc = 1, curblc = _GET_NUM_BLOCKS;
	unsigned int i,j;
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	_GetBlocksArrayPart( o1, blocks1[0], 0, _GET_NUM_BLOCKS, 0 );
	_GetBlocksArrayPart( o1, blocks1[1], _GET_NUM_BLOCKS, _GET_NUM_BLOCKS, 3 );

	_GetBlocksArrayPart( o2, blocks2[0], 0, _GET_NUM_BLOCKS, 0 );
	_GetBlocksArrayPart( o2, blocks2[1], _GET_NUM_BLOCKS, _GET_NUM_BLOCKS, 3 );

	_Wait( 0 );

	// Get blocks
	_GetBlock( blocks1[0], info->LS_blockDataArea[0], SPE_id, totalelements, idx );
	_GetBlock( blocks2[0], info->LS_blockDataArea[1], SPE_id, totalelements, idx );
	vector float *row1, *row2;
	vector float res = {0,0,0,0};

	_Wait(0);

	float r = 0;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		if( i > curblc )
		{
			curblc = ( i / _GET_NUM_BLOCKS ) * _GET_NUM_BLOCKS;
			nextblc = blc ^ 1; // n = 1, b = 0

			_Wait( nextblc + 2 );
			_GetBlocksArrayPart( o1, blocks1[blc], curblc, _GET_NUM_BLOCKS, blc + 2 );
			_GetBlocksArrayPart( o2, blocks2[blc], curblc, _GET_NUM_BLOCKS, blc + 2 );

			blc = nextblc;
		}


		nextidx = idx ^ 1;
		_GetBlock( blocks1[blc], info->LS_blockDataArea[0+nextidx*3], i%_GET_NUM_BLOCKS, totalelements, nextidx );
		_GetBlock( blocks2[blc], info->LS_blockDataArea[1+nextidx*3], i%_GET_NUM_BLOCKS, totalelements, nextidx );

		_Wait( idx );
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			res += spu_mul(row1[j], row1[j]);
		}

//		for( j = 0 ; j < totalelements / 16 ; j=j+4 )
//		{
//			res += spu_mul(row1[j], row1[j]);
//			res += spu_mul(row1[j+1], row1[j+1]);
//			res += spu_mul(row1[j+2], row1[j+2]);
//			res += spu_mul(row1[j+3], row1[j+3]);
//		}
		idx = nextidx;
	}
	_Wait( idx );

	// This is the last, block! Does it have padding?
	if( i == o1->numberOfBlocksXDim-1 )
	{
		unsigned int l = _FindPadding( o1 );
		// If the last number of elements is divisable with four!?
		if( l % 4 == 0 )
		{

			row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
			row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
			for( j = 0 ; j < l / 4 ; j++ )
			{
				res += spu_mul(row1[j], row2[j]);
			}

		}
		else
		{
			// Can this be optimized???
			for( j = 0 ; j < l ; j++ )
			{
				res[0] += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[1+idx*3])[j];
			}
		}
	}
	else
	{
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			res += spu_mul(row1[j], row1[j]);
		}
	}

	r  = spu_extract(res,0);
	r += spu_extract(res,1);
	r += spu_extract(res,2);
	r += spu_extract(res,3);

	spu_mfcdma32( &r, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );
}



void sdotv2( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *o1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *o2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], o1, 0 );
	_GetPyArrayMetaData( myop->obj[1], o2, 0 );

	// Make room for data block arrays
	char **blocks1[2];
	char **blocks2[2];
	blocks1[0] = (char**)(o2 + sizeof( PyArrayObject ));
	blocks1[1] = (char**)(blocks1[0] + 4 * _GET_NUM_BLOCKS );
	blocks2[0] = (char**)(blocks1[1] + 4 * _GET_NUM_BLOCKS );
	blocks2[1] = (char**)(blocks2[0] + 4 * _GET_NUM_BLOCKS );

	unsigned int idx = 0, nextidx;
	unsigned int blc = 0, nextblc = 1, curblc = _GET_NUM_BLOCKS;
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	// Get block arrays _GetBlocksArrayPart( PyArrayObject *pyobj, char **mem, unsigned int atidx, unsigned int numidxs, unsigned int tag )
	funcs->printuint(1000);
	//_GetBlocksArray2( o1, blocks1, funcs->printaddr, funcs->printuint );
	_GetBlocksArrayPart( o1, blocks1[0], 0, _GET_NUM_BLOCKS, 0 );
	_GetBlocksArrayPart( o1, blocks1[1], _GET_NUM_BLOCKS, _GET_NUM_BLOCKS, 3 );
	//_GetBlocksArray( o1, blocks1 );
	funcs->printuint(500);
	//_GetBlocksArray2( o2, blocks2, funcs->printaddr, funcs->printuint );
	_GetBlocksArrayPart( o2, blocks2[0], 0, _GET_NUM_BLOCKS, 0 );
	_GetBlocksArrayPart( o1, blocks2[1], _GET_NUM_BLOCKS, _GET_NUM_BLOCKS, 3 );
	//_GetBlocksArray( o2, blocks2 );
	funcs->printuint(999);

	_Wait( 0 );

	// Get blocks
	_GetBlock( blocks1[0], info->LS_blockDataArea[0], SPE_id, totalelements, idx );
	_GetBlock( blocks2[0], info->LS_blockDataArea[1], SPE_id, totalelements, idx );
	funcs->printuint(998);
	vector float *row1, *row2;
	vector float res = {0,0,0,0};

	float r = 0;

	unsigned int i, j;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		//funcs->printuint(i);

		if( i > curblc )
		{
			curblc = ( i / _GET_NUM_BLOCKS ) * _GET_NUM_BLOCKS;
			nextblc = blc ^ 1; // n = 1, b = 0

			_Wait( nextblc + 2 );
			_GetBlocksArrayPart( o1, blocks1[blc], curblc, _GET_NUM_BLOCKS, blc + 2 );
			_GetBlocksArrayPart( o2, blocks2[blc], curblc, _GET_NUM_BLOCKS, blc + 2 );

			blc = nextblc;
		}


		nextidx = idx ^ 1;
		_GetBlock( blocks1[blc], info->LS_blockDataArea[0+nextidx*3], i%_GET_NUM_BLOCKS, totalelements, nextidx );
		_GetBlock( blocks2[blc], info->LS_blockDataArea[1+nextidx*3], i%_GET_NUM_BLOCKS, totalelements, nextidx );

		_Wait( idx );
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			res += spu_mul(row1[j], row1[j]);
		}

//		for( j = 0 ; j < totalelements / 16 ; j=j+4 )
//		{
//			res += spu_mul(row1[j], row1[j]);
//			res += spu_mul(row1[j+1], row1[j+1]);
//			res += spu_mul(row1[j+2], row1[j+2]);
//			res += spu_mul(row1[j+3], row1[j+3]);
//		}
		idx = nextidx;
	}
	_Wait( idx );

	// This is the last, block! Does it have padding?
	if( i == o1->numberOfBlocksXDim-1 )
	{
		unsigned int l = _FindPadding( o1 );
		// If the last number of elements is divisable with four!?
		if( l % 4 == 0 )
		{
			row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
			row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
			for( j = 0 ; j < l / 4 ; j++ )
			{
				res += spu_mul(row1[j], row1[j]);
			}
		}
		else
		{
			// Can this be optimized???
			for( j = 0 ; j < l ; j++ )
			{
				res[0] += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[1+idx*3])[j];
			}
		}
	}
	else
	{
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			res += spu_mul(row1[j], row1[j]);
		}
	}



	r  = spu_extract(res,0);
	r += spu_extract(res,1);
	r += spu_extract(res,2);
	r += spu_extract(res,3);

	spu_mfcdma32( &r, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );
}


void sdotv( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *o1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *o2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], o1, 0 );
	_GetPyArrayMetaData( myop->obj[1], o2, 0 );

	// Make room for data block arrays
	char **blocks1 = (char**)(o2 + sizeof( PyArrayObject ));
	char **blocks2 = (char**)(blocks1 + 4 * _GET_NUM_BLOCKS );

	unsigned int idx = 0, nextidx;
	unsigned int blc = 0, nextblc = 1, curblc = _GET_NUM_BLOCKS;
	unsigned int i,j;
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	_GetBlocksArrayPart( o1, blocks1, 0, _GET_NUM_BLOCKS, 0 );
	_GetBlocksArrayPart( o2, blocks2, 0, _GET_NUM_BLOCKS, 0 );

	_Wait( 0 );

	// Get blocks
	_GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, totalelements, idx );
	_GetBlock( blocks2, info->LS_blockDataArea[1], SPE_id, totalelements, idx );
	vector float *row1, *row2;
	vector float res = {0,0,0,0};

	_Wait(0);

	float r = 0;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		if( i > curblc )
		{
			_GetBlocksArrayPart( o1, blocks1, curblc, _GET_NUM_BLOCKS, 2 );
			_GetBlocksArrayPart( o2, blocks2, curblc, _GET_NUM_BLOCKS, 2 );
			_Wait(2);
			curblc += _GET_NUM_BLOCKS;
		}


		nextidx = idx ^ 1;
		_GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i%_GET_NUM_BLOCKS, totalelements, nextidx );
		_GetBlock( blocks2, info->LS_blockDataArea[1+nextidx*3], i%_GET_NUM_BLOCKS, totalelements, nextidx );

		_Wait( idx );
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			res += spu_mul(row1[j], row2[j]);
		}

//		for( j = 0 ; j < totalelements / 4 ; j=j+16 )
//		{
//			res += spu_mul(row1[j],   row2[j]);
//			res += spu_mul(row1[j+1], row2[j+1]);
//			res += spu_mul(row1[j+2], row2[j+2]);
//			res += spu_mul(row1[j+3], row2[j+3]);
//			res += spu_mul(row1[j+4], row2[j+4]);
//			res += spu_mul(row1[j+5], row2[j+5]);
//			res += spu_mul(row1[j+6], row2[j+6]);
//			res += spu_mul(row1[j+7], row2[j+7]);
//			res += spu_mul(row1[j+8], row2[j+8]);
//			res += spu_mul(row1[j+9], row2[j+9]);
//			res += spu_mul(row1[j+10], row2[j+10]);
//			res += spu_mul(row1[j+11], row2[j+11]);
//			res += spu_mul(row1[j+12], row2[j+12]);
//			res += spu_mul(row1[j+13], row2[j+13]);
//			res += spu_mul(row1[j+14], row2[j+14]);
//			res += spu_mul(row1[j+15], row2[j+15]);
//		}
		idx = nextidx;
	}
	_Wait( idx );

	// This is the last, block! Does it have padding?
	if( i == o1->numberOfBlocksXDim-1 )
	{
		unsigned int l = _FindPadding( o1 );
		// If the last number of elements is divisable with four!?
		if( l % 4 == 0 )
		{

			row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
			row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
			for( j = 0 ; j < l / 4 ; j++ )
			{
				res += spu_mul(row1[j], row2[j]);
			}

		}
		else
		{
			// Can this be optimized???
			for( j = 0 ; j < l ; j++ )
			{
				res[0] += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[1+idx*3])[j];
			}
		}
	}
	else
	{
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		row2 = (vector float*)info->LS_blockDataArea[1+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			res += spu_mul(row1[j], row1[j]);
		}
	}

	r  = spu_extract(res,0);
	r += spu_extract(res,1);
	r += spu_extract(res,2);
	r += spu_extract(res,3);

	spu_mfcdma32( &r, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );
}



void sdot( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
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
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	// Get blocks
	_GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, totalelements, idx );
	_GetBlock( blocks2, info->LS_blockDataArea[1], SPE_id, totalelements, idx );

	float res = 0;

	unsigned int i, j;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		nextidx = idx ^ 1;
		_GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, totalelements, nextidx );
		_GetBlock( blocks2, info->LS_blockDataArea[1+nextidx*3], i, totalelements, nextidx );

		_Wait( idx );
		for( j = 0 ; j < totalelements ; j++ )
		{
			res += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[1+idx*3])[j];
		}
		idx = nextidx;
	}
	_Wait( idx );

	// This is the last, block! Does it have padding?
	if( i == o1->numberOfBlocksXDim-1 )
	{
		unsigned int l = _FindPadding( o1 );
		for( j = 0 ; j < l ; j++ )
		{
			res += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[1+idx*3])[j];
		}
	}
	else
	{
		for( j = 0 ; j < totalelements ; j++ )
		{
			res += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[1+idx*3])[j];
		}
	}



	spu_mfcdma32( &res, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );
}


void snrm2( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
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
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	// Get blocks
	_GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, totalelements, idx );

	float res = 0;

	unsigned int i, j;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		nextidx = idx ^ 1;
		_GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, totalelements, nextidx );

		_Wait( idx );
		for( j = 0 ; j < totalelements ; j++ )
		{
			res += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[0+idx*3])[j];
		}
		idx = nextidx;
	}
	_Wait( idx );

	for( j = 0 ; j < totalelements ; j++ )
	{
		res += ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[0+idx*3])[j];
	}

	spu_mfcdma32( &res, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );

	funcs->printfloat( res );
}

void sscal( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *o1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *s1 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], o1, 0 );
	_GetPyArrayMetaData( myop->scalar[0], s1, 0 );

	// Make room for data block arrays
	char **blocks1    = (char**)(s1 + sizeof( PyArrayObject ));
	char **blocks2    = (char**)(blocks1 + 4 * o1->numberOfBlocks);

	// Get block arrays
	_GetBlocksArray( o1, blocks1 );
	_GetBlocksArray( s1, blocks2 );

	unsigned int idx = 0, nextidx;
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	// Get blocks
	_GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, totalelements, idx );
	_GetBlock( blocks2, info->LS_blockDataArea[1], SPE_id, totalelements, 2 );

	// Wait for the scalar
	_Wait(2);

	float value = ((float*)info->LS_blockDataArea[1])[0];

	unsigned int i, j;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		nextidx = idx ^ 1;
		_GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, totalelements, nextidx );

		_Wait( idx );
		for( j = 0 ; j < totalelements ; j++ )
		{
			((float*)info->LS_blockDataArea[0+idx*3])[j] *= value;

		}

		_PutAsync( (unsigned int)blocks1[0+idx*3], totalelements * 4, info->LS_blockDataArea[0+idx*3], idx );

		idx = nextidx;
	}
	_Wait( idx );

	for( j = 0 ; j < totalelements ; j++ )
	{
		((float*)info->LS_blockDataArea[0+idx*3])[j] *= value;
	}

	_PutAsync( (unsigned int)blocks1[0+idx*3], totalelements * 4, info->LS_blockDataArea[0+idx*3], idx );
	_Wait( idx );
}

void sasum( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
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
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	// Get blocks
	_GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, totalelements, idx );

	float res = 0;

	unsigned int i, j;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		nextidx = idx ^ 1;
		_GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, totalelements, nextidx );

		_Wait( idx );
		for( j = 0 ; j < totalelements ; j++ )
		{
			res += ( ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[0+idx*3])[j] ) / ((float*)info->LS_blockDataArea[0+idx*3])[j];
		}
		idx = nextidx;
	}
	_Wait( idx );

	// This is the last, block! Does it have padding?
	if( i == o1->numberOfBlocksXDim-1 )
	{
		unsigned int l = _FindPadding( o1 );
		for( j = 0 ; j < l ; j++ )
		{
			res += ( ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[0+idx*3])[j] ) / ((float*)info->LS_blockDataArea[0+idx*3])[j];
		}
	}
	else
	{
		for( j = 0 ; j < totalelements ; j++ )
		{
			res += ( ((float*)info->LS_blockDataArea[0+idx*3])[j] * ((float*)info->LS_blockDataArea[0+idx*3])[j] ) / ((float*)info->LS_blockDataArea[0+idx*3])[j];
		}
	}

	spu_mfcdma32( &res, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );
}



void snrm2v( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
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
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	// Get blocks
	_GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, totalelements, idx );
	vector float *row1;
	vector float res = {0,0,0,0};

	float r = 0;

	unsigned int i, j;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		nextidx = idx ^ 1;
		_GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, totalelements, nextidx );

		_Wait( idx );
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			//    res += *(float*)info->LS_blockDataArea[0+idx*3] * *(float*)info->LS_blockDataArea[0+idx*3];
			res += spu_mul(row1[j], row1[j]);
		}
		idx = nextidx;
	}
	_Wait( idx );

	row1 = (vector float*)info->LS_blockDataArea[0+idx*3];

	for( j = 0 ; j < totalelements / 4 ; j++ )
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
}

void sasumv( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
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
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	// Get blocks
	_GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, totalelements, idx );
	vector float *row1;
	vector float res = {0,0,0,0};

	float r = 0;

	unsigned int i, j;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		nextidx = idx ^ 1;
		_GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, totalelements, nextidx );

		_Wait( idx );

		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];

		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			res += _fabsf4( row1[j] );
		}
		idx = nextidx;
	}
	_Wait( idx );

	// This is the last, block! Does it have padding?
	if( i == o1->numberOfBlocksXDim-1 )
	{
		unsigned int l = _FindPadding( o1 );
		// If the last number of elements is divisable with four!?
		if( l % 4 == 0 )
		{
			row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
			for( j = 0 ; j < l / 4 ; j++ )
			{
				res += _fabsf4( row1[j] );
			}
		}
		else
		{
			// Can this be optimized???
			// Incorrect!!! REMEMBER
			for( j = 0 ; j < l ; j++ )
			{
				res += _fabsf4( row1[j] );
			}
		}
	}
	else
	{
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			res += _fabsf4( row1[j] );
		}
	}

	r  = spu_extract(res,0);
	r += spu_extract(res,1);
	r += spu_extract(res,2);
	r += spu_extract(res,3);

	spu_mfcdma32( &r, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );
}

/*
 *  ISAMAX
 */

void _isamax_h( float value, unsigned int vi, unsigned int loop, float *max, unsigned int *index )
{
	if( value > *max )
	{
		*max   = value;
		*index = loop * 4 + vi;
	}
}

void isamaxv( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
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
	unsigned int totalelements = o1->blockSize * o1->blockSize;

	// Get blocks
	_GetBlock( blocks1, info->LS_blockDataArea[0], SPE_id, totalelements, idx );
	vector float *row1;
	vector float tmp = {0,0,0,0};
	float max = 0.0f;
	unsigned int index = 0;

	unsigned int i, j;

	for( i = SPE_id ; i < o1->numberOfBlocks-myop->num_SPES ; i = i + myop->num_SPES )
	{
		nextidx = idx ^ 1;
		_GetBlock( blocks1, info->LS_blockDataArea[0+nextidx*3], i, totalelements, nextidx );

		_Wait( idx );

		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];

		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			tmp = _fabsf4( row1[j] );
			_isamax_h( spu_extract(tmp,0), 0, j, &max, &index );
			_isamax_h( spu_extract(tmp,1), 1, j, &max, &index );
			_isamax_h( spu_extract(tmp,2), 2, j, &max, &index );
			_isamax_h( spu_extract(tmp,3), 3, j, &max, &index );
		}
		idx = nextidx;
	}
	_Wait( idx );

	// This is the last, block! Does it have padding?
	if( i == o1->numberOfBlocksXDim-1 )
	{
		unsigned int l = _FindPadding( o1 );
		// If the last number of elements is divisable with four!?
		if( l % 4 == 0 )
		{
			row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
			for( j = 0 ; j < l / 4 ; j++ )
			{
				tmp = _fabsf4( row1[j] );
				_isamax_h( spu_extract(tmp,0), 0, j, &max, &index );
				_isamax_h( spu_extract(tmp,1), 1, j, &max, &index );
				_isamax_h( spu_extract(tmp,2), 2, j, &max, &index );
				_isamax_h( spu_extract(tmp,3), 3, j, &max, &index );
			}
		}
		else
		{
			// Can this be optimized???
			// Incorrect!!! REMEMBER
			for( j = 0 ; j < l ; j++ )
			{
				tmp = _fabsf4( row1[j] );
				_isamax_h( spu_extract(tmp,0), 0, j, &max, &index );
				_isamax_h( spu_extract(tmp,1), 1, j, &max, &index );
				_isamax_h( spu_extract(tmp,2), 2, j, &max, &index );
				_isamax_h( spu_extract(tmp,3), 3, j, &max, &index );
			}
		}
	}
	else
	{
		row1 = (vector float*)info->LS_blockDataArea[0+idx*3];
		for( j = 0 ; j < totalelements / 4 ; j++ )
		{
			tmp = _fabsf4( row1[j] );
			_isamax_h( spu_extract(tmp,0), 0, j, &max, &index );
			_isamax_h( spu_extract(tmp,1), 1, j, &max, &index );
			_isamax_h( spu_extract(tmp,2), 2, j, &max, &index );
			_isamax_h( spu_extract(tmp,3), 3, j, &max, &index );
		}
	}

	spu_mfcdma32( &index, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );
}
