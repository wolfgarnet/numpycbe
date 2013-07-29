/*
 * libppu.c
 *
 *  Created on: Jan 14, 2009
 *      Author: wolle
 */

#include "pputool.h"

// The ppu thread function, running an spe context
void *PpuPthreadFunction( void *arg )
{
	// Cast the argument to ppu_pthread_data_t
	PpuPthreadData_t *datap = (PpuPthreadData_t *)arg;
	if ( spe_context_run( datap->spe_ctx, &datap->entry, 0, datap->argp, NULL, NULL ) < 0 )
	{
    	perror ( "Failed running context\n" );
	}
	pthread_exit( NULL );
}

unsigned int GetNumSPEs()
{
	return spe_cpu_info_get( SPE_COUNT_USABLE_SPES, -1 );
}

int CreateSPEThread( PpuPthreadData_t *spedata, spe_program_handle_t *context, void *myarg )
{
	// create SPE context
    if ( ( spedata->spe_ctx = spe_context_create ( 0, NULL ) ) == NULL )
	{
		perror ( "Failed creating context" );
        return -1;
    }

	// Load program into context
    if ( spe_program_load ( spedata->spe_ctx, context ) )
	{
		perror ( "Failed loading program" );
		return -1;
    }

    // Initialize context run data
    spedata->entry = SPE_DEFAULT_ENTRY;
    //speData[i].argp  = mydata;
    spedata->argp  = myarg;
    // Create pthread for each of the SPE conexts
    if ( pthread_create ( &spedata->pthread, NULL, &PpuPthreadFunction, spedata ) )
    {
      	perror ( "Failed creating thread" );
      	return -1;
    }
    return 1;
}

int CreateSPEContext( PpuPthreadData_t *spedata, spe_program_handle_t *context, void *myarg )
{
	// create SPE context
    if ( ( spedata->spe_ctx = spe_context_create ( 0, NULL ) ) == NULL )
	{
		perror ( "Failed creating context" );
        return -1;
    }

	// Load program into context
    if ( spe_program_load ( spedata->spe_ctx, context ) )
	{
		perror ( "Failed loading program" );
		return -1;
    }

    // Initialize context run data
    spedata->entry = SPE_DEFAULT_ENTRY;
    //speData[i].argp  = mydata;
    spedata->argp  = myarg;

    return 1;
}

int CreateSPEThread2( PpuPthreadData_t *spedata )
{
	// Create pthread for each of the SPE conexts
	if ( pthread_create ( &spedata->pthread, NULL, &PpuPthreadFunction, spedata ) )
	{
		perror ( "Failed creating thread" );
		return -1;
	}

	return 1;
}

int CompleteSPEThreads( PpuPthreadData_t *spedata )
{
	// Wait for the SPE threads to end
	if ( pthread_join ( spedata->pthread, NULL ) )
	{
 	 	perror ("Failed joining thread");
  		return -1;
	}

	// Destroy context
	if ( spe_context_destroy ( spedata->spe_ctx ) != 0 )
	{
		perror( "Failed destroying context" );
		return -1;
	}

	return 1;
}

//

