
#include <common.h>
#include <common_ppu.h>
#include "sgetrs.h"
#include "slaswp.h"
#include "../libs/fhbtool.h"

#include "timers.h"

void sgetrs( PyArrayObject *A, PyArrayObject *B, unsigned int *ipiv, unsigned int *info, Solve_t *solve )
{
	unsigned int i, checked, r, state;

	// Apply row interchanges to the right hand side
	slaswp_blocked( B->numberOfBlocksYDim, B, 0, 0, ipiv );

	/*
	 * Solve L * X = B, overwriting B with X
	 */
	Print1( "Solve L * X = B\n" );
	StartTimer( t_strsm2 );

	// Fill J
	FHBFillScalarUInt( solve->strsm_llnu->scalar[1], A->numberOfBlocksYDim+1 );

	// Call shader!!!

	//solve->strsm_llnu->num_SPES = 6;

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
		//bprintf( "Checked=%u\n", checked );
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}


	Print1( "Done solving L * X = B\n" );

	StopTimer( t_strsm2 );

	/*
	 * Solve U * X = B, overwriting B with X
	 */
	Print1( "Solve U * X = B\n" );
	StartTimer( t_strsm3 );

	// Call shader!!!

	state = 113;

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

	StopTimer( t_strsm3 );
}

