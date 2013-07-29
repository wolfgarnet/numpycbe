
void _compute( float *x, unsigned int numelements )
{
	vector float *res = (vector float*)( x );
	vector float zero = { 0, 0, 0, 0 };
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = zero;
	}
}

