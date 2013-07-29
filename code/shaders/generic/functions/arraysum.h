
void _compute( float *x, vector float *r, unsigned int numelements )
{
	vector float *row = (vector float*)( x );
	unsigned int j;
	for( j = 0 ; j < numelements / 4 ; j++ )
	{
		*r = spu_add( row[j], *r );
	}
}

float _finish( vector float *res )
{
	float r = spu_extract( *res, 0 );
	r += spu_extract( *res, 1 );
	r += spu_extract( *res, 2 );
	r += spu_extract( *res, 3 );

	return r;
}
