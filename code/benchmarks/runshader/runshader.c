/*
 * runshader.c
 *
 *  Created on: Apr 16, 2009
 *      Author: wolle
 */

#include <pputool.h>
#include <timer.h>
#include <fhbtool.h>
#include "../../common/common.h"
#include "../../common/common_ppu.h"

#include <malloc_align.h>

// Shaders
#include "../../shaders/test_random.h"
#include "../../shaders/monte_carlo.h"


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

void runmc( PyArrayObject *pyobj1, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;
	op1.num_SPES   = speThreads;
	op1.scalar[0]  = pyobj1;

	unsigned int checked = 0;

	unsigned int state = 3, r;
	unsigned int i;

  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	// Waiting for SPEs!
  	while( checked < speThreads )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			//printf( "CHECKED=%u\n", checked );
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}

  	state = 5;
 	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
 	checked = 0;
  	while( checked < speThreads )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
}


int main( int argc, char *argv[] )
{
	unsigned int numspes = 1;
	// There are arguments
	if ( argc > 1 )
	{
		if ( argc > 1 )
		{
			numspes = atoi( argv[1] );
		}
	}

	init( numspes );



	//run( test_random_size, test_random );
	PyArrayObject pyobj1;
	sMakeFHB( 1, 1, 4, 1.0f, &pyobj1 );
	((unsigned int**)pyobj1.blockData)[0][0] = (unsigned int)1000001;
	runmc( &pyobj1, monte_carlo_size, monte_carlo );

	return 1;
}
