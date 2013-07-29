/*
 * common_spu.h
 *
 *  Created on: Mar 23, 2009
 *      Author: wolle
 */

#ifndef COMMON_SPU_H_
#define COMMON_SPU_H_

#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include <spu_internals.h>

#include "../dispatcher/init.h"

//#define _DEBUG
unsigned int SPE_id, num_SPES;


typedef struct
{
	void *LS_blockDataArea[__NUMBER_OF_BLOCKS_IN_MEM * 2]; //
	void *LS_shaderMemory;

	char padding[4];
} LS_ShaderInfo __attribute__((aligned (16)));

typedef struct
{
	void (*printstr)( char * );
	void (*printuint)( unsigned int );
	void (*printint)( int );
	void (*printe)( long long );
	void (*printchar)( char );
	void (*printfloat)( float );
	void (*printfloat3)( float, float, float );
	void (*printfloatrow)( float*, unsigned int, unsigned int );
	void (*printfloat3v)( vector float*, vector float*, vector float* );
	void (*printfloatv)( vector float* );
	void (*printaddr)( unsigned int );

	//int (*printf)(const char *, ...); // Doesn't work

	// Important function pointers
	vector float (*rand_0_to_1_f)(void);
	//vector float (*rand_0_to_1_fm)(unsigned int, vector float*); // Doesn't seem to work!!!
} Functions_t;


#endif /* COMMON_SPU_H_ */
