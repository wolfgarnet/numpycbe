#include "spefuncs.h"

#include <isgreaterf4.h>

// Must be even
#define MAXIMUM_NUMBER_OF_NUMBERS 10
#define MHALF MAXIMUM_NUMBER_OF_NUMBERS / 2
#define MDOUBLE MAXIMUM_NUMBER_OF_NUMBERS * 2

#define RADIUS 1
#define RADIUSV {1,1,1,1}
const vector float MYRADIUS = RADIUSV;

unsigned int _MC( unsigned int its, vector float *rnums1, vector float *rnums2 )
{
	unsigned int j, hits = 0;
	vector float r;
	vector unsigned int gt;

	for( j = 0 ; j < its ; j++ )
	{
		r  = spu_add( spu_mul( rnums1[j], rnums1[j] ), spu_mul( rnums2[j], rnums2[j] ) );
		gt = _isgreaterf4( MYRADIUS, r );
		hits += spu_extract( gt, 0 ) & 1;
		hits += spu_extract( gt, 1 ) & 1;
		hits += spu_extract( gt, 2 ) & 1;
		hits += spu_extract( gt, 3 ) & 1;

		//total += 4;
	}

	return hits;
}

unsigned int _MCLast( vector float *rnums1, vector float *rnums2, unsigned int use )
{
	unsigned int hits = 0, j;
	vector float r;
	vector unsigned int gt;

	r  = spu_add( spu_mul( rnums1[0], rnums1[0] ), spu_mul( rnums2[0], rnums2[0] ) );
	gt = _isgreaterf4( MYRADIUS, r );
	for( j = 0 ; j < use ; j++ )
	{
		hits += spu_extract( gt, j ) & 1;
	}

	return hits;
}

void monte_carlo( unsigned int SPE_id, unsigned int EA_result, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *number = (PyArrayObject *)info->LS_shaderMemory;

	// Get meta data
	_GetPyArrayMetaData( myop->scalar[0], number, 0 );

	_Wait(0); // WAIT!!!

	char **blocks_number = (char**)info->LS_shaderMemory + sizeof( PyArrayObject );

	// The number of iterations
	unsigned int numberuint;
	unsigned int scalarrest;
	_GetScalar( number, blocks_number, &numberuint, 1 );

	//funcs->printuint( numberuint );
	scalarrest = numberuint % 4;
	numberuint /= 4;
	//funcs->printuint( numberuint );

	// Must be corrected
	unsigned int j, i, its;

	its = _CalcNumFromIdTotIt( SPE_id, myop->num_SPES, numberuint );

	funcs->printuint( its );

	unsigned int partition = its / MAXIMUM_NUMBER_OF_NUMBERS;
	unsigned int rest      = its % MAXIMUM_NUMBER_OF_NUMBERS;
//	funcs->printuint( partition );
//	funcs->printuint( rest );
//	funcs->printuint( 555 );

	vector float *rnums1 = (vector float*)info->LS_blockDataArea[0];
	vector float *rnums2 = (vector float*)rnums1+MAXIMUM_NUMBER_OF_NUMBERS*2*4;
	unsigned int hits = 0, total = 0, v;
	float t1, t2, r, ratio = 0, pi, pi2;

	for( i = 0 ; i < partition ; i++ )
	{
		// Create random numbers
		for( j = 0 ; j < MAXIMUM_NUMBER_OF_NUMBERS ; j++ )
		{
			rnums1[j] = funcs->rand_0_to_1_f();
			rnums2[j] = funcs->rand_0_to_1_f();
		}
		hits  += _MC( MAXIMUM_NUMBER_OF_NUMBERS, rnums1, rnums2 );
		total += MAXIMUM_NUMBER_OF_NUMBERS*4;
	}

	// Still some iterations left, less than the maximum number
	for( j = 0 ; j < rest ; j++ )
	{
		rnums1[j] = funcs->rand_0_to_1_f();
		rnums2[j] = funcs->rand_0_to_1_f();
	}
	hits  += _MC( rest, rnums1, rnums2 );
	total += rest*4;

	if( scalarrest )
	{
		rnums1[0] = funcs->rand_0_to_1_f();
		rnums2[0] = funcs->rand_0_to_1_f();
		hits  += _MCLast( rnums1, rnums2, scalarrest );
		total += scalarrest;
	}


	pi = ( (float)hits * 4 ) / (float)total;


	//pi = ( 4 * ratio ) / partition;

//	funcs->printuint( total );
//	funcs->printuint( hits );
//
//	funcs->printfloat(pi);
//
	spu_mfcdma32( &r, EA_result, 4, 0, MFC_PUT_CMD );
	_Wait( 0 );
}



