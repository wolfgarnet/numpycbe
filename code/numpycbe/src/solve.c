#include <common.h>
#include <common_ppu.h>

#include <fhbtool.h>
#include <timer.h>

#include <malloc_align.h>

#include <shaders.h>
//#include "../numpycbe/include/shaders.h"


#include "solve.h"

// Timers
TimerObject *t_max, *t_scal, *t_swap, *t_sger, *t_sgetf2, *t_strsm1, *t_strsm2, *t_strsm3, *t_sgetrf, *t_sgetrs, *t_sgemm, *t_sgesv;

#define _NUMBLOCKS 1

/*
 * SGESV
 */


void sgesv( PyArrayObject *A, PyArrayObject *B, unsigned int *info )
{
	Print1( "Running SGESV\n" );

	t_max = GetTimer();
	t_scal = GetTimer();
	t_swap = GetTimer();
	t_sger = GetTimer();
	t_sgetf2 = GetTimer();
	t_sgetrf = GetTimer();
	t_sgetrs = GetTimer();
	t_strsm1 = GetTimer();
	t_strsm2 = GetTimer();
	t_strsm3 = GetTimer();
	t_sgesv = GetTimer();
	t_sgemm = GetTimer();

	Print1( "Timers initialized\n" );

	_StartTimer( t_sgesv );

	Print1( "SGESV timer started\n" );

	/*
	 * CBE SPECIFICS
	 *
	 * Setting up LU factization shaders
	 */

	unsigned int state = 200;
	unsigned int i;

	Solve_t solve;
	Operation_t op_sger;
	Operation_t op_strsm_llnu_lu;
	Operation_t op_strsm_llnu;
	Operation_t op_sgemm;
	Operation_t op_strsm_lunn;

	solve.sger          = &op_sger;
	solve.strsm_llnu_lu = &op_strsm_llnu_lu;
	solve.strsm_llnu    = &op_strsm_llnu;
	solve.sgemm         = &op_sgemm;
	solve.strsm_lunn    = &op_strsm_lunn;

	Printf1( "A: bs=%u, dim[0]=%u\n", A->blockSize, A->dimensions[0] );
	// Make temporary objects
	PyArrayObject tmp;
	PyArrayObject J;
	PyArrayObject alpha;


	Print1( "Making temporary objects\n" );
	sMakeFHB( A->blockSize, A->dimensions[0], A->blockSize, 1.0f, &tmp );
	uMakeFHB( 1, 1, A->blockSize, 1, &J );
	sMakeFHB( 1, 1, A->blockSize, -1.0f, &alpha );

 	// Sgemm
	Print1( "Making temporary SGEMM objects\n" );
	PyArrayObject rowmatrix, colmatrix, squarematrix;
	InitMultVectors( &rowmatrix, &colmatrix, &squarematrix, A->numberOfBlocksXDim, A->numberOfBlocksYDim, A->blockSize );

	Print1( "Made temporary objects\n" );

	/*
	 * The SGER operation
	 *
	 * Shader slot 0
	 */

	op_sger.shaderSize = sger_lu_size;
	op_sger.EA_shader  = SHADER_sger_lu;
	op_sger.num_SPES   = 1;
	op_sger.obj[0]     = A;
	op_sger.obj[1]     = &tmp;
	op_sger.scalar[0]  = &alpha;
	op_sger.scalar[1]  = &J;



	// Send the SPEs the new state

	Print1( "Signaling sger\n" );

  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op_sger;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	/*
  	 * The STRSM left lower non-trasnposed unit operation, for LU
  	 *
  	 * Shader slot 1
  	 */

	op_strsm_llnu_lu.shaderSize = strsm_llnu_lu_size;
	op_strsm_llnu_lu.EA_shader  = SHADER_strsm_llnu_lu;
	op_strsm_llnu_lu.num_SPES   = 1;
	op_strsm_llnu_lu.obj[0]     = A;
	op_strsm_llnu_lu.obj[1]     = A;
	op_strsm_llnu_lu.scalar[0]  = &alpha;
	op_strsm_llnu_lu.scalar[1]  = &J;


	Print1( "Signaling strsm llnu lu\n" );

	// Send the SPEs the new state
	state = 201;
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][1] = (unsigned int)&op_strsm_llnu_lu;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

 	/*
  	 * The SGEMM
  	 *
  	 * Shader slot 2
  	 */

  	op_sgemm.shaderSize = sgemm_fixed_size;
  	op_sgemm.EA_shader  = SHADER_sgemm_fixed;
  	op_sgemm.num_SPES   = 6;
  	//op_sgemm.num_SPES   = 1;
  	op_sgemm.obj[0]     = &colmatrix;
  	op_sgemm.obj[1]     = &rowmatrix;
  	op_sgemm.obj[2]     = &squarematrix;

	solve.sgemm = &op_sgemm;

	Print1( "Signaling sgemm\n" );

	// Send the SPEs the new state
	state = 203;
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][3] = (unsigned int)&op_sgemm;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	/*
  	 * CODE
  	 */

	// IPIV
	unsigned int *ipiv = (unsigned int*)_malloc_align( A->dimensions[0] * 4, 4 );

	/*
	 * Call LU factorization SGETRF
	 */
	Print1( "Calling sgetrf\n" );
	_StartTimer( t_sgetrf );
	Print1( "TIMER\n" );
	sgetrf( A, ipiv, info, &solve );
	_StopTimer( t_sgetrf );


	//return;


//	// Update operations
//	op_strsm_llnu.num_SPES = 6;
//	state = 122;
//  	for ( i = 0 ; i < speThreads ; i++ )
//	{
//  		spe_pointer_addr[i][2] = (unsigned int)&op_strsm_llnu;
//  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
//	}

	/*
	 * CBE SPECIFICS
	 *
	 * Setting up solve shaders
	 */


  	/*
  	 * The STRSM left lower non-trasnposed unit operation
  	 *
  	 * Shader slot 0
  	 */

	//FHBFillScalarUInt( &J, 999999 );

	op_strsm_llnu.shaderSize = strsm_llnu_size;
	op_strsm_llnu.EA_shader  = SHADER_strsm_llnu;
	op_strsm_llnu.num_SPES   = 1;
	//op_strsm_llnu.num_SPES   = 6;
	op_strsm_llnu.obj[0]     = A;
	op_strsm_llnu.obj[1]     = B;
	op_strsm_llnu.scalar[0]  = &alpha;
	op_strsm_llnu.scalar[1]  = &J;


	Print1( "Signaling strsm llnu\n" );

	// Send the SPEs the new state
	state = 200;
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op_strsm_llnu;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	/*
  	 * The STRSM left upper non-trasnposed non-unit operation
  	 *
  	 * Shader slot 1
  	 */

	op_strsm_lunn.shaderSize = strsm_lunn_size;
	op_strsm_lunn.EA_shader  = SHADER_strsm_lunn;
	//op_strsm_lunn.num_SPES   = 6;
	op_strsm_lunn.num_SPES   = 1;
	op_strsm_lunn.obj[0]     = A;
	op_strsm_lunn.obj[1]     = B;
	op_strsm_lunn.scalar[0]  = &alpha;

	Print1( "Signaling strsm lunn\n" );

	// Send the SPEs the new state
	state = 201;
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][1] = (unsigned int)&op_strsm_lunn;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


//  	// Sanity check
//  	state = 1000;
//  	for ( i = 0 ; i < speThreads ; i++ )
//	{
//  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
//	}

//	// Waiting for SPEs!
//	unsigned int checked = 0, r;
//	while( checked < speThreads )
//	{
//		//printf( "Checked=%u\n", checked );
//		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
//		{
//			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
//			checked++;
//		}
//	}

  	//return;

	/*
	 * Solve
	 */
	Print1( "Calling sgetrs\n" );
	_StartTimer( t_sgetrs );
	sgetrs( A, B, ipiv, info, &solve );
	_StopTimer( t_sgetrs );


  	/*
  	 * The end
  	 */


	DeleteObject( &tmp );
	DeleteObject( &J );


	_StopTimer( t_sgesv );

#ifdef _DEBUG_TIME

	// TimerObject *t_max, *t_scal, *t_swap, *t_sger, *t_sgetf2, *t_strsm1, *t_strsm2, *t_strsm3, *t_sgetrf, *t_sgetrs;
	PrintTimer( "Total time for SGESV: ", t_sgesv );
	PrintTimer( "Total time for SGETRF: ", t_sgetrf );
	PrintTimer( "Total time for SGETRS: ", t_sgetrs );
	PrintTimer( "Total time for SGETF2: ", t_sgetf2 );

	printf( "Sgetrf(unblocked):\n" );
	PrintTimer( "Total time for isamax: ", t_max );
	PrintTimer( "Total time for sscal: ", t_scal );
	PrintTimer( "Total time for sswap: ", t_swap );
	PrintTimer( "Total time for sger: ", t_sger );

	printf( "Sgetrf(blocked):\n" );
	PrintTimer( "Total time for strsm llnu lu: ", t_strsm1 );
	PrintTimer( "Total time for sgemm: ", t_sgemm );

	printf( "Sgetrs:\n" );
	PrintTimer( "Total time for strsm llnu: ", t_strsm2 );
	PrintTimer( "Total time for strsm lunn: ", t_strsm3 );


#endif
}


/*
 * ISAMAX
 */


/*
 * Find the index for the largest element in a row
 */

unsigned int FHB_isamax_row( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A )
{
	Print1( "Running ISAMAX_row\n" );

	unsigned int brow = row / A->blockSize * A->numberOfBlocksXDim;
	unsigned int r    = row % A->blockSize * A->blockSize;
	unsigned int b;
	unsigned int i;
	unsigned int idx = 0;
	float max;

	b = brow + col / A->blockSize;
	max = A->blockData[b][r+col%A->blockSize];

	for( i = col+1 ; i < col + offset ; i++ )
	{
		b = brow + i / A->blockSize;
		if ( A->blockData[b][r+i%A->blockSize] > max )
		{
			max = A->blockData[b][r+i%A->blockSize];
			idx = i;
		}
	}

	return idx;
}

/*
 * Find the index for the largest element in a col
 */

unsigned int FHB_isamax_col( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A )
{
	Print1( "Running ISAMAX_col\n" );

	unsigned int c    = col % A->blockSize;
	unsigned int bcol = col / A->blockSize;
	unsigned int i;
	unsigned int idx = row;
	float max;

	unsigned int b = row / A->blockSize * A->numberOfBlocksXDim + bcol;
	max = ((float*)A->blockData[b])[row%A->blockSize*A->blockSize+c];

	//printf( "i=%u, ", row );

	for( i = row+1 ; i < row + offset ; i++ )
	{
		//printf( "i=%u, ", i );
		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;
		if ( ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] > max )
		{
			max = ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c];
			idx = i;
		}
	}
	//printf( ". i=%u\n", idx );

	return idx;
}


unsigned int FHB_isamax_col2( unsigned int col, unsigned int row, PyArrayObject *A )
{
	Print1( "Running ISAMAX_col2\n" );

	unsigned int c    = col % A->blockSize;
	unsigned int bcol = col / A->blockSize;
	unsigned int i;
	unsigned int idx = row, snade;
	float max, t;

	unsigned int b = row / A->blockSize * A->numberOfBlocksXDim + bcol;

	//Printf1( "BLOCKSIZE=%u\n", A->blockSize );
	//Printf1( "DIM[0]=%u, DIM[1]=%u\n", A->dimensions[0], A->dimensions[1] );


//	t = ((float*)A->blockData[0])[0];
//	Printf1( "T=%f\n", t );



	max = fabs(((float*)A->blockData[b])[row%A->blockSize*A->blockSize+c]);

	//snade = row%A->blockSize*A->blockSize+c;

	//Printf1( "Row=%u, col=%u, b=%u, index=%u\n", row, col, b, snade );


	//max = ((float*)A->blockData[b])[snade];



	Printf1( "MAX(%u)=%e, ", row, max );

	for( i = row+1 ; i < A->dimensions[0] ; i++ )
	{
		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;

		t = fabs(((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c]);
		Printf1( "i=%u(%e)", i, t );


		//if ( max <= ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] )
		if ( max < t )
		{
			max = t;
			idx = i;
			//Printf1( "i=%u(%f), ", row, max );
			Print1( "MAX" );
		}
		Print1( ", " );
	}
	Printf1( "\nMAX(%u) = %e\n", idx, max );

	return idx;
}


/*
 * SGETF2
 */


#define _SPEFACTOR 64


//void sgetf2_blocked( PyArrayObject *A, unsigned int jb, unsigned int mb, unsigned int nb, unsigned int *ipiv, unsigned int *info )
void sgetf2_blocked( PyArrayObject *A, unsigned int jb, unsigned int mb, unsigned int nb, unsigned int *ipiv, unsigned int *info, /* CELL */ Solve_t *solve )
{
	Print1( "----------------\nSGETF2 BLOCKED\n----------------\n" );

	unsigned int bidx, idx, bidx2, idx2;
	unsigned int br;

	unsigned int mindim = mb;
	if ( nb < mindim )
	{
		mindim = nb;
	}

	unsigned int i, j_, j, jp, state, checked, r;

  	Printf1( "MIN DIM=%u\n", mindim );

	//for( j = jj ; j < jj+mindim ; j++ )
  	//for( j = jb * A->blockSize ; j < (jb+1)*A->blockSize ; j++ )
  	unsigned int joffset = jb * A->blockSize;
  	Printf1( "JOFFSET=%u\n", joffset );
  	for( j_ = jb ; j_ < jb + _NUMBLOCKS ; j_++ )
	{
  		Printf1( "Block %u\n", j_ );
  		for( j = 0 ; j < A->blockSize ; j++ )
		{
  			//PrintNDArray( A, 3 );
  			Print1( "----------------|||||-----------------\n" );
  			Printf1( "j= %u\n", j );
  			//PrintNDArray( A, 1 );

  			//counter++;

  			Print2( "MAX\n" );
  			_StartTimer( t_max );
			jp = FHB_isamax_col2( j+joffset, j+joffset, A );
			_StopTimer( t_max );
			Printf2( "END OF MAX, jp=%u\n", jp );
			ipiv[j+joffset] = jp;

			bidx = getblockidx( jp, (j+joffset), A->numberOfBlocksXDim, A->blockSize );
			idx  = getidx( jp, (j+joffset), A->blockSize );
			if( ((float*)A->blockData[bidx])[idx] != 0.0f )
			{

				// Apply interchange to columns 1:N
				if( jp != j+joffset )
				{
					_StartTimer( t_swap );
					Printf2( "SWAP(%u, %u) j=%u, jb=%u!\n", j+joffset, jp, j, jb );
					FHB_sswap_row_blocked( A, jb, j+joffset, jp );
					Print2( "END OF SWAP!\n" );
					_StopTimer( t_swap );

					//PrintNDArray( A, 1 );
				}

				// Compute the elements j+1:m of the j'th column
				//if( j < A->blockSize * A->numberOfBlocksYDim - 1 - jb * A->blockSize )
				if( j < mb )
				{
					_StartTimer( t_scal );
					Print2( "SCAL\n" );
					//printf( "In2\n" );
					bidx = getblockidx( (j+joffset), (j+joffset), A->numberOfBlocksXDim, A->blockSize );
					idx  = getidx( j+joffset, j+joffset, A->blockSize );
					// FHB_sscal_col( unsigned int offset, unsigned int col, unsigned int row, float scale, PyArrayObject *A );
					//Tic();
					//printf( "Scaling with %f, from %u, %u\n", ((float*)A->blockData[bidx])[idx], bidx, idx );
					//PrintNDArray( A, 1 );
					FHB_sscal_col_blocked( j+joffset, j+joffset+1, 1/((float*)A->blockData[bidx])[idx], A );
					//PrintTicToc( "SSCAL: ", Toc() );
					Print2( "END OF SCAL\n" );
					_StopTimer( t_scal );

					//PrintTimer( "Scal_m: ", t_scal );

					//PrintNDArray( A, 1 );
				}

			}
			else
			{
				//printf( "In3\n" );
				*info = j;
			}
			//PrintTicToc( "BLOCK: ", Toc() );

			if( j < mindim-1 )
			{
				Print1( "Updating trailing sub matrix\n" );
				//printf( "--->-_>_<-<-<-->-_<->-<\n" );
				_StartTimer( t_sger );
				//PrintMatrix2( A, 1 );
				// Update trailing submatrix

				// Make x(a column vector)
				MakeVectorFromMatrixCol( A, solve->sger->obj[1], j+joffset );

				// Fill J
				FHBFillScalarUInt( solve->sger->scalar[1], j+joffset );

				//printf( "----->(%u)\b", j );
				// How many block rows left?
				br = ( A->numberOfBlocksYDim - jb );
				if( br < 6*_SPEFACTOR )
				{
					solve->sger->num_SPES = br/_SPEFACTOR+1;
				}
				else
				{
					solve->sger->num_SPES = 6;
				}

				solve->sger->num_SPES = 1;

				Printf2( "Using %u SPEs. j=%u\n", solve->sger->num_SPES, j+joffset );

				//printf( "Number of SPEs: %u, blocks left: %u\n", solve->sger->num_SPES, br );


				// Call shader!!!

				state = 300;
				int e;
				for ( i = 0 ; i < solve->sger->num_SPES ; i++ )
				{
					spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
				}

				//printf( "Shader called!\n" );

				// Waiting for SPEs!
				checked = 0;
				while( checked < solve->sger->num_SPES )
				{
					//printf( "Checked=%u\n", checked );
					if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
					{
						spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
						checked++;
					}
				}


				Print1( "Finsihed updating trailing sub matrix\n" );
				//PrintMatrix2( A, 1 );
				_StopTimer( t_sger );
			}

			//PrintNDArray( A, 1 );
		}
	}
}



/*
 * SGETRF
 */


void sgetrf( PyArrayObject *A, unsigned int *ipiv, unsigned int *info, Solve_t *solve )
{
	Print1( "running SGETRF\n" );

	unsigned int j, jb = _NUMBLOCKS, state, i, checked, r;
	//unsigned int m = A->dimensions[0], n = A->dimensions[1];






  	// Strsm

	for( j = 0 ; j < A->numberOfBlocksYDim ; j++ )
	{
		Printf1( "SGETFT: J=%u\n", j );

		_StartTimer( t_sgetf2 );
		// Factor diagonal and subdiagonal blocks and test for exact singularity
		//printf( "j=%u(%u)\n", j, (A->numberOfBlocksYDim-j) );
		sgetf2_blocked( A, j, A->blockSize * (A->numberOfBlocksYDim-j), A->blockSize, ipiv, info, solve );
		_StopTimer( t_sgetf2 );

//		printf( "IPIV=" );
//		for( i = 0 ; i < A->dimensions[0] ; i++ )
//		{
//			printf( "%u, ", ipiv[i] );
//		}
//		printf( "\n" );

		//PrintMatrix2( A, 1 );

		// Apply changesto columns 1:n

		// slaswp();
		// void slaswp( unsigned int n, PyArrayObject *A, unsigned int k1, unsigned int k2, unsigned int *ipiv );
		//slaswp( A->blockSize * j, A, j*A->blockSize, (j+1)*A->blockSize-1, ipiv );
		// d( unsigned int nb, PyArrayObject *A, unsigned int offsetb, unsigned int yb, unsigned int *ipiv );
		if( j > 0 )
		{
			Print1( "Calling SLASWP!\n" );
			slaswp_blocked( j, A, 0, j, ipiv );
		}

		if( j + jb < A->numberOfBlocksXDim )
		{
			// Apply interchanges to columns 1:j
			// slaswp
			//slaswp( A->blockSize * ( A->numberOfBlocksXDim - j ), A, j*A->blockSize, (j+1)*A->blockSize-1, ipiv );
			// d( unsigned int nb, PyArrayObject *A, unsigned int offsetb, unsigned int yb, unsigned int *ipiv );
			slaswp_blocked( ( A->numberOfBlocksXDim - j - 1 ), A, j+1, j, ipiv );

			// Compute block row of U
			// strsm

			Print1( "Compute block row of U\n" );

			//PrintNDArray( A, 1 );

			_StartTimer( t_strsm1 );

			// Fill J
			Printf1( "Filling J with %u\n", j );
			FHBFillScalarUInt( solve->strsm_llnu_lu->scalar[1], j );

			// Call shader!!!

			state = 301;

			if( A->numberOfBlocksXDim - j - 1 > 6 )
			{
				solve->strsm_llnu_lu->num_SPES = 6;
			}
			else
			{
				solve->strsm_llnu_lu->num_SPES = A->numberOfBlocksXDim - j - 1;
			}

			solve->strsm_llnu_lu->num_SPES = 1;

			Printf1( "Using %u SPEs\n", solve->strsm_llnu_lu->num_SPES );

			for ( i = 0 ; i < solve->strsm_llnu_lu->num_SPES ; i++ )
			{
				spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
			}

			//printf( "Shader called!\n" );

			// Waiting for SPEs!
			checked = 0;
			while( checked < solve->strsm_llnu_lu->num_SPES )
			{
				//printf( "Checked=%u\n", checked );
				if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
				{
					spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
					checked++;
				}
			}

			//PrintNDArray( A, 1 );


			Print1( "Finshed computing block row of U\n" );
			_StopTimer( t_strsm1 );
			//PrintTimer( "Strsm:", t_strsm1 );




			if( j + jb < A->numberOfBlocksYDim )
			{
				// Update trailing sub matrix
				// sgemm


				//printf( "Calling SGEMM, before:\n" );
				//PrintMatrix2( A, 2 );

				Print1( "Update trailing sub matrix\n" );

				_StartTimer( t_sgemm );

				CreateTempArrays( solve->sgemm->obj[1], solve->sgemm->obj[0], solve->sgemm->obj[2], A, j+1 );


				//printf( "Blocksize=%u, xdim=%u, ydim=%u, nb=%u\n", solve->sgemm->obj[0]->blockSize, solve->sgemm->obj[0]->numberOfBlocksXDim, solve->sgemm->obj[0]->numberOfBlocksYDim, solve->sgemm->obj[0]->numberOfBlocks );

//				PrintNDArray( solve->sgemm->obj[0], 1 );
//				PrintNDArray( solve->sgemm->obj[1], 1 );
//				PrintNDArray( solve->sgemm->obj[2], 1 );

				// Call shader!!!

				state = 303;

				if( A->numberOfBlocksXDim - j - 1 > 6 )
				{
					solve->sgemm->num_SPES = 6;
				}
				else
				{
					solve->sgemm->num_SPES = A->numberOfBlocksXDim - j - 1;
				}

				solve->sgemm->num_SPES = 1;

				Printf1( "Using %u SPEs\n", solve->sgemm->num_SPES );

				for ( i = 0 ; i < solve->sgemm->num_SPES ; i++ )
				{
					spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
				}

				//printf( "Shader called!\n" );

				// Waiting for SPEs!
				checked = 0;
				while( checked < solve->sgemm->num_SPES )
				{
					//printf( "Checked=%u\n", checked );
					if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
					{
						spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
						checked++;
					}
				}

				//PrintNDArray( A, 1 );

				Print1( "Finished updating trailing sub matrix\n" );
				_StopTimer( t_sgemm );
				//PrintTimer( "Strsm:", t_strsm1 );

				//printf( "Final SGEMM:\n" );
				//PrintNDArray( A, 2 );

			}
		}
	}

}


/*
 * SGETRS
 */



void sgetrs( PyArrayObject *A, PyArrayObject *B, unsigned int *ipiv, unsigned int *info, Solve_t *solve )
{
	//printf( "Running SGETRS\n" );

	unsigned int i, checked, r, state;

	// Apply row interchanges to the right hand side
	//slaswp_blocked( B->numberOfBlocksYDim, B, 0, 0, ipiv );

	/*
	 * Solve L * X = B, overwriting B with X
	 */
	Print1( "Solve L * X = B\n" );
	_StartTimer( t_strsm2 );

	// Fill J
	//FHBFillScalarUInt( solve->strsm_llnu->scalar[1], A->numberOfBlocksYDim+1 );
	FHBFillScalarUInt( solve->strsm_llnu->scalar[1], A->dimensions[0]+1 );

	//printf( "Printing A and B\n" );
	//PrintNDArray( A, 1 );
	//PrintNDArray( B, 1 );

	// Call shader!!!

	//solve->strsm_llnu->num_SPES = 6;

	state = 300;

	//printf( "Using %u SPEs\n", solve->strsm_llnu->num_SPES );

	for ( i = 0 ; i < solve->strsm_llnu->num_SPES ; i++ )
	{
		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

	//printf( "Shader called!\n" );

	// Waiting for SPEs!
	checked = 0;
	while( checked < solve->strsm_llnu->num_SPES )
	{
		//bprintf( "Checked=%u\n", checked );
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}


	Print1( "Done solving L * X = B\n" );

	//printf( "Printing B\n" );
	//PrintNDArray( B, 1 );

	_StopTimer( t_strsm2 );

	/*
	 * Solve U * X = B, overwriting B with X
	 */
	Print1( "Solve U * X = B\n" );
	_StartTimer( t_strsm3 );

	// Call shader!!!

	state = 301;

	for ( i = 0 ; i < solve->strsm_lunn->num_SPES ; i++ )
	{
		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

	//printf( "Shader called!\n" );

	// Waiting for SPEs!
	checked = 0;
	while( checked < solve->strsm_lunn->num_SPES )
	{
		//printf( "Checked=%u\n", checked );
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}

	Print1( "Done solving U * X = B\n" );

	_StopTimer( t_strsm3 );

	//printf( "Printing B\n" );
	//PrintNDArray( B, 1 );
}

/*
 * SLASWP
 */


//void slaswp( unsigned int n, PyArrayObject *A, unsigned int k1, unsigned int k2, unsigned int offset, unsigned int *ipiv );
//void slaswp( unsigned int n, PyArrayObject *A, unsigned int k1, unsigned int k2, unsigned int *ipiv )
//{
//	unsigned int i, j, k, ip;
//	unsigned int k1x = k1;
//
//	float temp;
//
//	Printf2( "SLASWP!!!\nk1=%u, k2=%u\n", k1, k2 );
//
//	unsigned int brow1, brow2, r1, r2, maxb = n / A->blockSize;
//	if( n % A->blockSize != 0 )
//	{
//		maxb++;
//	}
//
//	for( i = k1 ; i < k2+1 ; i++ )
//	{
//		Printf2( "I=%u\n", i );
//		ip = ipiv[k1x];
//
//		Printf2( "ip=%u, k1x=%u\n", ip, k1x );
//
//		brow1 = ip  / A->blockSize * A->numberOfBlocksXDim;
//		brow2 = k1x / A->blockSize * A->numberOfBlocksXDim;
//		r1    = ip  % A->blockSize * A->blockSize;
//		r2    = k1x % A->blockSize * A->blockSize;
//
//		// Exchange row k1x with ip
//		for( j = 0 ; j < maxb-1 ; j++ )
//		{
//			//printf( "J=%u\n", j );
//			// One row in a block
//			for( k = 0 ; k < A->blockSize ; k++ )
//			{
////				printf( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
////				printf( "brow1=%u, brow2=%u, r1=%u, r2=%u\n", brow1, brow2, r1, r2 );
//
//				temp = ((float*)A->blockData[brow1])[r1+k];
//				((float*)A->blockData[brow1])[r1+k] = ((float*)A->blockData[brow2])[r2+k];
//				((float*)A->blockData[brow2])[r2+k] = temp;
//
//				Printf2( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
//			}
//
//			brow1++;
//			brow2++;
//		}
//
//		// Last block
//		for( k = 0 ; k < n % A->blockSize ; k++ )
//		{
////				printf( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
////				printf( "brow1=%u, brow2=%u, r1=%u, r2=%u\n", brow1, brow2, r1, r2 );
//
//			temp = ((float*)A->blockData[brow1])[r1+k];
//			((float*)A->blockData[brow1])[r1+k] = ((float*)A->blockData[brow2])[r2+k];
//			((float*)A->blockData[brow2])[r2+k] = temp;
//
//			Printf2( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
//		}
//
//		k1x++;
//	}
//}

void slaswp_blocked( unsigned int nb, PyArrayObject *A, unsigned int offsetb, unsigned int yb, unsigned int *ipiv )
{
	Printf1( "Running SLASWP, offset at %u\n", offsetb );

	unsigned int brow1 = yb*A->numberOfBlocksXDim+offsetb, brow2;
	unsigned int r1, r2;

	unsigned int i, j, k, ip;

	float temp;

	// For each row in the block
	for( i = yb*A->blockSize ; i < (yb+1)*A->blockSize ; i++ )
	{

		ip = ipiv[i];

		Printf2( "I=%u, ip=%u\n", i, ip );

		r1 = ( i % A->blockSize ) * A->blockSize;
		r2 = ( ip % A->blockSize ) * A->blockSize;

		brow2 = ip / A->blockSize * A->numberOfBlocksXDim + offsetb;

		Printf1( "Exchanging %u and %u\n", i, ip );

		// For each block row
		for( j = 0 ; j < nb ; j++ )
		{
			//Printf2( "J=%u\n", j );
			for( k = 0 ; k < A->blockSize ; k++ )
			{
//				Printf2( "ROW1: b=%u, c=%u, val=%f\n", brow1+j, r1+k, ((float*)A->blockData[brow1+j])[r1+k] );
//				Printf2( "ROW1: b=%u, c=%u, val=%f\n", brow2+j, r2+k, ((float*)A->blockData[brow2+j])[r2+k] );
				temp = ((float*)A->blockData[brow1+j])[r1+k];
				((float*)A->blockData[brow1+j])[r1+k] = ((float*)A->blockData[brow2+j])[r2+k];
				((float*)A->blockData[brow2+j])[r2+k] = temp;
			}
		}
	}
}


/*
 * SSCAL
 */


void FHB_sscal_col( unsigned int offset, unsigned int col, unsigned int row, float scale, PyArrayObject *A )
{
	Print1( "Running SSCAL_col\n" );

	unsigned int c    = col % A->blockSize;
	unsigned int bcol = col / A->blockSize;
	unsigned int i, b;

	for( i = row ; i < row + offset ; i++ )
	{
		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;
		((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] *= scale;

	}
}

void FHB_sscal_col_blocked( unsigned int col, unsigned int row, float scale, PyArrayObject *A )
{
	Print1( "Running SSCAL_col_blocked\n" );

	unsigned int c    = col % A->blockSize;
	unsigned int bcol = col / A->blockSize;
	unsigned int i, b;

	for( i = row ; i < A->dimensions[0] ; i++ )
	{
		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;
		((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] *= scale;

	}
}


/*
 * SSWAP
 */


void FHB_sswap_row( PyArrayObject *A, unsigned int row1, unsigned int row2 )
{
	Print1( "Running SSWAP_row\n" );

	unsigned int j, k;

	float temp;

	unsigned int brow1 = row1 / A->blockSize * A->numberOfBlocksXDim;
	unsigned int brow2 = row2 / A->blockSize * A->numberOfBlocksXDim;
	unsigned int r1    = row1 % A->blockSize * A->blockSize;
	unsigned int r2    = row2 % A->blockSize * A->blockSize;

	// Exchange row k1x with ip
	for( j = 0 ; j < A->numberOfBlocksXDim ; j++ )
	{
		for( k = 0 ; k < A->blockSize ; k++ )
		{
			temp = ((float*)A->blockData[brow1])[r1+k];
			((float*)A->blockData[brow1])[r1+k] = ((float*)A->blockData[brow2])[r2+k];
			((float*)A->blockData[brow2])[r2+k] = temp;

			//printf( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
		}

		brow1++;
		brow2++;
	}

}

void FHB_sswap_row_blocked( PyArrayObject *A, unsigned int blockcol, unsigned int row1, unsigned int row2 )
{
	Print1( "Running SSWAP_row_blocked\n" );

	unsigned int j = blockcol, k;

	float temp;

	unsigned int brow1 = row1 / A->blockSize * A->numberOfBlocksXDim;
	unsigned int brow2 = row2 / A->blockSize * A->numberOfBlocksXDim;
	unsigned int r1    = row1 % A->blockSize * A->blockSize;
	unsigned int r2    = row2 % A->blockSize * A->blockSize;

//	for( j = 0 ; j < A->numberOfBlocksXDim ; j++ )
//	{
		for( k = 0 ; k < A->blockSize ; k++ )
		{
			temp = ((float*)A->blockData[brow1+j])[r1+k];
			((float*)A->blockData[brow1+j])[r1+k] = ((float*)A->blockData[brow2+j])[r2+k];
			((float*)A->blockData[brow2+j])[r2+k] = temp;

			//printf( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
		}
//	}


}

void CreateTempArrays( PyArrayObject *rowmatrix, PyArrayObject *colmatrix, PyArrayObject *squarematrix, PyArrayObject *arg1, unsigned int d )
{
	//Set rowmatrix/argument 1 til blas3Mult
	rowmatrix->numberOfBlocks     = arg1->numberOfBlocksXDim - d;
	rowmatrix->numberOfBlocksXDim = arg1->numberOfBlocksXDim - d;
	rowmatrix->numberOfBlocksYDim = 1;
	rowmatrix->blockSize          = arg1->blockSize;
	rowmatrix->nd                 = arg1->nd;
	rowmatrix->size_in_bytes      = arg1->size_in_bytes;

	rowmatrix->paddingx           = arg1->paddingx;
	rowmatrix->paddingy           = arg1->paddingy;

	unsigned int i;
	unsigned int startIndex = arg1->numberOfBlocksXDim * ( d - 1 ) + d;
	Print1( "ROWMATRIX blocks: " );
	for( i = 0 ; i < ( arg1->numberOfBlocksXDim - d ) ; i++ )
	{
		Printf1( "%u, ", startIndex+i );
		rowmatrix->blockData[i] = arg1->blockData[startIndex+i];
	}
	Print1( "\n" );

	//Set colmatrix/argument 2 til blas3Mult
	colmatrix->numberOfBlocks     = arg1->numberOfBlocksYDim-d;
	colmatrix->numberOfBlocksXDim = 1;
	colmatrix->numberOfBlocksYDim = arg1->numberOfBlocksYDim-d;
	colmatrix->blockSize          = arg1->blockSize;
	colmatrix->nd                 = arg1->nd;

	colmatrix->size_in_bytes      = arg1->size_in_bytes;

	colmatrix->paddingx           = arg1->paddingx;
	colmatrix->paddingy           = arg1->paddingy;

	Print1( "COLMATRIX blocks: " );
	startIndex = (arg1->numberOfBlocksXDim*d+d-1);
	for( i = 0 ; i < ( arg1->numberOfBlocksYDim - d ) ; i++ )
	{
		Printf1( "%u, ", startIndex+(i*arg1->numberOfBlocksXDim) );
		colmatrix->blockData[i] = arg1->blockData[startIndex+(i*arg1->numberOfBlocksXDim)];
	}
	Print1( "\n" );

	//Set squarematrix/argument 3 til blas3Mult
	squarematrix->numberOfBlocks     = (arg1->numberOfBlocksYDim-d) * (arg1->numberOfBlocksXDim-d);
	squarematrix->numberOfBlocksXDim = arg1->numberOfBlocksXDim-d;
	squarematrix->numberOfBlocksYDim = arg1->numberOfBlocksYDim-d;
	squarematrix->blockSize          = arg1->blockSize;
	squarematrix->nd                 = arg1->nd;

	squarematrix->size_in_bytes      = arg1->size_in_bytes;
	squarematrix->paddingx           = arg1->paddingx;
	squarematrix->paddingy           = arg1->paddingy;

	unsigned int u;

	unsigned int resultIndex = 0;

	startIndex = arg1->numberOfBlocksXDim*d+d;

	for( i = 0 ; i < ( arg1->numberOfBlocksYDim - d ) ; i++ )
	{
		for( u = 0 ; u < ( arg1->numberOfBlocksXDim - d ) ; u++ )
		{
			squarematrix->blockData[resultIndex] = arg1->blockData[startIndex+u];
			resultIndex++;
		}
		startIndex=startIndex+arg1->numberOfBlocksXDim;
	}
}

void InitMultVectors( PyArrayObject *rowmatrix, PyArrayObject *colmatrix, PyArrayObject *squarematrix, unsigned int numBlocksX, unsigned int numBlocksY, unsigned int _blockSize )
{
	//Setting the PyArrayObject
	rowmatrix->numberOfBlocks     = numBlocksX * numBlocksY;
	rowmatrix->numberOfBlocksXDim = numBlocksX;
	rowmatrix->numberOfBlocksYDim = numBlocksY;
	rowmatrix->blockSize          = _blockSize;//in elements
	rowmatrix->nd                 = 1;
	rowmatrix->blockData          = (char**)_malloc_align(sizeof(char*)*numBlocksX*numBlocksY,7);
	unsigned int i = 0;
	for( i = 0 ; i < numBlocksX * numBlocksY ; i++ )
	{
		rowmatrix->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
	}

	colmatrix->numberOfBlocks     = numBlocksX*numBlocksY;
	colmatrix->numberOfBlocksXDim = numBlocksX;
	colmatrix->numberOfBlocksYDim = numBlocksY;
	colmatrix->blockSize          = _blockSize;//in elements
	colmatrix->nd = 1;
	colmatrix->blockData = (char**)_malloc_align(sizeof(char*)*numBlocksX*numBlocksY,7);
	for(i = 0;i<numBlocksX*numBlocksY;i++)
	{
		colmatrix->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
	}

	squarematrix->numberOfBlocks = numBlocksX*numBlocksY;
	squarematrix->numberOfBlocksXDim = numBlocksX;
	squarematrix->numberOfBlocksYDim = numBlocksY;
	squarematrix->blockSize = _blockSize;//in elements
	squarematrix->nd = 1;
	squarematrix->blockData = (char**)_malloc_align(sizeof(char*)*numBlocksX*numBlocksY,7);
	for(i = 0;i<numBlocksX*numBlocksY;i++)
	{
		squarematrix->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
	}
}

