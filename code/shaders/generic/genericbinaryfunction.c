#include "../spefuncs.h"

#include "functions/##header##"

/*
 * Generic binary function for vectors, ie one dimensional arrays(can be applied to nd arrays)
 *
 * Notes:
 * 1) The minimum number of block pointers transfered from blockData, must be at least the number of SPEs
 * 2) A bus error occurs if the number of SPES is higher than the number of total blocks
 */

void genericbinary( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	_DBGP_UINT( 1 );

	// Make room for python meta data
	PyArrayObject *x1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *x2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *r  = (PyArrayObject *)x2+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], x1, 0 );
	_GetPyArrayMetaData( myop->obj[1], x2, 0 );
	_GetPyArrayMetaData( myop->obj[2],  r, 0 );

	_Wait(0); // WAIT!!!

	_DBGP_UINT( 2 );

	//unsigned int xpad = _CalcMaxBlockArraySize( x1->numberOfBlocks );

	// Make room for data block arrays
	char **blocks_x1 = (char**)( ( (unsigned int)( r + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_x2 = (char**)( blocks_x1 + _Pad16bytes( x1->numberOfBlocks ) );
	char **blocks_r  = (char**)( blocks_x2 + _Pad16bytes( x2->numberOfBlocks ) );

	// Fetch pointers to first __ datablocks for vectors
//	_GetBlockPointersPart( x1, blocks_x1, 0, xpad, 0 );
//	_GetBlockPointersPart( x2, blocks_x2, 0, xpad, 0 );
//	_GetBlockPointersPart(  r,  blocks_r, 0, xpad, 0 );
	_DBGP_UINT( 2001 );
	_GetBlockPointers( x1, blocks_x1, x1->numberOfBlocks, _BTAG );
	_Wait( _BTAG );

	_DBGP_UINT( 2002 );
	_GetBlockPointers( x2, blocks_x2, x2->numberOfBlocks, _BTAG );
	_Wait( _BTAG );

	_DBGP_UINT( 2003 );
	_GetBlockPointers(  r,  blocks_r,  r->numberOfBlocks, _BTAG );

	// Must wait for the block array to finish
	_Wait( _BTAG );

	_DBGP_UINT( 3 );

	// Begin double buffering
	_GetBlock( blocks_x1, info->LS_blockDataArea[0], SPE_id, x1->blockSize*x1->blockSize, 0 );
	_GetBlock( blocks_x2, info->LS_blockDataArea[1], SPE_id, x2->blockSize*x2->blockSize, 0 );
	// No need for the result block

	unsigned int i, j;
	unsigned int idx = 0, nextidx;
	unsigned int numelements = x1->blockSize * x1->blockSize;

	vector float *row1, *row2, *res;

	_DBGP_UINT( 4 );
	_DBGP_UINT( x1->numberOfBlocks );

	// Foreach block column for the specific block row
	for( i = SPE_id ; i < x1->numberOfBlocks-myop->num_SPES ; i += myop->num_SPES )
	{
		_DBGP_UINT( 100000 + i );

		nextidx = idx ^ 1;

		// Fetch next blocks
		_GetBlock( blocks_x1, info->LS_blockDataArea[nextidx*3], i + myop->num_SPES, numelements, nextidx );
		_GetBlock( blocks_x2, info->LS_blockDataArea[nextidx*3+1], i + myop->num_SPES, numelements, nextidx );

		_Wait( idx );

		//_compute( info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements );
		_compute( ##args1## );

		// Send back the result
		_PutAsync( (unsigned int)blocks_r[i], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

//		// Get next block array part
//		if( i > blc-1 )
//		{
//			_GetBlockPointersPart( x1, blocks_x1, blc, xpad, 0 );
//			_GetBlockPointersPart( x2, blocks_x2, blc, xpad, 0 );
//			_GetBlockPointersPart(  r,  blocks_r, blc, xpad, 0 );
//			blc += xpad;
//			_Wait( 0 );
//		}

		idx = nextidx;
	}

	_Wait( idx );

	_DBGP_UINT( 100000 + i );

	// Finish the last block

	//_compute( info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements );
	_compute( ##args1## );

	// Send back the result
	_PutAsync( (unsigned int)blocks_r[i], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

	_Wait( idx );
}
