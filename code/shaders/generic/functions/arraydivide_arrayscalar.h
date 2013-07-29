#include <divf4.h>

/*
 * DIVIDE
 *
 * Op1 = array, op2 = scalar
 *
 * A / s
 */

void _compute( float *x, vector float *y, float *r, unsigned int numelements )
{
	// Elementwise product/arraymultiply
	vector float *row1 = (vector float*)( x );
	vector float *res  = (vector float*)( r );
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = _divf4( row1[j], *y );
	}
}
