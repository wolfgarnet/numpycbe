#include "spefuncs.h"

/*
 * Array multiply for vectors, ie one dimensional arrays
 *
 * Notes:
 * 1) The minimum number of block pointers transfered from blockData, must be at least the number of SPEs
 * 2) A bus error occurs if the number of SPES is higher than the number of total blocks
 */

void arraymultiply1d( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *x1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *x2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *r  = (PyArrayObject *)x2+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], x1, 0 );
	_GetPyArrayMetaData( myop->obj[1], x2, 0 );
	_GetPyArrayMetaData( myop->obj[2],  r, 0 );

	_Wait(0); // WAIT!!!

	unsigned int xpad = _CalcMaxBlockArraySize( x1->numberOfBlocksXDim );

	// Make room for data block arrays
	char **blocks_x1 = (char**)( ( (unsigned int)( r + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_x2 = (char**)( blocks_x1 + 4 * xpad );
	char **blocks_r  = (char**)( blocks_x2 + 4 * xpad );

	// Fetch pointers to first __ datablocks for vectors
	_GetBlockPointersPart( x1, blocks_x1, 0, xpad, 0 );
	_GetBlockPointersPart( x2, blocks_x2, 0, xpad, 0 );
	_GetBlockPointersPart(  r,  blocks_r, 0, xpad, 0 );

	// Must wait for the block array to finish
	_Wait( 0 );

	// Begin double buffering
	_GetBlock( blocks_x1, info->LS_blockDataArea[0], SPE_id, x1->blockSize*x1->blockSize, 0 );
	_GetBlock( blocks_x2, info->LS_blockDataArea[1], SPE_id, x2->blockSize*x2->blockSize, 0 );
	// No need for the result block

	unsigned int i, j;
	unsigned int idx = 0, nextidx;
	unsigned int numelements = x1->blockSize * x1->blockSize;
	unsigned int blc = xpad;

	vector float *row1, *row2, *res;

	// Foreach block column for the specific block row
	for( i = SPE_id + myop->num_SPES ; i < x1->numberOfBlocksXDim ; i += myop->num_SPES )
	{
		nextidx = idx ^ 1;

		// Fetch next blocks
		_GetBlock( blocks_x1, info->LS_blockDataArea[nextidx*3], i%xpad, numelements, nextidx );
		_GetBlock( blocks_x2, info->LS_blockDataArea[nextidx*3+1], i%xpad, numelements, nextidx );

		_Wait( idx );

		// Elementwise product/arraymultiply
		row1 = (vector float*)( info->LS_blockDataArea[idx*3] );
		row2 = (vector float*)( info->LS_blockDataArea[idx*3+1] );
		res  = (vector float*)( info->LS_blockDataArea[idx*3+2] );
		for( j = 0 ; j < numelements / 4 ; j++ )
		{
			res[j] = spu_mul( row1[j], row2[j] );
		}

		// Send back the result
		_PutAsync( (unsigned int)blocks_r[(i-myop->num_SPES)%xpad], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

		// Get next block array part
		if( i > blc-1 )
		{
			_GetBlockPointersPart( x1, blocks_x1, blc, xpad, 0 );
			_GetBlockPointersPart( x2, blocks_x2, blc, xpad, 0 );
			_GetBlockPointersPart(  r,  blocks_r, blc, xpad, 0 );
			blc += xpad;
			_Wait( 0 );
		}

		idx = nextidx;
	}

	_Wait( 0 );

	// Finish the last block

	// Elementwise product/arraymultiply
	row1 = (vector float*)( info->LS_blockDataArea[idx*3] );
	row2 = (vector float*)( info->LS_blockDataArea[idx*3+1] );
	res  = (vector float*)( info->LS_blockDataArea[idx*3+2] );
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = spu_mul( row1[j], row2[j] );
	}

	// Send back the result
	_PutAsync( (unsigned int)blocks_r[(i-myop->num_SPES)%xpad], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

	_Wait( idx );
}

/*
 * Array multiply for two dimensional arrays, ie used for two dimensional arrays or higher
 *
 * Notes:
 * 1) The minimum number of block pointers transfered from blockData, must be at least the number of SPEs
 * 2) A bus error occurs if the number of SPES is higher than the number of total blocks
 */

void arraymultiply2d( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *x1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *x2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *r  = (PyArrayObject *)x2+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], x1, 0 );
	_GetPyArrayMetaData( myop->obj[1], x2, 0 );
	_GetPyArrayMetaData( myop->obj[2],  r, 0 );

	_Wait(0); // WAIT!!!

	unsigned int xpad = _CalcMaxBlockArraySize( x1->numberOfBlocks );

	// Make room for data block arrays
	char **blocks_x1 = (char**)( ( (unsigned int)( r + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_x2 = (char**)( blocks_x1 + 4 * xpad );
	char **blocks_r  = (char**)( blocks_x2 + 4 * xpad );

	// Fetch pointers to first __ datablocks for vectors
	_GetBlockPointersPart( x1, blocks_x1, 0, xpad, 0 );
	_GetBlockPointersPart( x2, blocks_x2, 0, xpad, 0 );
	_GetBlockPointersPart(  r,  blocks_r, 0, xpad, 0 );

	// Must wait for the block array to finish
	_Wait( 0 );

	// Begin double buffering
	_GetBlock( blocks_x1, info->LS_blockDataArea[0], SPE_id, x1->blockSize*x1->blockSize, 0 );
	_GetBlock( blocks_x2, info->LS_blockDataArea[1], SPE_id, x2->blockSize*x2->blockSize, 0 );
	// No need for the result block

	unsigned int i, j;
	unsigned int idx = 0, nextidx;
	unsigned int numelements = x1->blockSize * x1->blockSize;
	unsigned int blc = xpad;

	vector float *row1, *row2, *res;

	// Foreach block column for the specific block row
	for( i = SPE_id + myop->num_SPES ; i < x1->numberOfBlocksXDim ; i += myop->num_SPES )
	{
		nextidx = idx ^ 1;

		// Fetch next blocks
		_GetBlock( blocks_x1, info->LS_blockDataArea[nextidx*3], i%xpad, numelements, nextidx );
		_GetBlock( blocks_x2, info->LS_blockDataArea[nextidx*3+1], i%xpad, numelements, nextidx );

		_Wait( idx );

		// Elementwise product/arraymultiply
		row1 = (vector float*)( info->LS_blockDataArea[idx*3] );
		row2 = (vector float*)( info->LS_blockDataArea[idx*3+1] );
		res  = (vector float*)( info->LS_blockDataArea[idx*3+2] );
		for( j = 0 ; j < numelements / 4 ; j++ )
		{
			res[j] = spu_mul( row1[j], row2[j] );
		}

		// Send back the result
		_PutAsync( (unsigned int)blocks_r[(i-myop->num_SPES)%xpad], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

		// Get next block array part
		if( i > blc-1 )
		{
			_GetBlockPointersPart( x1, blocks_x1, blc, xpad, 0 );
			_GetBlockPointersPart( x2, blocks_x2, blc, xpad, 0 );
			_GetBlockPointersPart(  r,  blocks_r, blc, xpad, 0 );
			blc += xpad;
			_Wait( 0 );
		}

		idx = nextidx;
	}

	_Wait( 0 );

	// Finish the last block

	// Elementwise product/arraymultiply
	row1 = (vector float*)( info->LS_blockDataArea[idx*3] );
	row2 = (vector float*)( info->LS_blockDataArea[idx*3+1] );
	res  = (vector float*)( info->LS_blockDataArea[idx*3+2] );
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = spu_mul( row1[j], row2[j] );
	}

	// Send back the result
	_PutAsync( (unsigned int)blocks_r[(i-myop->num_SPES)%xpad], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

	_Wait( idx );
}




