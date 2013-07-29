
/*
 * GREATER THAN OR EQUAL
 *
 * Op1 = array, op2 = array
 *
 */

#include <isgreaterequalf4.h>

#define _TRUE 0xff
#define _FALSE 0x0

void _compute( float *x, float *y, float *r, unsigned int numelements )
{
	// Elementwise product/arraymultiply
	vector float *row1 = (vector float*)( x );
	vector float *row2 = (vector float*)( y );
	vector unsigned int *res  = (vector unsigned int*)( r );
	char *rr = (char*)r;
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = _isgreaterequalf4( row1[j], row2[j] );
		*rr = (char)spu_extract( res[j], 0 ) & _TRUE;
		rr++;
		*rr = (char)spu_extract( res[j], 1 ) & _TRUE;
		rr++;
		*rr = (char)spu_extract( res[j], 2 ) & _TRUE;
		rr++;
		*rr = (char)spu_extract( res[j], 3 ) & _TRUE;
		rr++;
	}
}