
/*
 * ADD
 *
 * Op1 = scalar, op2 = array
 *
 * s / A
 */

void _compute( float *x, vector float *y, float *r, unsigned int numelements )
{
	// Elementwise product/arraymultiply
	vector float *row1 = (vector float*)( x );
	vector float *res  = (vector float*)( r );
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = spu_add( *y, row1[j] );
	}
}

