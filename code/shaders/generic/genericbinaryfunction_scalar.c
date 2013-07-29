#include "../spefuncs.h"

#include "functions/##header##"

#define _ESIZE1 ##size1##
#define _ESIZE2 ##size2##

/*
 * Generic binary function for op1=array and op2=scalar. No padding correction yet
 *
 * Notes:
 * 1) The minimum number of block pointers transfered from blockData, must be at least the number of SPEs
 * 2) A bus error occurs if the number of SPES is higher than the number of total blocks
 */

void genericbinary( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	_DBGP_UINT( 1 );

	// Make room for python meta data
	PyArrayObject *op1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *op2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *res  = (PyArrayObject *)op2+sizeof( PyArrayObject );

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], op1, 0 );
	_GetPyArrayMetaData( myop->obj[1], op2, 0 );
	_GetPyArrayMetaData( myop->obj[2], res, 0 );

	_Wait(0); // WAIT!!!

	_DBGP_UINT( 2 );

	// Make room for data block arrays
	char **blocks_op1 = (char**)( ( (unsigned int)( res + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_op2 = (char**)( blocks_op1 + _Pad16bytes( op1->numberOfBlocks ) );
	char **blocks_res = (char**)( blocks_op2 + _Pad16bytes( op2->numberOfBlocks ) );

	// Fetch pointers to first __ datablocks for vectors
//	_GetBlockPointersPart( op1, blocks_op1, 0, xpad, 0 );
//	_GetBlockPointersPart( res, blocks_res, 0, xpad, 0 );
	_GetBlockPointers( op1, blocks_op1, op1->numberOfBlocks, _BTAG );
	_GetBlockPointers( res, blocks_res, res->numberOfBlocks, _BTAG );


	_DBGP_UINT( 3 );

	// Get scalar x2
	float scalar_f;
	_GetScalar( op2, blocks_op2, &scalar_f, 1 );

	// Must wait for the block array to finish
	_Wait( _BTAG );

	_DBGP_UINT( 4 );

	unsigned int i, j;
	unsigned int idx = 0, nextidx;
	unsigned int numelements = op1->blockSize * op1->blockSize;

	vector float scalar_v = spu_splats( scalar_f );

	// Begin double buffering
	_GetBlock( blocks_op1, info->LS_blockDataArea[0], SPE_id, numelements, 0 );
	// No need for the result block

	_DBGP_UINT( 5 );

	// Foreach block column for the specific block row
	for( i = SPE_id ; i < op1->numberOfBlocks-myop->num_SPES ; i += myop->num_SPES )
	{
		_DBGP_UINT( 100000 + i );

		nextidx = idx ^ 1;

		// Fetch next blocks
		_GetBlock( blocks_op1, info->LS_blockDataArea[nextidx*3], i + myop->num_SPES, numelements, nextidx );

		_Wait( idx );

		_compute( ##args1## );

		// Send back the result
		_PutAsync( (unsigned int)blocks_res[i], numelements * _ESIZE2, info->LS_blockDataArea[idx*3+2], idx );

//		// Get next block array part
//		if( i > blc-1 )
//		{
//			_GetBlockPointersPart( op1, blocks_op2, blc, xpad, 0 );
//			_GetBlockPointersPart( res, blocks_res, blc, xpad, 0 );
//			blc += xpad;
//			_Wait( 0 );
//		}

		idx = nextidx;
	}

	// Finish the last block
	_Wait( idx );

	_compute( ##args1## );

	// Send back the result
	_PutAsync( (unsigned int)blocks_res[i], numelements * _ESIZE2, info->LS_blockDataArea[idx*3+2], idx );

	_Wait( idx );
}
