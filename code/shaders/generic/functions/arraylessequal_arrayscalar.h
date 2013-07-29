
/*
 * LESS THAN OR EQUAL
 *
 * Op1 = array, op2 = scalar
 *
 * A <= s   ==>    s > A
 *
 */

#include <isgreaterf4.h>

#define _TRUE 0x3f800000
#define _FALSE 0x0

void _compute( float *x, vector float *y, float *r, unsigned int numelements )
{
	// Elementwise product/arraymultiply
	vector float *row1 = (vector float*)( x );
	vector unsigned int *res  = (vector unsigned int*)( r );
	unsigned int *rr = (unsigned int*)r;
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = _isgreaterf4( *y,row1[j] );
		*rr = (unsigned int)spu_extract( res[j], 0 ) & _TRUE;
		rr++;
		*rr = (unsigned int)spu_extract( res[j], 1 ) & _TRUE;
		rr++;
		*rr = (unsigned int)spu_extract( res[j], 2 ) & _TRUE;
		rr++;
		*rr = (unsigned int)spu_extract( res[j], 3 ) & _TRUE;
		rr++;
	}
}

