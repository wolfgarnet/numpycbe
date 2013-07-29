#include "spefuncs.h"

//#define _64X64BLOCKS

#ifdef _64X64BLOCKS

void _compute( unsigned int J_i, unsigned int bs, unsigned int sr, void *A, void *x, void *y )
{
	unsigned int row, col;

	vector float *rowA, tempx;
	vector float *rowY = (vector float*)( y+(J_i%64*256) ); // y

	// Foreach element in the block
	for( row = sr ; row < 64 ; row++ )
	{
		rowA = (vector float*)( A + row*256 );
		tempx = spu_splats( ( ((float*)x)[row] ) );

		rowA[0] = spu_add( spu_mul( rowY[0], tempx ), rowA[0] );
		rowA[1] = spu_add( spu_mul( rowY[1], tempx ), rowA[1] );
		rowA[2] = spu_add( spu_mul( rowY[2], tempx ), rowA[2] );
		rowA[3] = spu_add( spu_mul( rowY[3], tempx ), rowA[3] );
		rowA[4] = spu_add( spu_mul( rowY[4], tempx ), rowA[4] );
		rowA[5] = spu_add( spu_mul( rowY[5], tempx ), rowA[5] );
		rowA[6] = spu_add( spu_mul( rowY[6], tempx ), rowA[6] );
		rowA[7] = spu_add( spu_mul( rowY[7], tempx ), rowA[7] );
		rowA[8] = spu_add( spu_mul( rowY[8], tempx ), rowA[8] );
		rowA[9] = spu_add( spu_mul( rowY[9], tempx ), rowA[9] );
		rowA[10] = spu_add( spu_mul( rowY[10], tempx ), rowA[10] );
		rowA[11] = spu_add( spu_mul( rowY[11], tempx ), rowA[11] );
		rowA[12] = spu_add( spu_mul( rowY[12], tempx ), rowA[12] );
		rowA[13] = spu_add( spu_mul( rowY[13], tempx ), rowA[13] );
		rowA[14] = spu_add( spu_mul( rowY[14], tempx ), rowA[14] );
		rowA[15] = spu_add( spu_mul( rowY[15], tempx ), rowA[15] );
	}
}

void _compute_ji( unsigned int J_i, unsigned int bs, unsigned int sr, void *A, void *x, void *y )
{
	unsigned int next4 = (( (J_i+1) % bs + 3 ) &~ 3 );

	//my = (float*)( info->LS_blockDataArea[2]+(J_i%A->blockSize*A->blockSize*4) ); // y
	float *my = (float*)( y+(J_i%bs*256) ); // y
	float *mA, mx;
	vector float *rowA, tempx;
	vector float *rowY = (vector float*)( y+(J_i%64*256) ); // y

	unsigned int row, col;

	for( row = sr ; row < 64 ; row++ )
	{

		mA = (float*)( A + row*256 );
		mx = ((float*)x)[row];

		for( col = (J_i+1) % 64 ; col < next4 ; col++ )
		{
			mA[col] = my[col] * mx + mA[col];
		}

		// For the rest of the row in the block

		rowA = (vector float*)( A + row*256 );

		tempx = spu_splats( ( ((float*)x)[row] ) );

		for( col = next4 ; col < 64 / 4 ; col++ )
		{
			rowA[col] = spu_add( spu_mul( rowY[col], tempx ), rowA[col] );
		}

	}
}

#else

void _compute( unsigned int J_i, unsigned int bs, unsigned int sr, void *A, void *x, void *y )
{
	unsigned int row, col;

	vector float *rowA, tempx;
	vector float *rowY = (vector float*)( y+(J_i%bs*bs*4) ); // y

	// Foreach element in the block
	for( row = sr ; row < bs ; row++ )
	{
		rowA = (vector float*)( A + row*bs*4 );
		tempx = spu_splats( ( ((float*)x)[row] ) );
		for( col = 0 ; col < bs / 4 ; col++ )
		{
			rowA[col] = spu_add( spu_mul( rowY[col], tempx ), rowA[col] );
		}
	}
}

void _compute_ji( unsigned int J_i, unsigned int bs, unsigned int sr, void *A, void *x, void *y )
{
	unsigned int next4 = (( (J_i+1) % bs + 3 ) &~ 3 );

	//my = (float*)( info->LS_blockDataArea[2]+(J_i%A->blockSize*A->blockSize*4) ); // y
	float *my = (float*)( y+(J_i%bs*bs*4) ); // y
	float *mA, mx;
	vector float *rowA, tempx;
	vector float *rowY = (vector float*)( y+(J_i%bs*bs*4) ); // y

	unsigned int row, col;

	for( row = sr ; row < bs ; row++ )
	{

		mA = (float*)( A + row*bs*4 );
		mx = ((float*)x)[row];

		for( col = (J_i+1) % bs ; col < next4 ; col++ )
		{
			mA[col] = my[col] * mx + mA[col];
		}

		// For the rest of the row in the block

		rowA = (vector float*)( A + row*bs*4 );

		tempx = spu_splats( ( ((float*)x)[row] ) );

		for( col = next4 ; col < bs / 4 ; col++ )
		{
			rowA[col] = spu_add( spu_mul( rowY[col], tempx ), rowA[col] );
		}

	}
}

#endif

void sger_solve( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *A     = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *x     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *alpha = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*2;
	PyArrayObject *J     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*3;


	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], A, 0 );
	_GetPyArrayMetaData( myop->obj[1], x, 0 );
	_GetPyArrayMetaData( myop->scalar[0], alpha, 0 );
	_GetPyArrayMetaData( myop->scalar[1], J, 0 );
	_Wait(0); // WAIT!!!

	// Make room for data block arrays
	char **blocks_A     = (char**)( ( (unsigned int)( J + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_x     = (char**)( blocks_A     + _Pad16bytes( A->numberOfBlocks ) );
	char **blocks_alpha = (char**)( blocks_x     + _Pad16bytes( x->numberOfBlocks ) );
	char **blocks_J     = (char**)( blocks_alpha + _Pad16bytes( alpha->numberOfBlocks ) );

	// Local vars
	unsigned int idx = 0, nextidx;
	unsigned int xidx = 0, xnextidx;
	unsigned int i, j, row, col, e;
	unsigned int nextblc = _GET_NUM_BLOCKS;

	unsigned int c = 0;

	unsigned int Afourth  = A->blockSize / 4;
	unsigned int xfourth  = x->blockSize / 4;
	unsigned int AAfourth = A->blockSize*A->blockSize / 4;
	unsigned int t = 0;

	// Uploadet scalars
	float alpha_f;
	unsigned int J_i;
	_GetScalar( alpha, blocks_alpha, &alpha_f, 1 );
	_GetScalar( J, blocks_J, &J_i, 1 );
	vector float alphav = spu_splats(alpha_f);
	vector float tempx;

	_DBGP_UINT( J_i );
	_DBGP_FLOAT( alpha_f );
//	funcs->printuint( J_i );
//	funcs->printfloat( alpha_f );

	vector float *rowA, *rowX, *rowY;
	unsigned int offset = 0, sc, sr;
	int xc = -1;
	unsigned int next4;

	float *mA, *my, mx;

	unsigned int col_x = (J_i+1) / A->blockSize;
	unsigned int row_x = J_i / A->blockSize * A->numberOfBlocksXDim;

	unsigned int col_y = J_i / A->blockSize;
	unsigned int row_y = (J_i+1) / A->blockSize * A->numberOfBlocksXDim;

	unsigned int col_A = (J_i+1) / A->blockSize;
	//unsigned int row_A = (J_i+1) / A->blockSize * A->numberOfBlocksXDim;
	unsigned int row_A = (J_i+1) / A->blockSize;

	// Fetch pointers to first __ datablocks for vectors
	_GetBlockPointers( A, blocks_A, A->numberOfBlocks, _BTAG );
	_GetBlockPointers( x, blocks_x, x->numberOfBlocks, _BTAG );



	// Foreach block row assigned to the SPE
	for( i = row_A+SPE_id ; i < A->numberOfBlocksYDim ; i += myop->num_SPES )
	{
		_DBGP_UINT( 100000 + i );
		idx = 0;

		unsigned int offset = i * A->numberOfBlocksXDim;

		// Wait for the block arrays to transfer
		_Wait( _BTAG );

		// Get x if needed(Not done yet!!!!)
		_DBGP_UINT( i/x->blockSize );
		_GetBlock( blocks_x, info->LS_blockDataArea[1], i/x->blockSize, x->blockSize*x->blockSize, _XTAG );

		// Get the first blocks
		_DBGP_UINT( offset+col_A );
		_DBGP_UINT( col_A );
		_DBGP_UINT( offset );
		_GetBlock( blocks_A, info->LS_blockDataArea[idx*3], offset+col_A, A->blockSize*A->blockSize, idx );

		_Wait( _XTAG );

		_DBGP_UINT( 333 );

		// Preprocess entire x
		rowX = (vector float*)( info->LS_blockDataArea[1] );

		for( row = 0 ; row < x->blockSize*x->blockSize / 4 ; row++ )
		{
			rowX[row] = spu_mul( rowX[row], alphav );
		}

		sr = 0;
		if( i == row_A )
		{
			sr = (J_i+1) % A->blockSize;
		}

		// Foreach block column for the specific block row
		for( j = col_A ; j < A->numberOfBlocksXDim-1 ; j++ )
		{
			_DBGP_UINT( 200000 + j );

			nextidx = idx ^ 1;

			// A
			_GetBlock( blocks_A, info->LS_blockDataArea[nextidx*3], offset+j+1, A->blockSize*A->blockSize, nextidx );

			// y
			_GetRow( blocks_A, info->LS_blockDataArea[2], J_i/A->blockSize*A->numberOfBlocksXDim + j, J_i%A->blockSize, A->blockSize, _YTAG );

			rowY = (vector float*)( info->LS_blockDataArea[2]+(J_i%A->blockSize*A->blockSize*4) ); // y

			_Wait( idx );
			_Wait( _YTAG );

			// Compute

			// The first block row. Misaligned both in rows and columns
			if( j == col_A )
			{
				_compute_ji( J_i, A->blockSize, sr, info->LS_blockDataArea[idx*3], info->LS_blockDataArea[1], info->LS_blockDataArea[2] );
			}

			// Not misaligned at all
			else
			{
				_compute( J_i, A->blockSize, sr, info->LS_blockDataArea[idx*3], info->LS_blockDataArea[1], info->LS_blockDataArea[2] );
			}

			// Save result
			_PutAsync( ((unsigned int*)blocks_A)[offset+j], A->blockSize*A->blockSize * 4, info->LS_blockDataArea[idx*3], idx );

			idx = nextidx;
		}

		_DBGP_UINT( 200000 + j );

		_GetRow( blocks_A, info->LS_blockDataArea[2], J_i/A->blockSize*A->numberOfBlocksXDim + j, J_i%A->blockSize, A->blockSize, _YTAG );

		rowY = (vector float*)( info->LS_blockDataArea[2]+(J_i%A->blockSize*A->blockSize*4) ); // y

		_Wait( idx );
		_Wait( _YTAG );


		// Compute
		// If misaligned data with only one block(or the last?)
		if( j == col_A )
		{
			_DBGP_UINT( 111 );
			_compute_ji( J_i, A->blockSize, sr, info->LS_blockDataArea[idx*3], info->LS_blockDataArea[1], info->LS_blockDataArea[2] );
		}
		else
		{
			_DBGP_UINT( 222 );
			_compute( J_i, A->blockSize, sr, info->LS_blockDataArea[idx*3], info->LS_blockDataArea[1], info->LS_blockDataArea[2] );
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





