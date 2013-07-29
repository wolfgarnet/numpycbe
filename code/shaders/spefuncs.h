#include "../common/common.h"
#include "../common/common_spu.h"

//#include <fmaf4.h>

#define _GET_NUM_BLOCKS 256

// Tags
#define _ATAG 0
#define _XTAG 2
#define _YTAG 4
#define _BTAG 6

// Must be a multiple of four and more than the number of SPEs
//#define _MAX_BLOCK_ARRAY 256
#define _MAX_BLOCK_ARRAY 8

//#define _DEBUG

/*
 * Debug
 */

#ifdef _DEBUG

#define _DBGP_UINT( i ) funcs->printuint( i )
#define _DBGP_FLOAT( f ) funcs->printfloat( f )
#define _DBGP_MATRIX( A, data, funcs ) _PrintMatrix( A, data, funcs )

#else

#define _DBGP_UINT( i )
#define _DBGP_FLOAT( f )
#define _DBGP_MATRIX( A, data, funcs )

#endif


/*
 * Communication functions
 */

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
}

// atidx and numidxs must be divisable by four!
void _GetBlocksArrayPart2( PyArrayObject *pyobj, char **mem, unsigned int atidx, unsigned int numidxs, unsigned int tag )
{
	//size = ( size + 15 ) &~ 15;
	spu_mfcdma32( mem, ((unsigned int)pyobj->blockData)+(atidx*4), ((numidxs + 3 ) &~ 3 ) * 4, tag, MFC_GET_CMD );
}

void _GetBlocksArrayPart( PyArrayObject *pyobj, char **mem, unsigned int row, unsigned int xpad, unsigned int tag )
{
	spu_mfcdma32( mem, ((unsigned int)pyobj->blockData)+((row*xpad)*4), xpad * 4, tag, MFC_GET_CMD );
}

void _GetBlockPointersPart( PyArrayObject *pyobj, char **mem, unsigned int idx, unsigned int xpad, unsigned int tag )
{
	spu_mfcdma32( mem, ((unsigned int)pyobj->blockData)+(idx*4), xpad * 4, tag, MFC_GET_CMD );
}

void _GetBlockPointers( PyArrayObject *pyobj, char **mem, unsigned int size, unsigned int tag )
{
	size = (size+3)&~3;
	//spu_mfcdma32( mem, (unsigned int)pyobj->blockData, size * 4, tag, MFC_GET_CMD );
	_GetAsync( (unsigned int)pyobj->blockData, 4 * size, mem, tag );
}


//void _GetScalar( PyArrayObject *pyobj, char **mem, float *var, unsigned int tag )
void _GetScalar( PyArrayObject *pyobj, char **mem, void *var, unsigned int tag )
{
	spu_mfcdma32( mem, (unsigned int)(pyobj->blockData), 4, tag, MFC_GET_CMD );
	spu_writech( MFC_WrTagMask, 1 << tag );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );

	spu_mfcdma32( var, (unsigned int)mem[0], 4, tag, MFC_GET_CMD );
	spu_writech( MFC_WrTagMask, 1 << tag );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void _PutScalar( PyArrayObject *pyobj, char **mem, void *var, unsigned int tag )
{
	spu_mfcdma32( mem, (unsigned int)(pyobj->blockData), 4, tag, MFC_GET_CMD );
	spu_writech( MFC_WrTagMask, 1 << tag );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );

	spu_mfcdma32( var, (unsigned int)mem[0], 4, tag, MFC_PUT_CMD );
	spu_writech( MFC_WrTagMask, 1 << tag );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void _GetBlock( char **blocks, char *block, unsigned int blockid, unsigned int numelements, unsigned int tag )
{
	_GetAsync( (unsigned int)blocks[blockid], 4 * numelements, block, tag );
}

void _GetRow( char **blocks, char *block, unsigned int blockid, unsigned int row, unsigned int numelements, unsigned int tag )
{
	_GetAsync( (unsigned int)blocks[blockid]+(row*numelements*4), 4 * numelements, block+(row*numelements*4), tag );
}

void _PutRow( char **blocks, char *block, unsigned int blockid, unsigned int row, unsigned int numelements, unsigned int tag )
{
	_PutAsync( (unsigned int)blocks[blockid]+(row*numelements*4), 4 * numelements, block+(row*numelements*4), tag );
}

void _PutRow2Row( char **blocks, char *block, unsigned int blockid, unsigned int row, unsigned int numelements, unsigned int tag )
{
	_PutAsync( (unsigned int)blocks[blockid]+(row*numelements*4), 4 * numelements, block, tag );
}

void _GetRow2Row( char **blocks, char *block, unsigned int blockid, unsigned int row, unsigned int numelements, unsigned int tag )
{
	_GetAsync( (unsigned int)blocks[blockid]+(row*numelements*4), 4 * numelements, block, tag );
}

//void _GetBlockRow( char **blocks, char *block, unsigned int blockid, unsigned int row, unsigned int numelements, unsigned int tag )
//{
//	//_GetAsync( (unsigned int)blocks[blockid], 4 * numelements, block, tag );
//	spu_mfcdma32( mem, (unsigned int)(pyobj->blockData), 4, tag, MFC_GET_CMD );
//}

/*
 * Calculation functions
 */

unsigned int _CalcNumFromIdTotIt( unsigned int SPE_id, unsigned int num_SPES, unsigned int totalIterations )
{
	unsigned int i = totalIterations / num_SPES;
	if( totalIterations % num_SPES != 0 )
	{
		if( SPE_id < totalIterations % num_SPES )
		{
			i++;
		}
	}

	return i;
}

unsigned int _CalcMaxBlockArraySize( unsigned int xdim )
{
	if( xdim < _MAX_BLOCK_ARRAY )
	{
		return ( xdim + 15 ) &~ 15;
	}
	else
	{
		return _MAX_BLOCK_ARRAY;
	}
}

//unsigned int _CalcMaxBlockArraySize( unsigned int xdim )
//{
//	if( xdim < _MAX_BLOCK_ARRAY )
//	{
//		return xdim;
//	}
//	else
//	{
//		return _MAX_BLOCK_ARRAY;
//	}
//}


/*
 * Loop unroling
 */

//#define _LoopUnrol()

/*
 * Miscellaneous
 */

unsigned int _FindPadding( PyArrayObject *pyobj )
{
	return pyobj->blockSize - pyobj->paddingx;
}

unsigned int _Pad16bytes( unsigned int value )
{
	return ( value + 15 ) &~ 15;
}

void _PrintMatrix( PyArrayObject *A, void *data, Functions_t *funcs )
{
	unsigned int i;
	for( i = 0 ; i < A->blockSize ; i++ )
	{
		funcs->printfloatrow( (float*)(data+A->blockSize*4*i), A->blockSize, i );
	}
}
