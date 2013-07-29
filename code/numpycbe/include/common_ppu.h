/*
 * common_ppu.h
 *
 *  Created on: Jan 27, 2009
 *      Author: wolle
 */

#ifndef COMMON_PPU_H_
#define COMMON_PPU_H_

#include <libspe2.h>
#include <pthread.h>

#define MAX_SPE_THREADS 6;

// data and args for each spe thread
typedef struct PpuPthreadData
{
	spe_context_ptr_t spe_ctx;
	pthread_t pthread;
	unsigned int entry;
	void *argp;
} PpuPthreadData_t;

// TEST

unsigned int speThreads;
#define MAX_SPE_THREADS 6

float resultFloats[MAX_SPE_THREADS][4] __attribute__((aligned(16)));

unsigned int spe_pointer_addr[MAX_SPE_THREADS][4] __attribute__((aligned(16)));
PpuPthreadData_t speData[MAX_SPE_THREADS];


#endif /*COMMON_PPU_H_*/

