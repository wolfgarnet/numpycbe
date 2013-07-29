#include "spefuncs.h"

#define SIZE 64

void test_random( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	unsigned int i;

	vector float x;

	for( i = 0 ; i < SIZE ; i++ )
	{
		x = funcs->rand_0_to_1_f();
		funcs->printfloatv(&x);
	}


//	vector float *x2;
//	funcs->rand_0_to_1_fm(SIZE, x2);
//
//	for( i = 0 ; i < SIZE ; i++ )
//	{
//
//		funcs->printfloatv(&x2[i]);
//	}

//	unsigned int i;
//	vector float *x2;
//	vector float *x3;
//	funcs->printuint(667);
//	funcs->rand_0_to_1_fm(SIZE, x2);
//	funcs->rand_0_to_1_fm(SIZE, x3);
//
//	for( i = 0 ; i < SIZE ; i++ )
//	{
//
//		funcs->printfloatv(&x2[i]);
//	}
//
//	for( i = 0 ; i < SIZE ; i++ )
//	{
//		funcs->printfloatv(&x3[i]);
//	}
}
