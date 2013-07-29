
/*
 * Subtract
 *
 * Op1 = array, op2 = array
 *
 */

void _compute( float *x, float *y, float *r, unsigned int numelements )
{
	// Elementwise product/arraymultiply
	vector float *row1 = (vector float*)( x );
	vector float *row2 = (vector float*)( y );
	vector float *res  = (vector float*)( r );
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = spu_sub( row1[j], row2[j] );
	}
}

