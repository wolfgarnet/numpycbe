//_compute( ##args1##, slice_info->slice_startBlock+firstRowToFetch, numberOfRowsForThisParticularSPE );
void _compute( float *x, float *y, float *r, unsigned int numelements, unsigned int startBlock, unsigned int numberOfRowsForThisParticularSPE, unsigned int blockSize )
{
	// Elementwise product/arraymultiply
	vector float *row1 = (vector float*)( x );
	vector float *row2 = (vector float*)( y );
	vector float *res  = (vector float*)( r );
	unsigned int j;
	
	//JMI. 15/6/09: Changing numelements so it fits with numberOfRowsForThisParticularSPE.
	unsigned elemnum = numberOfRowsForThisParticularSPE * blockSize;
	for( j = 0 ; j < elemnum/*numelements*/ / 4 ; j++ )
	{
		res[j] = spu_mul( row1[j], row2[j] );
	}
}
