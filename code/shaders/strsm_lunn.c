#include "spefuncs.h"

#include "divf4.h"

#ifdef _64BLOCKS

void _compute( unsigned int bs, unsigned int k, unsigned int j, vector float *BKJ, float *B, float *A )
{
	vector float *BIJ, aik;
	unsigned int i, k2;
//	BIJ = (vector float*)B;
//	pfv(BIJ);
	//for( ; k < 64 ; k++ )
	for( k2 = 0 ; k2 < k ; k2++ )
	{
		aik = spu_splats( A[k*64+j%64] );

		BIJ = (vector float*)( B + ( k * 64 ) );


		BIJ[0] = spu_sub( BIJ[0], spu_mul( BKJ[0], aik ) );
		BIJ[1] = spu_sub( BIJ[1], spu_mul( BKJ[1], aik ) );
		BIJ[2] = spu_sub( BIJ[2], spu_mul( BKJ[2], aik ) );
		BIJ[3] = spu_sub( BIJ[3], spu_mul( BKJ[3], aik ) );
		BIJ[4] = spu_sub( BIJ[4], spu_mul( BKJ[4], aik ) );
		BIJ[5] = spu_sub( BIJ[5], spu_mul( BKJ[5], aik ) );
		BIJ[6] = spu_sub( BIJ[6], spu_mul( BKJ[6], aik ) );
		BIJ[7] = spu_sub( BIJ[7], spu_mul( BKJ[7], aik ) );
		BIJ[8] = spu_sub( BIJ[8], spu_mul( BKJ[8], aik ) );
		BIJ[9] = spu_sub( BIJ[9], spu_mul( BKJ[9], aik ) );
		BIJ[10] = spu_sub( BIJ[10], spu_mul( BKJ[10], aik ) );
		BIJ[11] = spu_sub( BIJ[11], spu_mul( BKJ[11], aik ) );
		BIJ[12] = spu_sub( BIJ[12], spu_mul( BKJ[12], aik ) );
		BIJ[13] = spu_sub( BIJ[13], spu_mul( BKJ[13], aik ) );
		BIJ[14] = spu_sub( BIJ[14], spu_mul( BKJ[14], aik ) );
		BIJ[15] = spu_sub( BIJ[15], spu_mul( BKJ[15], aik ) );
	}
}

#else

void _compute( unsigned int bs, unsigned int k, unsigned int j, vector float *BKJ, float *B, float *A )
{
	vector float *BIJ, aik;
	unsigned int i, k2;

	for( k2 = 0 ; k2 < k ; k2++ )
	{
		aik = spu_splats( A[k2*bs+j%bs] );

		BIJ = (vector float*)( B + ( k2 * bs ) );
		// -----> For each vector in row
		for( i = 0 ; i < bs / 4 ; i++ )
		{
			// BIJ[i] = BIJ[i] - BKJ[i] * aik;
			BIJ[i] = spu_sub( BIJ[i], spu_mul( BKJ[i], aik ) );
		}
	}
}


void _compute2( unsigned int bs, unsigned int k, unsigned int j, vector float *BKJ, float *B, float *A, Functions_t *funcs )
{
	vector float *BIJ, aik;
	unsigned int i, k2;

	funcs->printuint(4400000 + k);

	for( k2 = 0 ; k2 < k ; k2++ )
	{
		aik = spu_splats( A[k2*bs+j%bs] );

		//funcs->printuint(k2);
		funcs->printuint(k2*bs+j%bs);

		BIJ = (vector float*)( B + ( k2 * bs ) );
		// -----> For each vector in row
		for( i = 0 ; i < bs / 4 ; i++ )
		{
			funcs->printfloatv(&aik);
			funcs->printfloatv(&BIJ[i]);
			funcs->printfloatv(&BKJ[i]);
			// BIJ[i] = BIJ[i] - BKJ[i] * aik;
			BIJ[i] = spu_sub( BIJ[i], spu_mul( BKJ[i], aik ) );
			funcs->printfloatv(&BIJ[i]);
		}
	}
}

#endif


void strsm_lunn( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *A     = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *B     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *alpha = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*2;

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], A, 0 );
	_GetPyArrayMetaData( myop->obj[1], B, 0 );
	_GetPyArrayMetaData( myop->scalar[0], alpha, 0 );
	_Wait(0); // WAIT!!!

	// Make room for data block arrays
	char **blocks_A     = (char**)( ( (unsigned int)( alpha + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_B     = (char**)( blocks_A     + _Pad16bytes( A->numberOfBlocks ) );
	char **blocks_alpha = (char**)( blocks_B     + _Pad16bytes( B->numberOfBlocks ) );

	// Local vars
	unsigned int idx = 0, nextidx;
	unsigned int idx2 = 0, nextidx2;
	unsigned int i, j, k, l, row, row1, col, q;

//	unsigned int c = 0;
//
//	unsigned int Afourth  = A->blockSize / 4;
//	unsigned int xfourth  = x->blockSize / 4;
//	unsigned int AAfourth = A->blockSize*A->blockSize / 4;
//	unsigned int t = 0;

	// Uploadet scalars
	float alpha_f, beta_f;
	_GetScalar( alpha, blocks_alpha, &alpha_f, 1 );

	vector float alphav = spu_splats(alpha_f);

	// Fetch pointers to first __ datablocks for vectors
	_GetBlockPointers( B, blocks_B, B->numberOfBlocks, _BTAG );
	_GetBlockPointers( A, blocks_A, A->numberOfBlocks, _BTAG );
	_Wait(_BTAG);

	vector float *BKJ, *BIJ, akk, aik;
	float *AIK;


	unsigned int counter = 0;

	int ain, ain2;

	// For each block column
	// BLOCK >
	for( col = SPE_id ; col < B->numberOfBlocksXDim ; col+=myop->num_SPES )
	{
		_DBGP_UINT( 100000 + col );
		// For each block in a column -> BKJ
		// The k'th block row
		for( k = B->numberOfBlocksYDim-1 ; k+1 > 0  ; k-- )
		{

			_DBGP_UINT( 200000 + k );

			// Get AKK
			_GetBlock( blocks_A, info->LS_blockDataArea[4], k*B->numberOfBlocksXDim+k, B->blockSize*B->blockSize, 11 );

			// Get B
			_GetBlock( blocks_B, info->LS_blockDataArea[1], k*B->numberOfBlocksXDim+col, B->blockSize*B->blockSize, 11 );

			// Calculate the BKJ block

			row1 = B->blockSize -1;
			if( k == B->numberOfBlocksYDim-1 )
			{
				row1 -= B->paddingy;
			}
			row = row1;

			_Wait( 11 );

			// For each row in BKJ, from bs -> 0
			for( ; row+1 > 0 ; row-- )
			{
				_DBGP_UINT( 410000 + row );

				BKJ = (vector float*)( info->LS_blockDataArea[1] + row * B->blockSize * 4 );


				// Preprocess BKJ
				akk = spu_splats( ((float*)info->LS_blockDataArea[4])[row*A->blockSize+row] );

				for( j = 0 ; j < B->blockSize / 4 ; j++ )
				{
					BKJ[j] = _divf4( BKJ[j], akk );
				}

				// Do not process the final row
				// Make flow
				if( row == 0 )
				{
					_PutRow2Row( blocks_B, info->LS_blockDataArea[1], k*B->numberOfBlocksXDim+col, row, B->blockSize, 11 );
					continue;
				}

				// Foreach row in the same block
				BIJ = (vector float*)( info->LS_blockDataArea[idx*3] + ( row * B->blockSize * 4 ) );

				for( l = 0 ; l < B->blockSize / 4 ; l++ )
				{
					BIJ[l] = BKJ[l];
				}

				_DBGP_UINT( row );


				_DBGP_UINT( 660 );
				_DBGP_MATRIX( A, info->LS_blockDataArea[4], funcs );

				_DBGP_UINT( 661 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[1], funcs );

#ifdef _DEBUG
				_compute2( B->blockSize, row, row, BKJ, (float*)info->LS_blockDataArea[1], (float*)info->LS_blockDataArea[4], funcs );
#else
				_compute( B->blockSize, row, row, BKJ, (float*)info->LS_blockDataArea[1], (float*)info->LS_blockDataArea[4] );
#endif

				_DBGP_UINT( 664 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[1], funcs );

			}


			_DBGP_UINT( 680 );
			_DBGP_MATRIX( B, info->LS_blockDataArea[1], funcs );

			_DBGP_UINT( k*B->numberOfBlocksXDim+col );

			// Return the result to the PPE
			_PutAsync( (unsigned int)blocks_B[k*B->numberOfBlocksXDim+col], B->blockSize * B->blockSize * 4, info->LS_blockDataArea[1], idx );

			// Compute the rest of the blocks!
			if ( k == 0 )
			{
				continue;
			}

			// Foreach block above BKJ

			// Begin double buffering
			idx = 0;

			// BIJ + AIK
			_GetBlock( blocks_B, info->LS_blockDataArea[idx*3], col, B->blockSize*B->blockSize, idx );
			_GetBlock( blocks_A, info->LS_blockDataArea[2+idx*3], k, A->blockSize*A->blockSize, idx );

			// Foreach block above BKJ
			for( i = 0 ; i < k-1 ; i++ )
			{
				//counter++;
				_DBGP_UINT( 300000 + i );

				nextidx = idx ^ 1;

				// BIJ + AIK
				_GetBlock( blocks_B, info->LS_blockDataArea[nextidx*3], (i+1)*B->numberOfBlocksXDim+col, B->blockSize*B->blockSize, nextidx );
				_GetBlock( blocks_A, info->LS_blockDataArea[2+nextidx*3], (i+1)*A->numberOfBlocksXDim+k, A->blockSize*A->blockSize, nextidx );

				_Wait(idx);

				_DBGP_UINT( 660 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );

				row = row1;

				//for( j = 0 ; j < row1 ; j++ )
				for( ; row+1 > 0 ; row-- )
				{
					_DBGP_UINT( 420000 + row );

					_DBGP_UINT( 660 );
					_DBGP_MATRIX( A, info->LS_blockDataArea[2+idx*3], funcs );

					_DBGP_UINT( 661 );
					_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );

					_DBGP_UINT( 662 );
					_DBGP_MATRIX( B, info->LS_blockDataArea[1], funcs );

					_DBGP_UINT( row );
					BKJ = (vector float*)( info->LS_blockDataArea[1] + row * B->blockSize * 4 );

#ifdef _DEBUG
					_compute2( B->blockSize, B->blockSize, row, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[2+idx*3], funcs );
#else
					_compute( B->blockSize, B->blockSize, row, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[2+idx*3] );
#endif

					_DBGP_UINT( 664 );
					_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );
				}

				// Return the result to the PPE
				_PutAsync( (unsigned int)blocks_B[i*B->numberOfBlocksXDim+col], B->blockSize * B->blockSize * 4, info->LS_blockDataArea[idx*3], idx );

				_DBGP_UINT( 680 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );


				idx = nextidx;
			}

			_Wait(idx);

			_DBGP_UINT( 310000 + i );

			row = row1;
//			funcs->printuint(900000 + idx);

			//for( j = 0 ; j < row1 ; j++ )
			for( ; row+1 > 0 ; row-- )
			{
				_DBGP_UINT( 430000 + row );

				_DBGP_UINT( 660 );
				_DBGP_MATRIX( A, info->LS_blockDataArea[2+idx*3], funcs );

				_DBGP_UINT( 661 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );

				_DBGP_UINT( 662 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[1], funcs );

				BKJ = (vector float*)( info->LS_blockDataArea[1] + row * B->blockSize * 4 );

#ifdef _DEBUG
				_compute2( B->blockSize, B->blockSize, row, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[2+idx*3], funcs );
#else
				_compute( B->blockSize, B->blockSize, row, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[2+idx*3] );
#endif

				_DBGP_UINT( 664 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );
			}

			_DBGP_UINT( 680 );
			_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );

			// Return the result to the PPE
			_PutAsync( (unsigned int)blocks_B[i*B->numberOfBlocksXDim+col], B->blockSize * B->blockSize * 4, info->LS_blockDataArea[idx*3], idx );

		}


	}

	//funcs->printuint(counter);

}

