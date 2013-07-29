#include <common.h>
#include <common_ppu.h>

#include "../libs/fhbtool.h"


#include "sgetrf.h"
#include "sgetf2.h"
#include "slaswp.h"

#include "timers.h"

void sgetrf( PyArrayObject *A, unsigned int *ipiv, unsigned int *info, Solve_t *solve )
{
	printf( "SGETRF\n" );

	unsigned int j, jb = _NUMBLOCKS, state, i, checked, r;
	//unsigned int m = A->dimensions[0], n = A->dimensions[1];




  	// Sgemm

  	// Strsm

	for( j = 0 ; j < A->numberOfBlocksYDim ; j++ )
	{
		Printf1( "SGETFT: J=%u\n", j );

		StartTimer( t_sgetf2 );
		// Factor diagonal and subdiagonal blocks and test for exact singularity
		sgetf2_blocked( A, j, A->blockSize, A->blockSize, ipiv, info, solve );
		StopTimer( t_sgetf2 );

		//PrintMatrix2( A, 1 );

		// Apply changesto columns 1:n

		// slaswp();
		// void slaswp( unsigned int n, PyArrayObject *A, unsigned int k1, unsigned int k2, unsigned int *ipiv );
		//slaswp( A->blockSize * j, A, j*A->blockSize, (j+1)*A->blockSize-1, ipiv );
		// d( unsigned int nb, PyArrayObject *A, unsigned int offsetb, unsigned int yb, unsigned int *ipiv );
		if( j > 0 )
		{
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

			StartTimer( t_strsm1 );

			// Fill J
			FHBFillScalarUInt( solve->strsm_llnu->scalar[1], j );

			// Call shader!!!

			state = 112;

			for ( i = 0 ; i < solve->strsm_llnu->num_SPES ; i++ )
			{
				spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
			}

			//printf( "Shader called!\n" );

			// Waiting for SPEs!
			checked = 0;
			while( checked < solve->strsm_llnu->num_SPES )
			{
				//printf( "Checked=%u\n", checked );
				if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
				{
					spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
					checked++;
				}
			}


			Print1( "Finshed computing block row of U\n" );
			StopTimer( t_strsm1 );
			//PrintTimer( "Strsm:", t_strsm1 );




			if( j + jb < A->numberOfBlocksYDim )
			{
				// Update trailing sub matrix
				// sgemm

			}
		}
	}

}
