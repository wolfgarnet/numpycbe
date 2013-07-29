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

static void * _malloc_align(size_t size, unsigned int log2_align)
{
  void *ret;
  char *real;
  unsigned long offset;
  unsigned long align;

  align = 1 << log2_align;
  real = (char *)malloc(size + 2*sizeof(void *) + (align-1));
  if (real) {
    offset = (align - (unsigned long)(real + 2*sizeof(void *))) & (align-1);
    ret = (void *)((real + 2*sizeof(void *)) + offset);
    *((size_t *)(ret)-2) = size;
    *((void **)(ret)-1) = (void *)(real);
  } else {
    ret = (void *)(real);
  }
  return (ret);
}


void *PpuPthreadFunction( void *arg );

unsigned int GetNumSPEs();

int CreateSPEThread( PpuPthreadData_t *spedata, spe_program_handle_t *context, void *myarg );

int CreateSPEThread2( PpuPthreadData_t *spedata );

int CreateSPEContext( PpuPthreadData_t *spedata, spe_program_handle_t *context, void *myarg );

int CompleteSPEThreads( PpuPthreadData_t *spedata );


//

#endif /* LIBPPU_H_ */
