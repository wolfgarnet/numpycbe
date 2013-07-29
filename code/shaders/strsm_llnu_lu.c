#include "spefuncs.h"

#ifdef _64BLOCKS

void _compute( unsigned int bs, unsigned int k, unsigned int row, vector float *BKJ, float *B, float *A )
{
	vector float *BIJ, aik;
	unsigned int i;
//	BIJ = (vector float*)B;
//	pfv(BIJ);
	for( ; k < 64 ; k++ )
	{
		aik = spu_splats( A[k*64+row] );

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

void _compute( unsigned int bs, unsigned int k, unsigned int row, vector float *BKJ, float *B, float *A )
{
	vector float *BIJ, aik;
	unsigned int i;

	for( ; k < bs ; k++ )
	{
		aik = spu_splats( A[k*bs+row] );

		BIJ = (vector float*)( B + ( k * bs ) );
		// -----> For each vector in row
		for( i = 0 ; i < bs / 4 ; i++ )
		{
			// BIJ[i] = BIJ[i] - BKJ[i] * aik;
			BIJ[i] = spu_sub( BIJ[i], spu_mul( BKJ[i], aik ) );
		}
	}
}

void _compute2( unsigned int bs, unsigned int k, unsigned int row, vector float *BKJ, float *B, float *A, Functions_t *funcs  )
{
	vector float *BIJ, aik;
	unsigned int i;

	for( ; k < bs ; k++ )
	{
		aik = spu_splats( A[k*bs+row] );

		BIJ = (vector float*)( B + ( k * bs ) );
		funcs->printuint( 900000 + k );
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



#endif


void strsm_llnu_lu( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	_DBGP_UINT( 1 );

	// Make room for python meta data
	PyArrayObject *A     = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *B     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *alpha = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*2;
	PyArrayObject *cb    = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*3;

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], A, 0 );
	_GetPyArrayMetaData( myop->obj[1], B, 0 );
	_GetPyArrayMetaData( myop->scalar[0], alpha, 0 );
	_GetPyArrayMetaData( myop->scalar[1], cb, 0 );
	_Wait(0); // WAIT!!!

	_DBGP_UINT( 2 );

	// Make room for data block arrays
	char **blocks_A     = (char**)( ( (unsigned int)( cb + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_B     = (char**)( blocks_A     + _Pad16bytes( A->numberOfBlocks ) );
	char **blocks_alpha = (char**)( blocks_B     + _Pad16bytes( B->numberOfBlocks ) );
	char **blocks_cb    = (char**)( blocks_alpha + _Pad16bytes( alpha->numberOfBlocks ) );

	// Local vars
	unsigned int idx = 0, nextidx;
	unsigned int i, j, k, l, row, col;

//	unsigned int c = 0;
//
//	unsigned int Afourth  = A->blockSize / 4;
//	unsigned int xfourth  = x->blockSize / 4;
//	unsigned int AAfourth = A->blockSize*A->blockSize / 4;
//	unsigned int t = 0;

	// Uploadet scalars
	float alpha_f, beta_f;
	unsigned int CB;
	_GetScalar( alpha, blocks_alpha, &alpha_f, 1 );
	_GetScalar( cb, blocks_cb, &CB, 1 );

	_DBGP_UINT( 3 );

	vector float alphav = spu_splats(alpha_f);

	// Fetch pointers to first __ datablocks for vectors
	_GetBlockPointers( B, blocks_B, B->numberOfBlocks, _BTAG );
	_GetBlockPointers( A, blocks_A, A->numberOfBlocks, _BTAG );
	_Wait(_BTAG);

	vector float *BKJ, *BIJ, aik;
	float *AIK;

	unsigned int x;

	int ain, ain2;

	_DBGP_UINT( 4000 );
	_DBGP_UINT( CB );

	// Same A, get it once
	_GetBlock( blocks_A, info->LS_blockDataArea[1], CB*A->numberOfBlocksXDim+CB, A->blockSize*A->blockSize, idx );


	_Wait(idx);
	_DBGP_UINT( CB*B->numberOfBlocksXDim+1+CB+SPE_id );

	// Get first block of B
	_GetBlock( blocks_B, info->LS_blockDataArea[0], CB*B->numberOfBlocksXDim+1+CB+SPE_id, B->blockSize*B->blockSize, idx );

	_Wait(idx);
	_DBGP_UINT( 5000 );

	//funcs->printuint(CB + SPE_id + 1);
	//funcs->printuint(B->numberOfBlocksXDim-myop->num_SPES);

	// For each block column
	// BLOCK >
	for( col = CB + SPE_id + 1 ; col < B->numberOfBlocksXDim-myop->num_SPES ; col += myop->num_SPES )
	{
		_DBGP_UINT( 100000 + col );

		nextidx = idx ^ 1;

		//funcs->printuint(CB*A->numberOfBlocksXDim+col+myop->num_SPES);
		_GetBlock( blocks_B, info->LS_blockDataArea[nextidx*3], CB*B->numberOfBlocksXDim+col+myop->num_SPES, B->blockSize*B->blockSize, nextidx );

		_Wait( idx );

		for( l = 1 ; l < B->blockSize ; l++ )
		{
			_DBGP_UINT( 200000 + l );

			BKJ = (vector float*)( info->LS_blockDataArea[idx*3] + (l-1) % B->blockSize * B->blockSize * 4 );

			// BIJ[i] = BIJ[i] - BKJ[i] * aik;
			_compute( B->blockSize, l, l-1, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[1] );
			//_compute2( B->blockSize, l, l-1, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[1], funcs );
		}


		// Return the result to the PPE
		_PutAsync( (unsigned int)blocks_B[CB*A->numberOfBlocksXDim+col], B->blockSize * B->blockSize * 4, info->LS_blockDataArea[idx*3], idx );

		idx = nextidx;
	}

	_DBGP_UINT( 100000 + col );

	_Wait( idx );

	for( l = 1 ; l < B->blockSize ; l++ )
	{
		BKJ = (vector float*)( info->LS_blockDataArea[idx*3] + (l-1) % B->blockSize * B->blockSize * 4 );
		//_compute2( B->blockSize, l, l-1, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[1], funcs );
		_compute( B->blockSize, l, l-1, BKJ, (float*)info->LS_blockDataArea[idx*3], (float*)info->LS_blockDataArea[1] );
	}

	//funcs->printuint( 678 );
	//_PrintMatrix( B, info->LS_blockDataArea[idx*3], funcs );


	// Return the result to the PPE
	_PutAsync( (unsigned int)blocks_B[CB*A->numberOfBlocksXDim+col], B->blockSize * B->blockSize * 4, info->LS_blockDataArea[idx*3], idx );

}





