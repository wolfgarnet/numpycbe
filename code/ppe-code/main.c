
#include "../common/common.h"
#include "sgetf2.h"
#include "sgetrf.h"
#include "sgesv.h"
#include "fhbtool.h"
#include "pputool.h"
#include <timer.h>
#include <malloc_align.h>

#include "timers.h"

int init( unsigned int numspes )
{
	// Get the number of available SPEs
	speThreads = GetNumSPEs();
	// Clamp to the defined number of SPEs used
	if ( speThreads > MAX_SPE_THREADS )
	{
		speThreads = MAX_SPE_THREADS;
	}
	if( speThreads > numspes )
	{
		speThreads = numspes;
	}
	unsigned int i;

	time_t seconds;
	seconds = time( NULL );

	// Get dispatcher
	spe_program_handle_t *dispatcher = spe_image_open( "dispatcher" );

	// Initialize threads
	for( i = 0 ; i < speThreads ; i++ )
	{
		CreateSPEThread( &speData[i], dispatcher, &spe_pointer_addr[i] );

		seconds++;
		// Sending the SPE its id
		spe_in_mbox_write ( speData[i].spe_ctx, &i, 1, SPE_MBOX_ALL_BLOCKING );
		// Sending the SPE the seed
		spe_in_mbox_write ( speData[i].spe_ctx, &seconds, 1, SPE_MBOX_ALL_BLOCKING );
	}

	return 0;
}

int main()
{
	printf( "SGETF2 test!!!\n" );

	TimerObject *to = GetTimer();
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

	//unsigned int m = 16, n = 16, blocksize = 4, numspes = 1;
	//unsigned int m = 4, n = 4, blocksize = 4, numspes = 1;
	//unsigned int m = 1024, n = 1024, blocksize = 64, numspes = 1;
	unsigned int m = 2048, n = 2048, blocksize = 64, numspes = 6;
	//unsigned int m = 4096, n = 4096, blocksize = 64, numspes = 6;
	PyArrayObject A, B;
	sMakeFHB( m, n, blocksize, 1.0f, &A );
	sMakeFHB( m, n, blocksize, 1.0f, &B );

	init( numspes );

	unsigned int info;

	//PrintMatrix2( &A, 1 );

	StartTimer( to );
	//sgetf2( &A, 0, ipiv, &info );
	//sgetf2_blocked( &A, 0, 4, 4, ipiv, &info );

	//sgetrf( &A, ipiv, &info, solve );
	sgesv( &A, &B, &info );

	StopTimer( to );
	PrintTimer( "Total time: ", to );

	printf( "LU:\n" );
	PrintTimer( "Max: ", t_max );
	PrintTimer( "Scal: ", t_scal );
	PrintTimer( "Swap: ", t_swap );
	PrintTimer( "Sger: ", t_sger );
	PrintTimer( "Strsm1: ", t_strsm1 );
	PrintTimer( "Sgetf2: ", t_sgetf2 );
	PrintTimer( "Sgetrf: ", t_sgetrf );


	printf( "\nSolve:\n" );
	PrintTimer( "Strsm2: ", t_strsm2 );
	PrintTimer( "Strsm3: ", t_strsm3 );
	PrintTimer( "Sgetrs: ", t_sgetrs );

	//PrintMatrix2( &A, 1 );

	//printf( "COUNTER=%u\n", counter );

	return 0;
}
