#include "../spefuncs.h"

#include "functions/##header##"

/*
 * Generic unary function for vectors, ie one dimensional arrays(can be applied to nd arrays)
 *
 * Notes:
 * 1) The minimum number of block pointers transfered from blockData, must be at least the number of SPEs
 * 2) A bus error occurs if the number of SPES is higher than the number of total blocks
 */

void genericunary_scalarreturnvalue( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	_DBGP_UINT( 1 );

	// Make room for python meta data
	PyArrayObject *x1 = (PyArrayObject *)info->LS_shaderMemory;

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], x1, 0 );

	_Wait(0); // WAIT!!!

	_DBGP_UINT( 2 );

	unsigned int xpad = _CalcMaxBlockArraySize( x1->numberOfBlocks );

	// Make room for data block arrays
	char **blocks_x1 = (char**)( ( (unsigned int)( x1 + sizeof( PyArrayObject )) + 127 ) &~ 127 );

	// Fetch pointers to first __ datablocks for vectors
	//_GetBlockPointersPart( x1, blocks_x1, 0, xpad, 0 );
	_GetBlockPointers( x1, blocks_x1, x1->numberOfBlocks, _BTAG );

	// Must wait for the block array to finish
	_Wait( _BTAG );

	_DBGP_UINT( 3 );

	// Begin double buffering
	_GetBlock( blocks_x1, info->LS_blockDataArea[0], SPE_id, x1->blockSize*x1->blockSize, 0 );

	// No need for the result block

	unsigned int i, j;
	unsigned int idx = 0, nextidx;
	unsigned int numelements = x1->blockSize * x1->blockSize;
	unsigned int blc = xpad;

	float res_scalar;
	vector float *row1, res = { 0, 0, 0, 0 };

	_DBGP_UINT( 123 );

	// Foreach block column for the specific block row
	for( i = SPE_id ; i < x1->numberOfBlocks-myop->num_SPES ; i += myop->num_SPES )
	{
		_DBGP_UINT( 100000 + i );

		nextidx = idx ^ 1;

		// Fetch next blocks
		_GetBlock( blocks_x1, info->LS_blockDataArea[nextidx*3], i + myop->num_SPES, numelements, nextidx );

		_Wait( idx );

		_compute( ##args1## );

//		// Get next block array part
//		if( i > blc-1 )
//		{
//			_GetBlockPointersPart( x1, blocks_x1, blc, xpad, 0 );
//			blc += xpad;
//			_Wait( 0 );
//		}

		idx = nextidx;
	}

	_Wait( 0 );

	// Finish the last block
	_compute( ##args1## );

	res_scalar = _finish( ##args2## );

	// Send back the result
	spu_mfcdma32( &res_scalar, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );

	_Wait( idx );
}


