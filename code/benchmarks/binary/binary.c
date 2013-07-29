/*
 * binary.c
 *
 *  Created on: Apr 20, 2009
 *      Author: wolle
 */


#include <pputool.h>
#include <timer.h>
#include <fhbtool.h>
#include "../../common/common.h"
#include "../../common/common_ppu.h"

#include <malloc_align.h>

// Shaders
#include "../../shaders/arraymultiply.h"
#include "../../shaders/arrayadd.h"

#include "../../shaders/arraymultiply_scalar.h"
#include "../../shaders/arrayadd_scalar.h"

#include "../../shaders/arraylessequal_scalar.h"


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

void run( PyArrayObject *pyobj1, PyArrayObject *pyobj2, PyArrayObject *pyobj3, unsigned int shadersize, unsigned int *shader )
{
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;
	op1.num_SPES   = speThreads;
	op1.obj[0]  = pyobj1;
	op1.obj[1]  = pyobj2;
	op1.obj[2]  = pyobj3;

	unsigned int state = 1, r;
	unsigned int i;

  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	// Waiting for SPEs!
 	unsigned int checked = 0;
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
	unsigned int numspes = 1, blocksize = 64, m = 1, n = 1024;
	// There are arguments
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

	init( numspes );



	//run( test_random_size, test_random );
	PyArrayObject pyobj1, pyobj2, pyobj3;
	sMakeFHB( m, n, blocksize, 3.0f, &pyobj1 );
	//sMakeFHB( m, n, blocksize, 2.0f, &pyobj2 );
	sMakeFHB( 1, 1, blocksize, 4.0f, &pyobj2 );
	bMakeFHB( m, n, blocksize, 1, &pyobj3 );

//	if( m == 1 || n == 1 )
//	{
//		PrintVector( &pyobj1 );
//		PrintVector( &pyobj3 );
//	}
//	else
//	{
//		PrintMatrix( &pyobj1 );
//		PrintMatrix( &pyobj3 );
//	}


	//run( &pyobj1, &pyobj2, &pyobj3, arraymultiply_size, arraymultiply );
	//run( &pyobj1, &pyobj2, &pyobj3, arrayadd_scalar_size, arrayadd_scalar );
	//run( &pyobj1, &pyobj2, &pyobj3, arraymultiply_scalar_size, arraymultiply_scalar );
	run( &pyobj1, &pyobj2, &pyobj3, arraylessequal_scalar_size, arraylessequal_scalar );


	PrintNDArray( &pyobj1, 0 );
	PrintNDArray( &pyobj3, 0 );

	return 1;
}
