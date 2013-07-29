/*
 * mem_test_ppu.c
 *
 *  Created on: Apr 12, 2009
 *      Author: wolle
 */

#include <pputool.h>
#include <timer.h>
#include <fhbtool.h>
#include "../../common/common.h"
#include "../../common/common_ppu.h"

#include <malloc_align.h>

// SHADERS
#include "../../shaders/memtest_fhb.h"
#include "../../shaders/memtest_con.h"

unsigned int speThreads;
#define MAX_SPE_THREADS 6

unsigned int spe_pointer_addr[MAX_SPE_THREADS][4] __attribute__((aligned(16)));
PpuPthreadData_t speData[MAX_SPE_THREADS];

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

	// Get dispatcher
	spe_program_handle_t *dispatcher = spe_image_open( "dispatcher" );

	// Initialize threads
	for( i = 0 ; i < speThreads ; i++ )
	{
		CreateSPEThread( &speData[i], dispatcher, &spe_pointer_addr[i] );

		// Sending the SPE its id
		spe_in_mbox_write ( speData[i].spe_ctx, &i, 1, SPE_MBOX_ALL_BLOCKING );
	}

	return 0;
}

double mem_test( PyArrayObject *pyobj1, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;
	op1.obj[0]     = pyobj1;
	op1.num_SPES   = speThreads;

	unsigned int checked = 0;

	unsigned int state = 3, r;
	unsigned int i;

	//printf( "Sending states to SPEs\n" );
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	// Waiting for SPEs!
  	//printf( "Waiting\n" );
  	while( checked < speThreads )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			//printf( "CHECKED=%u\n", checked );
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}

  	Tic();
  	//printf( "RUN\n" );
  	// Make SPEs run
  	state = 5;
 	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
 	//printf( "Waiting again!\n" );
 	checked = 0;
  	while( checked < speThreads )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}


	double time = Toc_d();
	PrintTicToc( "Finished at ", Toc() );

	//printf( "Result=%f\n", r1 );

	return time;
}



int main( int argc, char *argv[] )
{
	unsigned int m = 128, n = 128, blocksize = 64, numspes = 1;
	// There are arguments
	if ( argc > 1 )
	{
		// The first argument is present
		if ( argc > 1 )
		{
			m = atoi( argv[1] );
		}

		if ( argc > 2 )
		{
			n = atoi( argv[2] );
		}

		if ( argc > 3 )
		{
			blocksize = atoi( argv[3] );
		}

		if ( argc > 4 )
		{
			numspes = atoi( argv[4] );
		}
	}

	double seconds;
	unsigned int bytes = m*n*4;
	init( numspes );

	PyArrayObject pyobj1;
	sMakeFHB( m, n, blocksize, 1.0f, &pyobj1 );


	seconds = mem_test( &pyobj1, memtest_fhb_size, memtest_fhb );

	pputimer_GetStats( seconds, bytes, 1000 );

	DeleteObject( &pyobj1 );

	// Contigeous

	pyobj1.blockData    = (char**)_malloc_align( 4, 7 );
	pyobj1.blockData[0] = (char*)_malloc_align( m*n*4, 7 );
	pyobj1.blockData    = (char**)pyobj1.blockData[0];

	seconds = mem_test( &pyobj1, memtest_con_size, memtest_con );

	pputimer_GetStats( seconds, bytes, 1000 );


	return 1;
}
