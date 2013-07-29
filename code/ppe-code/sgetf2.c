
/*
 * SGETF2(PPE)
 *
 * Computes an LU factorization of a general M x N matrix
 *
 * BLAS requirements:
 * SGER, SSWAP, ISAMAX
 */

#include <timer.h>

#include <common.h>
#include <common_ppu.h>

#include "sgetf2.h"
#include "../libs/fhbtool.h"
#include <math.h>

#include "timers.h"

// PPE code
#include "isamax.h"
#include "sswap.h"
#include "sscal.h"

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
  			//counter++;
  			Printf1( "j= %u\n", j );
  			Print2( "MAX\n" );
  			StartTimer( t_max );
			jp = FHB_isamax_col2( j+joffset, j+joffset, A );
			StopTimer( t_max );
			Printf2( "END OF MAX, jp=%u\n", jp );
			ipiv[j+joffset] = jp;

			bidx = getblockidx( jp, (j+joffset), A->numberOfBlocksXDim, A->blockSize );
			idx  = getidx( jp, (j+joffset), A->blockSize );
			if( ((float*)A->blockData[bidx])[idx] != 0.0f )
			{

				// Apply interchange to columns 1:N
				if( jp != j+joffset )
				{
					StartTimer( t_swap );
					Printf2( "SWAP(%u, %u) j=%u, jb=%u!\n", j+joffset, jp, j, jb );
					FHB_sswap_row_blocked( A, jb, j+joffset, jp );
					Print2( "END OF SWAP!\n" );
					StopTimer( t_swap );
				}

				// Compute the elements j+1:m of the j'th column
				if( j < A->blockSize - 1 )
				{
					StartTimer( t_scal );
					Print2( "SCAL\n" );
					//printf( "In2\n" );
					bidx = getblockidx( (j+joffset), (j+joffset), A->numberOfBlocksXDim, A->blockSize );
					idx  = getidx( j+joffset, j+joffset, A->blockSize );
					// FHB_sscal_col( unsigned int offset, unsigned int col, unsigned int row, float scale, PyArrayObject *A );
					//Tic();
					FHB_sscal_col_blocked( j+joffset, j+joffset+1, 1/((float*)A->blockData[bidx])[idx], A );
					//PrintTicToc( "SSCAL: ", Toc() );
					Print2( "END OF SCAL\n" );
					StopTimer( t_scal );

					//PrintTimer( "Scal_m: ", t_scal );
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
				StartTimer( t_sger );
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

				state = 110;
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
				StopTimer( t_sger );
			}
		}
	}
}







//
//
//
//
//void sgetf2( PyArrayObject *A, unsigned int jj, unsigned int m, unsigned int n, unsigned int *ipiv, unsigned int *info )
//{
//	printf( "SGETF2\n" );
//
//	unsigned int bidx, idx, bidx2, idx2;
//	m = A->dimensions[0], n = A->dimensions[1];
//	unsigned int br;
//
//	unsigned int mindim = m;
//	if ( n < mindim )
//	{
//		mindim = n;
//	}
//
//	PyArrayObject tmp;
//	PyArrayObject J;
//	PyArrayObject alpha;
//	sMakeFHB( m, A->dimensions[0], A->blockSize, 1.0f, &tmp );
//	uMakeFHB( 1, 1, A->blockSize, 1, &J );
//	sMakeFHB( 1, 1, A->blockSize, -1.0f, &alpha );
//
//	unsigned int i, j, jp;
//
//	// The SGER operation
//	Operation_t op_sger;
//	op_sger.shaderSize = sger_solve_size;
//	op_sger.EA_shader  = sger_solve;
//	op_sger.num_SPES   = 1;
//	op_sger.obj[0]     = A;
//	op_sger.obj[1]     = &tmp;
//	op_sger.scalar[0]  = &alpha;
//	op_sger.scalar[1]  = &J;
//
//	// Send the SPEs the new state
//	unsigned int state = 100;
//	unsigned int checked, r;
//  	for ( i = 0 ; i < speThreads ; i++ )
//	{
//  		spe_pointer_addr[i][0] = (unsigned int)&op_sger;
//  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
//	}
//
//	//for( j = jj ; j < jj+mindim ; j++ )
//  	for( j = 0 ; j < mindim ; j++ )
//	{
//		//printf( "J=%u\n", j );
//		//printf( "ISAMAX. m-j=%u, j=%u\n", m-j, j );
//		// FHB_isamax_col( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A )
//		//Tic();
//		jp = FHB_isamax_col( m - j, j, j, A );
//		//PrintTicToc( "ISAMAX: ", Toc() );
//		ipiv[j] = jp;
//		//printf( "jp=%u, j=%u\n", jp, j );
//
//		bidx = getblockidx( jp, j, A->numberOfBlocksXDim, A->blockSize );
//		//printf( "%u - %u\n", jp/A->blockSize*A->numberOfBlocksXDim, j/A->blockSize );
//		//bidx = jp/A->blockSize*A->numberOfBlocksXDim+j/A->blockSize;
//		idx  = getidx( jp, j, A->blockSize );
//		//idx = (jp%A->blockSize)*A->blockSize + j%A->blockSize;
//		//printf( "%u - %u\n", (jp%A->blockSize)*A->blockSize, j%A->blockSize );
//		//printf( "BEFORE IF(%u,%u)\n", bidx,idx );
//		//Tic();
//		if( ((float*)A->blockData[bidx])[idx] != 0.0f )
//		{
//			//printf( "In1\n" );
//			// Apply interchange to columns 1:N
//			if( jp != j )
//			{
//				//printf( "SSWAP=%u,%u\n", j, jp );
//				// FHB_sswap_row( PyArrayObject *A, unsigned int row1, unsigned int row2 )
//				//Tic();
//				FHB_sswap_row( A, j, jp );
//				//PrintTicToc( "SSWAP: ", Toc() );
//				//printf( "SSWAP end=%u\n", j );
//			}
//
//			// Compute the elements j+1:m of the j'th column
//			if( j < m - 1 )
//			{
//				//printf( "In2\n" );
//				bidx = getblockidx( j, j, A->numberOfBlocksXDim, A->blockSize );
//				idx  = getidx( j, j, A->blockSize );
//				// FHB_sscal_col( unsigned int offset, unsigned int col, unsigned int row, float scale, PyArrayObject *A );
//				//Tic();
//				FHB_sscal_col( m-j-1, j, j+1, 1/((float*)A->blockData[bidx])[idx], A );
//				//PrintTicToc( "SSCAL: ", Toc() );
//
//			}
//
//		}
//		else
//		{
//			//printf( "In3\n" );
//			*info = j;
//		}
//		//PrintTicToc( "BLOCK: ", Toc() );
//
//
//		if( j < mindim )
//		{
//			//printf( "In4=%u\n", speThreads );
//			//PrintMatrix2( A, 1 );
//			// Update trailing submatrix
//
//			// Make x(a column vector)
//			MakeVectorFromMatrixCol( A, &tmp, j );
//
//			// Fill J
//			FHBFillScalarUInt( &J, j );
//
//			//printf( "----->(%u)\b", j );
//			// How many block rows left?
//			br = ((m-j) / A->blockSize)+1;
//			if( br < 18 )
//			{
//				op_sger.num_SPES = br/3;
//			}
//			else
//			{
//				op_sger.num_SPES = 6;
//			}
//			//printf( "NUM=%u\n", op_sger.num_SPES );
//
//
//			// Call shader!!!
//
//		  	state = 110;
//		 	for ( i = 0 ; i < speThreads ; i++ )
//			{
//		  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
//			}
//
//		  	// Waiting for SPEs!
//		 	checked = 0;
//		  	while( checked < speThreads )
//			{
//		  		//printf( "Checked=%u\n", checked );
//				if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
//				{
//					spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
//					checked++;
//				}
//			}
//
//		  	//PrintMatrix2( A, 1 );
//		}
//	}
//
//	printf( "---->\n" );
//
//	DeleteObject( &tmp );
//	DeleteObject( &J );
//}






//void sgetf2( PyArrayObject *A, unsigned int *ipiv, unsigned int *info )
//{
//	printf( "SGETF2\n" );
//
//	unsigned int bidx, idx, bidx2, idx2;
//	unsigned int m = A->dimensions[0], n = A->dimensions[1];
//	unsigned int br;
//
//	unsigned int mindim = m;
//	if ( n < mindim )
//	{
//		mindim = n;
//	}
//
//	PyArrayObject tmp;
//	PyArrayObject J;
//	PyArrayObject alpha;
//	sMakeFHB( m, A->dimensions[0], A->blockSize, 1.0f, &tmp );
//	uMakeFHB( 1, 1, A->blockSize, 1, &J );
//	sMakeFHB( 1, 1, A->blockSize, -1.0f, &alpha );
//
//	unsigned int i, j, jp;
//
//	// The SGER operation
//	Operation_t op_sger;
//	op_sger.shaderSize = sger_solve_size;
//	op_sger.EA_shader  = sger_solve;
//	op_sger.num_SPES   = 1;
//	op_sger.obj[0]     = A;
//	op_sger.obj[1]     = &tmp;
//	op_sger.scalar[0]  = &alpha;
//	op_sger.scalar[1]  = &J;
//
//	// Send the SPEs the new state
//	unsigned int state = 100;
//	unsigned int checked, r;
//  	for ( i = 0 ; i < speThreads ; i++ )
//	{
//  		spe_pointer_addr[i][0] = (unsigned int)&op_sger;
//  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
//	}
//
//	for( j = 0 ; j < mindim ; j++ )
//	{
//		//printf( "J=%u\n", j );
//		//printf( "ISAMAX. m-j=%u, j=%u\n", m-j, j );
//		// FHB_isamax_col( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A )
//		//Tic();
//		jp = FHB_isamax_col( m - j, j, j, A );
//		//PrintTicToc( "ISAMAX: ", Toc() );
//		ipiv[j] = jp;
//		//printf( "jp=%u, j=%u\n", jp, j );
//
//		bidx = getblockidx( jp, j, A->numberOfBlocksXDim, A->blockSize );
//		//printf( "%u - %u\n", jp/A->blockSize*A->numberOfBlocksXDim, j/A->blockSize );
//		//bidx = jp/A->blockSize*A->numberOfBlocksXDim+j/A->blockSize;
//		idx  = getidx( jp, j, A->blockSize );
//		//idx = (jp%A->blockSize)*A->blockSize + j%A->blockSize;
//		//printf( "%u - %u\n", (jp%A->blockSize)*A->blockSize, j%A->blockSize );
//		//printf( "BEFORE IF(%u,%u)\n", bidx,idx );
//		Tic();
//		if( ((float*)A->blockData[bidx])[idx] != 0.0f )
//		{
//			//printf( "In1\n" );
//			// Apply interchange to columns 1:N
//			if( jp != j )
//			{
//				//printf( "SSWAP=%u,%u\n", j, jp );
//				// FHB_sswap_row( PyArrayObject *A, unsigned int row1, unsigned int row2 )
//				//Tic();
//				FHB_sswap_row( A, j, jp );
//				//PrintTicToc( "SSWAP: ", Toc() );
//				//printf( "SSWAP end=%u\n", j );
//			}
//
//			// Compute the elements j+1:m of the j'th column
//			if( j < m - 1 )
//			{
//				//printf( "In2\n" );
//				bidx = getblockidx( j, j, A->numberOfBlocksXDim, A->blockSize );
//				idx  = getidx( j, j, A->blockSize );
//				if ( fabs( ((float*)A->blockData[bidx])[idx] ) > 0 ) // OOOOBBBBBSSSSS!!!!!
//				{
//					// FHB_sscal_col( unsigned int offset, unsigned int col, unsigned int row, float scale, PyArrayObject *A );
//					//Tic();
//					FHB_sscal_col( m-j-1, j, j+1, 1/((float*)A->blockData[bidx])[idx], A );
//					//PrintTicToc( "SSCAL: ", Toc() );
//				}
//				else
//				{
//					for( i = 0 ; i < m-j+1 ; i++ )
//					{
//						bidx2 = getblockidx( j+1, j, A->numberOfBlocksXDim, A->blockSize );
//						idx2  = getidx( j+1, j, A->blockSize );
//						((float*)A->blockData[bidx2])[idx2] /= ((float*)A->blockData[bidx])[idx];
//					}
//				}
//			}
//
//		}
//		else
//		{
//			//printf( "In3\n" );
//			*info = j;
//		}
//		PrintTicToc( "BLOCK: ", Toc() );
//
//		if( j < mindim )
//		{
//			//printf( "In4=%u\n", speThreads );
//			//PrintMatrix2( A, 1 );
//			// Update trailing submatrix
//
//			// Make x(a column vector)
//			MakeVectorFromMatrixCol( A, &tmp, j );
//
//			// Fill J
//			FHBFillScalarUInt( &J, j );
//
//			//printf( "----->(%u)\b", j );
//			// How many block rows left?
//			br = ((m-j) / A->blockSize)+1;
//			if( br < 6 )
//			{
//				op_sger.num_SPES = br;
//			}
//			else
//			{
//				op_sger.num_SPES = 6;
//			}
//			//printf( "NUM=%u\n", op_sger.num_SPES );
//
//
//			// Call shader!!!
//
//		  	state = 110;
//		 	for ( i = 0 ; i < speThreads ; i++ )
//			{
//		  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
//			}
//
//		  	// Waiting for SPEs!
//		 	checked = 0;
//		  	while( checked < speThreads )
//			{
//		  		//printf( "Checked=%u\n", checked );
//				if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
//				{
//					spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
//					checked++;
//				}
//			}
//
//		  	//PrintMatrix2( A, 1 );
//		}
//	}
//
//	DeleteObject( &tmp );
//	DeleteObject( &J );
//}
