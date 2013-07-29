#include "../spefuncs.h"

#include "functions/##header##"

/*
 * Fill one dimensional array with random numbers between 0 and 1. Ie vectors
 *
 * Notes:
 * 1) The minimum number of block pointers transfered from blockData, must be at least the number of SPEs
 * 2) A bus error occurs if the number of SPES is higher than the number of total blocks
 */

void genericfiller( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *r = (PyArrayObject *)info->LS_shaderMemory;

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], r, 0 );

	_Wait(0); // WAIT!!!

	// Make room for data block arrays
	char **blocks_r = (char**)( ( (unsigned int)( r + sizeof( PyArrayObject )) + 127 ) &~ 127 );

	unsigned int i, j;
	unsigned int idx = 0, nextidx;
	unsigned int numelements = r->blockSize * r->blockSize;

	vector float *res;
	//funcs->printuint(r->numberOfBlocksXDim);

	// Fetch pointers to first __ datablocks for vectors
	//_GetBlockPointersPart( r, blocks_r, 0, xpad, 0 );
	_GetBlockPointers( r, blocks_r, r->numberOfBlocks, _BTAG );

	// Must wait for the block array to finish
	_Wait( _BTAG );

	// Begin double buffering

	res  = (vector float*)( info->LS_blockDataArea[idx] );

	_compute( ##args1## );

	// Send back the result
	//_PutAsync( (unsigned int)blocks_r[SPE_id%xpad], numelements * 4, info->LS_blockDataArea[idx], idx );
	_PutAsync( (unsigned int)blocks_r[SPE_id], numelements * 4, info->LS_blockDataArea[idx], idx );

	// Foreach block column for the specific block row
	for( i = SPE_id + myop->num_SPES ; i < r->numberOfBlocks ; i += myop->num_SPES )
	{
		nextidx = idx ^ 1;

		_compute( ##args2## );

		// Send back the result
		_PutAsync( (unsigned int)blocks_r[i], numelements * 4, info->LS_blockDataArea[nextidx], nextidx );

//		// Get next block array part
//		if( i > blc-1 )
//		{
//			_GetBlockPointersPart(  r,  blocks_r, blc, xpad, 0 );
//			blc += xpad;
//			_Wait( 0 );
//		}

		_Wait( idx );

		idx = nextidx;
	}

	_Wait( idx );
}




