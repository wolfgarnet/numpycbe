#include "../spefuncs.h"

#include "functions/##header##"

/*
 * Generic unary function for vectors, ie one dimensional arrays(can be applied to nd arrays)
 *
 * Notes:
 * 1) The minimum number of block pointers transfered from blockData, must be at least the number of SPEs
 * 2) A bus error occurs if the number of SPES is higher than the number of total blocks
 */

void genericunary( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *op1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *res = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], op1, 0 );
	_GetPyArrayMetaData( myop->obj[1], res, 0 );

	_Wait(0); // WAIT!!!

	unsigned int xpad = _CalcMaxBlockArraySize( op1->numberOfBlocks );

	// Make room for data block arrays
	char **blocks_op1 = (char**)( ( (unsigned int)( res + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_res = (char**)( blocks_op1 + 4 * xpad );

	// Fetch pointers to first __ datablocks for vectors
	_GetBlockPointersPart( op1, blocks_op1, 0, xpad, 0 );
	_GetBlockPointersPart( res, blocks_res, 0, xpad, 0 );

	// Must wait for the block array to finish
	_Wait( 0 );

	unsigned int i, j;
	unsigned int idx = 0, nextidx;
	unsigned int numelements = op1->blockSize * op1->blockSize;
	unsigned int blc = xpad;

	// Begin double buffering
	_GetBlock( blocks_op1, info->LS_blockDataArea[0], SPE_id, numelements, 0 );
	// No need for the result block

	// Foreach block column for the specific block row
	for( i = SPE_id + myop->num_SPES ; i < op1->numberOfBlocks ; i += myop->num_SPES )
	{
		nextidx = idx ^ 1;

		// Fetch next blocks
		_GetBlock( blocks_op1, info->LS_blockDataArea[nextidx*3], i%xpad, numelements, nextidx );

		_Wait( idx );

		_compute( ##args1## );

		// Send back the result
		_PutAsync( (unsigned int)blocks_res[(i-myop->num_SPES)%xpad], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

		// Get next block array part
		if( i > blc-1 )
		{
			_GetBlockPointersPart( op1, blocks_op1, blc, xpad, 0 );
			_GetBlockPointersPart( res, blocks_res, blc, xpad, 0 );
			blc += xpad;
			_Wait( 0 );
		}

		idx = nextidx;
	}

	_Wait( 0 );

	// Finish the last block

	//_compute( info->LS_blockDataArea[idx*3], info->LS_blockDataArea[idx*3+1], info->LS_blockDataArea[idx*3+2], numelements );
	_compute( ##args1## );

	// Send back the result
	_PutAsync( (unsigned int)blocks_res[(i-myop->num_SPES)%xpad], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

	_Wait( idx );
}

