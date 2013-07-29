#include "spefuncs.h"

void sger( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *A     = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *x     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *y     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*2;
	PyArrayObject *alpha = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*3;
	PyArrayObject *J     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*4;

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], A, 0 );
	_GetPyArrayMetaData( myop->obj[1], x, 0 );
	_GetPyArrayMetaData( myop->obj[2], y, 0 );
	_GetPyArrayMetaData( myop->scalar[0], alpha, 0 );
	_GetPyArrayMetaData( myop->scalar[1], J, 0 );
	_Wait(0); // WAIT!!!

	// Make room for data block arrays
	char **blocks_A     = (char**)( ( (unsigned int)( J + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_x     = (char**)( blocks_A     + _Pad16bytes( A->numberOfBlocks ) );
	char **blocks_y     = (char**)( blocks_x     + _Pad16bytes( x->numberOfBlocks ) );
	char **blocks_alpha = (char**)( blocks_y     + _Pad16bytes( y->numberOfBlocks ) );
	char **blocks_J     = (char**)( blocks_alpha + _Pad16bytes( alpha->numberOfBlocks ) );

	// Local vars
	unsigned int idx = 0, nextidx;
	unsigned int xidx = 0, xnextidx;
	unsigned int i, j, row, col, element;
	unsigned int nextblc = _GET_NUM_BLOCKS;

	unsigned int c = 0;

	unsigned int Afourth  = A->blockSize / 4;
	unsigned int xfourth  = x->blockSize / 4;
	unsigned int AAfourth = A->blockSize*A->blockSize / 4;
	unsigned int t = 0;

	// Uploadet scalars
	float alpha_f, J_f;
	_GetScalar( alpha, blocks_alpha, &alpha_f, 1 );
	_GetScalar( J, blocks_J, &J_f, 1 );
	vector float alphav = spu_splats(alpha_f);
	vector float tempx;


	vector float *rowA, *rowX, *rowY;
	unsigned int offset = 0;
	int xc = -1;

	// Fetch pointers to first __ datablocks for vectors
	_GetBlockPointers( x, blocks_x, x->numberOfBlocks, _BTAG );
	_GetBlockPointers( y, blocks_y, y->numberOfBlocks, _BTAG );
	_GetBlockPointers( A, blocks_A, A->numberOfBlocks, _BTAG );

	// Foreach block row assigned to the SPE
	for( i = SPE_id ; i < A->numberOfBlocksYDim ; i += myop->num_SPES )
	{
		idx = 0;

		unsigned int offset = i * A->numberOfBlocksXDim;

		// Wait for the block arrays to transfer
		_Wait( _BTAG );

		// Get the first blocks
		_GetBlock( blocks_A, info->LS_blockDataArea[0+idx*3], offset, A->blockSize*A->blockSize, idx );

		// Foreach block column for the specific block row
		for( j = 0 ; j < A->numberOfBlocksXDim-1 ; j++ )
		{
			nextidx = idx ^ 1;

			_GetBlock( blocks_A, info->LS_blockDataArea[0+nextidx*3], offset+j+1, A->blockSize*A->blockSize, nextidx );

			// Preprocess X if needed
			if( xc < (int)i )
			{
				_GetBlock( blocks_x, info->LS_blockDataArea[1], i/x->blockSize, x->blockSize*x->blockSize, _XTAG );
				_Wait( _XTAG );
				rowX = (vector float*)( info->LS_blockDataArea[1] + ( j % x->blockSize ) * x->blockSize * i * 4 );

				for( row = 0 ; row < x->blockSize / 4 ; row++ )
				{
					rowX[row] = spu_mul( rowX[row], alphav );
				}
				xc += x->blockSize;
			}

			// Do we need another block from the vector y?
			if( j % y->blockSize == 0 )
			{
				_GetBlock( blocks_y, info->LS_blockDataArea[2], j / y->blockSize, y->blockSize*x->blockSize, _YTAG );
			}

			_Wait( idx );
			_Wait( _YTAG );

			// Compute

			rowY = (vector float*)( info->LS_blockDataArea[2] + ( j % y->blockSize ) * y->blockSize * 4 );

			// Foreach element in the block
			for( row = 0 ; row < A->blockSize ; row++ )
			{
				rowA = (vector float*)( info->LS_blockDataArea[idx*3] + row*A->blockSize*4 );
				tempx = spu_splats( ( ((float*)info->LS_blockDataArea[1])[row] ) );

				for( col = 0 ; col < Afourth ; col++ )
				{
//					funcs->printfloatv( &rowA[col] );
//					funcs->printfloatv( &rowY[col] );
					rowA[col] = spu_add( spu_mul( rowY[col], tempx ), rowA[col] );
//					funcs->printfloatv( &rowA[col] );
				}
			}

			// Save result
			_PutAsync( ((unsigned int*)blocks_A)[offset+j], A->blockSize*A->blockSize * 4, info->LS_blockDataArea[idx*3], idx );

			idx = nextidx;
		}

		// Preprocess X if needed
		if( xc < (int)i )
		{
//			funcs->printuint( 888 );
//			funcs->printint( xc );
			_GetBlock( blocks_x, info->LS_blockDataArea[1], i, x->blockSize*x->blockSize, _XTAG );
			_Wait( _XTAG );
			rowX = (vector float*)( info->LS_blockDataArea[1] + ( j % x->blockSize ) * x->blockSize * i * 4 );
			for( row = 0 ; row < x->blockSize / 4 ; row++ )
			{
				rowX[row] = spu_mul( rowX[row], alphav );
			}
			xc += x->blockSize;
//			funcs->printint( xc );
		}

		// Do we need another block from the vector y?
		if( j % y->blockSize == 0 )
		{
			_GetBlock( blocks_y, info->LS_blockDataArea[2], j / y->blockSize, y->blockSize*x->blockSize, _YTAG );
		}

		_Wait( idx );
		_Wait( _YTAG );

		// Compute

		rowY = (vector float*)( info->LS_blockDataArea[2] + ( j % y->blockSize ) * y->blockSize * 4 );

		// Foreach element in the block
		for( row = 0 ; row < A->blockSize ; row++ )
		{
			rowA = (vector float*)( info->LS_blockDataArea[idx*3] + row*A->blockSize*4 );
			//tempx = spu_splats( ( *((float*)info->LS_blockDataArea[1]) + ( ( j % x->blockSize ) * x->blockSize * i + row ) * 4 ) );
			tempx = spu_splats( ( ((float*)info->LS_blockDataArea[1])[row] ) );
//			funcs->printuint( 1234 );
//			funcs->printfloatv( &tempx );
			for( col = 0 ; col < Afourth ; col++ )
			{

//				funcs->printfloatv( &rowY[col] );
				rowA[col] = spu_add( spu_mul( rowY[col], tempx ), rowA[col] );
//				funcs->printfloatv( &rowA[col] );
			}
		}

		// Save result
		_PutAsync( ((unsigned int*)blocks_A)[offset+j], A->blockSize*A->blockSize * 4, info->LS_blockDataArea[idx*3], idx );

		_Wait( idx );
	}
}


/*
 *         A                X             Y           alpha
 * | 0,0 0,1 0,2 0,3 |   | 0,0 |
 * | 1,0 1,1 1,2 1,3 | + | 1,0 | * | 0,0 0,1 0,2 | * | 0,0 |
 * | 2,0 2,1 2,2 2,3 |   | 2,0 |
 * | 3.0 3,1 3,2 3,3 |   | 3,0 |
 *
 */





