
void _compute( float *x, unsigned int numelements )
{
	vector float *res = (vector float*)( x );
	vector float ones = { 1, 1, 1, 1 };
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = ones;
	}
}


