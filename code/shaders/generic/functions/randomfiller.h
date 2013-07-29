
void _compute( float *x, unsigned int numelements, vector float(*random)(void) )
{
	vector float *res = (vector float*)( x );
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		res[j] = random();
	}
}

