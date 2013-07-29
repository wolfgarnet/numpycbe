/*
 * libppu.h
 *
 *  Created on: Jan 27, 2009
 *      Author: wolle
 */

#ifndef LIBPPU_H_
#define LIBPPU_H_


#include "common.h"
#include "common_ppu.h"
#include <malloc.h>



void *PpuPthreadFunction( void *arg );

unsigned int GetNumSPEs();

int CreateSPEThread( PpuPthreadData_t *spedata, spe_program_handle_t *context, void *myarg );

int CreateSPEThread2( PpuPthreadData_t *spedata );

int CreateSPEContext( PpuPthreadData_t *spedata, spe_program_handle_t *context, void *myarg );

int CompleteSPEThreads( PpuPthreadData_t *spedata );


//

#endif /* LIBPPU_H_ */
