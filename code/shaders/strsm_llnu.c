#include "spefuncs.h"

#ifdef _64BLOCKS

void _compute( unsigned int bs, unsigned int k, unsigned int j, vector float *BKJ, float *B, float *A )
{
	vector float *BIJ, aik;
	unsigned int i;
//	BIJ = (vector float*)B;
//	pfv(BIJ);
	for( ; k < 64 ; k++ )
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
	unsigned int i;

	for( ; k < bs ; k++ )
	{
		aik = spu_splats( A[k*bs+j%bs] );

		BIJ = (vector float*)( B + ( k * bs ) );
		// -----> For each vector in row
		for( i = 0 ; i < bs / 4 ; i++ )
		{
			// BIJ[i] = BIJ[i] - BKJ[i] * aik;
			BIJ[i] = spu_sub( BIJ[i], spu_mul( BKJ[i], aik ) );
		}
	}
}

#endif

void _compute2( unsigned int bs, unsigned int k, unsigned int j, vector float *BKJ, float *B, float *A, Functions_t *funcs )
{
	vector float *BIJ, aik;
	unsigned int i;

	funcs->printuint(4400000 + k);

	for( ; k < bs ; k++ )
	{
		aik = spu_splats( A[k*bs+j%bs] );

//		funcs->printuint(k);
		funcs->printuint(k*bs+j%bs);

		BIJ = (vector float*)( B + ( k * bs ) );
		// -----> For each vector in row
		for( i = 0 ; i < bs / 4 ; i++ )
		{
			funcs->printfloatv( &aik );
			funcs->printfloatv( &BIJ[i] );
			funcs->printfloatv( &BKJ[i] );
			// BIJ[i] = BIJ[i] - BKJ[i] * aik;
			BIJ[i] = spu_sub( BIJ[i], spu_mul( BKJ[i], aik ) );
			funcs->printfloatv( &BIJ[i] );
		}
	}
}


void strsm_llnu( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *A     = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *B     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *alpha = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*2;
	//PyArrayObject *cb    = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*3;

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], A, 0 );
	_GetPyArrayMetaData( myop->obj[1], B, 0 );
	_GetPyArrayMetaData( myop->scalar[0], alpha, 0 );
	//_GetPyArrayMetaData( myop->scalar[1], cb, 0 );
	_Wait(0); // WAIT!!!

	// Make room for data block arrays
	char **blocks_A     = (char**)( ( (unsigned int)( alpha + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_B     = (char**)( blocks_A     + _Pad16bytes( A->numberOfBlocks ) );
	char **blocks_alpha = (char**)( blocks_B     + _Pad16bytes( B->numberOfBlocks ) );
	//char **blocks_cb    = (char**)( blocks_alpha + _Pad16bytes( alpha->numberOfBlocks ) );

	// Local vars
	unsigned int idx = 0, nextidx;
	unsigned int idx2 = 0, nextidx2;
	unsigned int i, j, k, l, row, col;

	// Uploadet scalars
	float alpha_f;
	//unsigned int CB;
	_GetScalar( alpha, blocks_alpha, &alpha_f, 1 );
	//_GetScalar( cb, blocks_cb, &CB, 1 );

	vector float alphav = spu_splats(alpha_f);
	//funcs->printuint(1);

	// Fetch pointers to first __ datablocks for vectors
	_GetBlockPointers( B, blocks_B, B->numberOfBlocks, 1 );
	_GetBlockPointers( A, blocks_A, A->numberOfBlocks, 1 );
	_Wait(1);


	vector float *BKJ;

	//funcs->printuint(B->blockSize);

	/*
	 * BKJ = 0
	 * BIJ = 1
	 * AIK = 2
	 */

	unsigned int counter = 0;

	//funcs->printuint(1);

	// For each block column
	// BLOCK >
	for( col = SPE_id ; col < B->numberOfBlocksXDim ; col+=myop->num_SPES )
	{
		_DBGP_UINT( 100000 + col );

		idx = 0;

		// For each block in a column
		// The k'th block row
		// v
		for( k = 0 ; k < B->numberOfBlocksYDim ; k++ )
		{
			//nextidx = idx ^ 1;

			_DBGP_UINT( 200000 + k );

			// Get BKJ
			_GetBlock( blocks_B, info->LS_blockDataArea[idx*3], k*B->numberOfBlocksXDim+col, B->blockSize*B->blockSize, idx );

			// Get AIJ
			_GetBlock( blocks_A, info->LS_blockDataArea[2+idx*3], k*A->numberOfBlocksXDim+k, A->blockSize*A->blockSize, idx );

			_Wait(idx);

			// Foreach row in the same block
			for( l = 1 ; l < B->blockSize ; l++ )
			{
				_DBGP_UINT( 100000 + col );
				_DBGP_UINT( 200000 + k );
				_DBGP_UINT( 520000 + l );

				// Current row
				row = k * B->blockSize + l;

				BKJ = (vector float*)( info->LS_blockDataArea[idx*3] + (l-1) % B->blockSize * B->blockSize * 4 );

				_Wait( idx );

				_DBGP_UINT( 666 );
				_DBGP_MATRIX( A, info->LS_blockDataArea[2+idx*3], funcs );

				_DBGP_UINT( 667 );
				_DBGP_MATRIX( A, info->LS_blockDataArea[idx*3], funcs );

#ifdef _DEBUG
				_compute2( B->blockSize, l, l-1, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[2+idx*3], funcs );
#else
				_compute( B->blockSize, l, l-1, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[2+idx*3] );
#endif

				_DBGP_UINT( 664 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );

				//counter += A->blockSize * ( A->blockSize - l ) * 2;

				// Return the result to the PPE
				_PutAsync( (unsigned int)blocks_B[k*B->numberOfBlocksXDim+col], B->blockSize * B->blockSize * 4, info->LS_blockDataArea[idx*3], idx );
			}



			_DBGP_UINT( 680 );
			_DBGP_MATRIX( B, info->LS_blockDataArea[idx*3], funcs );

			if( k == ( B->numberOfBlocksYDim - 1 ) )
			{
				continue;
			}

			// Compute the rest of the blocks
			idx2 = 0;

			// Get AIK and BIJ
			_GetBlock( blocks_B, info->LS_blockDataArea[1+idx2*3], B->numberOfBlocksXDim+col, B->blockSize*B->blockSize, idx2 );
			_GetBlock( blocks_A, info->LS_blockDataArea[2+idx2*3], A->numberOfBlocksXDim+k, A->blockSize*A->blockSize, idx2 );

			// For each block below BKJ, if any?!
			// The i'th block
			for( i = k+1 ; i < B->numberOfBlocksYDim-1 ; i++ )
			{
				//counter++;
				_DBGP_UINT( 300000 + i );

				nextidx2 = idx2 ^ 1;

				// Get AIK and BIJ double buffered
				_GetBlock( blocks_B, info->LS_blockDataArea[1+nextidx2*3], i*B->numberOfBlocksXDim+col, B->blockSize*B->blockSize, nextidx2 );
				_GetBlock( blocks_A, info->LS_blockDataArea[2+nextidx2*3], i*A->numberOfBlocksXDim+k, A->blockSize*A->blockSize, nextidx2 );

				_Wait(idx2);

				// For each BKJ row above BIJ in the block
				for( l = 0 ; l < B->blockSize ; l++ )
				{
					_DBGP_UINT( 100000 + col );
					_DBGP_UINT( 200000 + k );
					_DBGP_UINT( 520000 + l );

					BKJ = (vector float*)( info->LS_blockDataArea[idx*3] + l % B->blockSize * B->blockSize * 4 );



					_DBGP_UINT( 666 );
					_DBGP_MATRIX( A, info->LS_blockDataArea[2+idx2*3], funcs );

					_DBGP_UINT( 667 );
					_DBGP_MATRIX( A, info->LS_blockDataArea[idx*3], funcs );

					_DBGP_UINT( 668 );
					_DBGP_MATRIX( A, info->LS_blockDataArea[1+idx2*3], funcs );

					_DBGP_UINT( row );

#ifdef _DEBUG
					_compute2( B->blockSize, 0, l, BKJ, (float*)info->LS_blockDataArea[1+idx2*3], (float*)info->LS_blockDataArea[2+idx2*3], funcs );
#else
					_compute( B->blockSize, 0, l, BKJ, (float*)info->LS_blockDataArea[1+idx2*3], (float*)info->LS_blockDataArea[2+idx2*3] );
#endif
					//counter += A->blockSize * A->blockSize * 2;

					_DBGP_UINT( 664 );
					_DBGP_MATRIX( B, info->LS_blockDataArea[1+idx2*3], funcs );

				}

				// Return the result to the PPE
				_PutAsync( (unsigned int)blocks_B[i*B->numberOfBlocksXDim+col], B->blockSize * B->blockSize * 4, info->LS_blockDataArea[1+idx2*3], idx2 );


				_DBGP_UINT( 680 );
				_DBGP_MATRIX( B, info->LS_blockDataArea[1+idx2*3], funcs );

				idx2 = nextidx2;
			}

			_Wait(idx2);

			// For each BKJ row above BIJ in the block
			for( l = 0 ; l < B->blockSize ; l++ )
			{

				BKJ = (vector float*)( info->LS_blockDataArea[idx*3] + l % B->blockSize * B->blockSize * 4 );

#ifdef _DEBUG
				_compute2( B->blockSize, 0, l, BKJ, (float*)info->LS_blockDataArea[1+idx2*3], (float*)info->LS_blockDataArea[2+idx2*3], funcs );
#else
				_compute( B->blockSize, 0, l, BKJ, (float*)info->LS_blockDataArea[1+idx2*3], (float*)info->LS_blockDataArea[2+idx2*3] );
#endif
				//counter += A->blockSize * A->blockSize * 2;

				idx2 = nextidx2;
			}

			// Return the result to the PPE
			_PutAsync( (unsigned int)blocks_B[i*B->numberOfBlocksXDim+col], B->blockSize * B->blockSize * 4, info->LS_blockDataArea[1+idx2*3], idx2 );


			//idx = nextidx;
		}
	}

	//funcs->printfloat(counter);
	//funcs->printuint(counter);

}

