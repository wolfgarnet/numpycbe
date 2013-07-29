
#include <common.h>
#include <common_ppu.h>
#include "timers.h"
#include <fhbtool.h>

#include <malloc_align.h>


#include "sgetrf.h"
#include "sgetrs.h"

// SPU shaders
#include <sger_lu.h>
#include <strsm_llnu.h>
#include <strsm_lunn.h>

void sgesv( PyArrayObject *A, PyArrayObject *B, unsigned int *info )
{
	/*
	 * CBE SPECIFICS
	 */

	unsigned int state = 100;
	unsigned int i;

	Solve_t solve;

	// Make temporary objects
	PyArrayObject tmp;
	PyArrayObject J;
	PyArrayObject alpha;
	sMakeFHB( A->blockSize, A->dimensions[0], A->blockSize, 1.0f, &tmp );
	uMakeFHB( 1, 1, A->blockSize, 1, &J );
	sMakeFHB( 1, 1, A->blockSize, -1.0f, &alpha );

	/*
	 * The SGER operation
	 */
	Operation_t op_sger;
	op_sger.shaderSize = sger_lu_size;
	op_sger.EA_shader  = sger_lu;
	op_sger.num_SPES   = 1;
	op_sger.obj[0]     = A;
	op_sger.obj[1]     = &tmp;
	op_sger.scalar[0]  = &alpha;
	op_sger.scalar[1]  = &J;

	solve.sger = &op_sger;

	// Send the SPEs the new state

	Print1( "Signaling sger\n" );

  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op_sger;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	/*
  	 * The STRSM left lower non-trasnposed unit operation
  	 */
	Operation_t op_strsm_llnu;
	op_strsm_llnu.shaderSize = strsm_llnu_size;
	op_strsm_llnu.EA_shader  = strsm_llnu;
	op_strsm_llnu.num_SPES   = 1;
	op_strsm_llnu.obj[0]     = A;
	op_strsm_llnu.obj[1]     = B;
	op_strsm_llnu.scalar[0]  = &alpha;
	op_strsm_llnu.scalar[1]  = &J;

	solve.strsm_llnu = &op_strsm_llnu;

	Print1( "Signaling strsm llnu\n" );

	// Send the SPEs the new state
	state = 102;
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][2] = (unsigned int)&op_strsm_llnu;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	/*
  	 * The STRSM left upper non-trasnposed non-unit operation
  	 */
	Operation_t op_strsm_lunn;
	op_strsm_lunn.shaderSize = strsm_lunn_size;
	op_strsm_lunn.EA_shader  = strsm_lunn;
	op_strsm_lunn.num_SPES   = 6;
	op_strsm_lunn.obj[0]     = A;
	op_strsm_lunn.obj[1]     = B;
	op_strsm_lunn.scalar[0]  = &alpha;

	solve.strsm_lunn = &op_strsm_lunn;

	Print1( "Signaling strsm lunn\n" );

	// Send the SPEs the new state
	state = 103;
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][3] = (unsigned int)&op_strsm_lunn;
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
	StartTimer( t_sgetrf );
	sgetrf( A, ipiv, info, &solve );
	StopTimer( t_sgetrf );


	// Update operations
	op_strsm_llnu.num_SPES = 6;
	state = 122;
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][2] = (unsigned int)&op_strsm_llnu;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

	/*
	 * Solve
	 */
	Print1( "Calling sgetrs\n" );
	StartTimer( t_sgetrs );
	sgetrs( A, B, ipiv, info, &solve );
	StopTimer( t_sgetrs );


  	/*
  	 * The end
  	 */


	DeleteObject( &tmp );
	DeleteObject( &J );
}
