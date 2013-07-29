#include "../spefuncs.h"

#include "functions/##header##"

void genericbinary( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
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

	// Make room for data block arrays
	char **blocks_x1 = (char**)( ( (unsigned int)( r + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_x2 = (char**)( blocks_x1 + _Pad16bytes( x1->numberOfBlocks ) );
	char **blocks_r  = (char**)( blocks_x2 + _Pad16bytes( x2->numberOfBlocks ) );
	_GetBlockPointers( x1, blocks_x1, x1->numberOfBlocks, _BTAG );
	_GetBlockPointers( x2, blocks_x2, x2->numberOfBlocks, _BTAG );
	_GetBlockPointers(  r,  blocks_r,  r->numberOfBlocks, _BTAG );

	// Must wait for the block array to finish
	_Wait( _BTAG );

	// Begin double buffering
	_GetBlock( blocks_x1, info->LS_blockDataArea[0], SPE_id, x1->blockSize*x1->blockSize, 0 );
	_GetBlock( blocks_x2, info->LS_blockDataArea[1], SPE_id, x2->blockSize*x2->blockSize, 0 );
	// No need for the result block

	unsigned int i, j;
	unsigned int idx = 0, nextidx;
	unsigned int numelements = x1->blockSize * x1->blockSize;

	// Foreach block column for the specific block row
	for( i = SPE_id ; i < x1->numberOfBlocks-myop->num_SPES ; i += myop->num_SPES )
	{
		nextidx = idx ^ 1;

		// Fetch next blocks
		_GetBlock( blocks_x1, info->LS_blockDataArea[nextidx*3], i + myop->num_SPES, numelements, nextidx );
		_GetBlock( blocks_x2, info->LS_blockDataArea[nextidx*3+1], i + myop->num_SPES, numelements, nextidx );

		_Wait( idx );

		_compute( ##args1## );

		// Send back the result
		_PutAsync( (unsigned int)blocks_r[i], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

		idx = nextidx;
	}

	_Wait( idx );

	// Finish the last block
	_compute( ##args1## );

	// Send back the result
	_PutAsync( (unsigned int)blocks_r[i], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

	_Wait( idx );
}
