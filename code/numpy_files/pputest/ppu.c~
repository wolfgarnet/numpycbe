/*
 * ppu.c
 *
 *  Created on: Mar 23, 2009
 *      Author: wolle
 */

#include "common.h"
#include "common_ppu.h"
#include "libppu.h"
#include "binreader.h"
#include "timer.h"
#include <stdlib.h>

#include "sdot.h"
#include "sdotv.h"
#include "snrm2.h"
#include "snrm2v.h"

#define MAX_SPE_THREADS 6

unsigned int spe_pointer_addr[MAX_SPE_THREADS][4] __attribute__((aligned(16)));
PpuPthreadData_t speData[MAX_SPE_THREADS];

unsigned int speThreads;


void *sMakeBlock( unsigned int elements, float value )
{
	void *block = (void *)memalign( 16, ( 4 * elements + 15 ) & ~15 );
	void *p = block;
	unsigned int i;
	for( i = 0 ; i < elements ; i++ )
	{
		*(float*)p = value;
		//printf( "%f, ", *(float*)p );
		p += 4;
	}
	return block;
}

void sMakeMatrix( unsigned int x, unsigned int y, unsigned int blocksize, float value, PyArrayObject *obj )
{
	obj->blockData = (char **)memalign( 16, ( 4 * x * y + 15 ) & ~15 );
	//printf( "TEST=%u\n", ( 4 * x * y + 15 ) & ~15 );
	obj->numberOfBlocks     = x * y;
	obj->numberOfBlocksXDim = x;
	obj->numberOfBlocksYDim = y;
	obj->size_in_bytes      = 4;
	obj->blockSizes         = blocksize*blocksize;
	unsigned int i;
	for( i = 0 ; i < x ; i++ )
	{
		obj->blockData[i] = sMakeBlock( blocksize*blocksize, value );
		//printf( "1.=%f\n", *(float*)obj->blockData[i] );
	}
}

int SNRM2( PyArrayObject *pyobj1, unsigned int shadersize, unsigned int *shader )
{
	Tic();
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;
	op1.obj[0]     = pyobj1;
	op1.num_SPES   = speThreads;

	float *results[6];

	unsigned int state = 2, r;
	unsigned int i;
	//printf( "Sending states to SPEs\n" );
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		results[i] = (float *)memalign( 128, ( 4 + 127 ) & ~127 );
  		r = (unsigned int)results[i];
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
  		spe_in_mbox_write ( speData[i].spe_ctx, &r, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
  	//printf( "Waiting for SPEs\n" );
  	unsigned int checked = 0;
	while( checked < speThreads )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
	float r1 = 0.0f;
	for ( i = 0 ; i < speThreads ; i++ )
	{
		r1 += results[i][0];
		// Cleanup
		free( results[i] );
	}

	r1 = sqrtf( r1 );

	PrintTicToc( "Finished at ", Toc() );

	printf( "Result=%f\n", r1 );

	return 1;
}

int SDOT( PyArrayObject *pyobj1, PyArrayObject *pyobj2, unsigned int shadersize, unsigned int *shader )
{
	Tic();
	Operation_t op1;
	op1.shaderSize = shadersize;
	op1.EA_shader  = shader;
	op1.obj[0]     = pyobj1;
	op1.obj[1]     = pyobj2;
	op1.num_SPES   = speThreads;

	float *results[6];

	unsigned int state = 2, r;
	unsigned int i;
	//printf( "Sending states to SPEs\n" );
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		results[i] = (float *)memalign( 128, ( 4 + 127 ) & ~127 );
  		r = (unsigned int)results[i];
  		spe_pointer_addr[i][0] = (unsigned int)&op1;
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
  		spe_in_mbox_write ( speData[i].spe_ctx, &r, 1, SPE_MBOX_ALL_BLOCKING );
	}

  	// Waiting for SPEs!
  	//printf( "Waiting for SPEs\n" );
  	unsigned int checked = 0;
	while( checked < speThreads )
	{
		if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
		{
			spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
			checked++;
		}
	}
	float r1 = 0.0f;
	for ( i = 0 ; i < speThreads ; i++ )
	{
		r1 += results[i][0];
		// Cleanup
		free( results[i] );
	}

	PrintTicToc( "Finished at ", Toc() );

	printf( "Result=%f\n", r1 );

	return 1;
}

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



int main( int argc, char *argv[] )
{
	unsigned int numspes = 1, i;
	unsigned int numblocks = 8, blocksize = 4;
	unsigned int type = 1;

	// There are arguments
	if ( argc > 1 )
	{
		// The first argument is present
		if ( argc > 1 )
		{
			numblocks = atoi( argv[1] );
		}

		if ( argc > 2 )
		{
			blocksize = atoi( argv[2] );
		}

		if ( argc > 3 )
		{
			type = atoi( argv[3] );
		}

		if ( argc > 4 )
		{
			numspes = atoi( argv[4] );
		}

	}
	else
	{
		printf( "Usage pputest <numblocks> <blocksize^2> <type> <num spes>\n" );
		printf( "type: 1: sdot, 2: sdotv, 3: snrm2, 4: snrm2v\n" );
		return -1;
	}

	init( numspes );

	printf( "Testing SDOT()\n" );
	printf( "Num SPEs:\t%u\n", speThreads );
	printf( "------------------\n" );
	printf( "Num blocks:\t\t%u\n", numblocks );
	printf( "Num elements pr block: \t%u\n", blocksize*blocksize );
	printf( "1 block in bytes: \t%u\n", blocksize*blocksize*4 );
	printf( "Num elements:\t\t%u\n", numblocks*blocksize*blocksize );
	printf( "Total size in MB: \t%f\n", (double)(numblocks*blocksize*blocksize*4)/(1024*1024) );
	printf( "Total size in MB: \t%f\n", (double)(numblocks*blocksize*blocksize*4)/(1024*1024)*2 );
	printf( "------------------\n" );

	// PyArrayObject
	PyArrayObject pyobj1;
	PyArrayObject pyobj2;
	PyArrayObject pyscalar1;
	sMakeMatrix( numblocks, 1, blocksize, 1.0f, &pyobj1 );
	sMakeMatrix( numblocks, 1, blocksize, 1.0f, &pyobj2 );
	sMakeMatrix( 1, 1, 8, 1.0f, &pyscalar1 );


	unsigned int *shader;

	switch( type )
	{
	case 1:
		shader = sdot;
		printf( "Calling SDOT();\n" );
		SDOT( &pyobj1, &pyobj2, sdot_size, shader );
		break;

	case 2:
		shader = sdotv;
		printf( "Calling SDOTvvvv();\n" );
		SDOT( &pyobj1, &pyobj2, sdotv_size, shader );
		break;

	case 3:
		shader = snrm2;
		printf( "Calling SNRM2();\n" );
		SNRM2( &pyobj1, snrm2_size, shader );
		break;

	case 4:
		shader = snrm2v;
		printf( "Calling SNRM2vvvvv();\n" );
		SNRM2( &pyobj1, snrm2v_size, shader );
		break;
	}


 	unsigned int state = 0;
  	for ( i = 0 ; i < speThreads ; i++ )
	{
  		spe_in_mbox_write ( speData[i].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
	}


  	// Wait for all the SPE threads to complete.

  	for ( i = 0 ; i < speThreads ; i++ )
	{
		CompleteSPEThreads( &speData[i] );
	}

	return 1;
}
