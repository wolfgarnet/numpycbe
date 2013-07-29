#include <floorf4.h>

/*
 * FLOOR
 *
 * Op1 = array
 *
 */

void _compute( float *x, float *r, unsigned int numelements )
{
	// Elementwise product/arraymultiply
	vector float *row1 = (vector float*)( x );
	vector float *res  = (vector float*)( r );
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = _floorf4( row1[j] );
	}
}

