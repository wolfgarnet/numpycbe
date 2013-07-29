
#include <common.h>

/*
 * SLASWP
 *
 * k1 < k2
 */

void slaswp( unsigned int n, PyArrayObject *A, unsigned int k1, unsigned int k2, unsigned int *ipiv )
{
	unsigned int i, j, k, ip;
	unsigned int k1x = k1;

	float temp;

	Printf2( "SLASWP!!!\nk1=%u, k2=%u\n", k1, k2 );

	unsigned int brow1, brow2, r1, r2, maxb = n / A->blockSize;
	if( n % A->blockSize != 0 )
	{
		maxb++;
	}

	for( i = k1 ; i < k2+1 ; i++ )
	{
		Printf2( "I=%u\n", i );
		ip = ipiv[k1x];

		Printf2( "ip=%u, k1x=%u\n", ip, k1x );

		brow1 = ip  / A->blockSize * A->numberOfBlocksXDim;
		brow2 = k1x / A->blockSize * A->numberOfBlocksXDim;
		r1    = ip  % A->blockSize * A->blockSize;
		r2    = k1x % A->blockSize * A->blockSize;

		// Exchange row k1x with ip
		for( j = 0 ; j < maxb-1 ; j++ )
		{
			//printf( "J=%u\n", j );
			// One row in a block
			for( k = 0 ; k < A->blockSize ; k++ )
			{
//				printf( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
//				printf( "brow1=%u, brow2=%u, r1=%u, r2=%u\n", brow1, brow2, r1, r2 );

				temp = ((float*)A->blockData[brow1])[r1+k];
				((float*)A->blockData[brow1])[r1+k] = ((float*)A->blockData[brow2])[r2+k];
				((float*)A->blockData[brow2])[r2+k] = temp;

				Printf2( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
			}

			brow1++;
			brow2++;
		}

		// Last block
		for( k = 0 ; k < n % A->blockSize ; k++ )
		{
//				printf( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
//				printf( "brow1=%u, brow2=%u, r1=%u, r2=%u\n", brow1, brow2, r1, r2 );

			temp = ((float*)A->blockData[brow1])[r1+k];
			((float*)A->blockData[brow1])[r1+k] = ((float*)A->blockData[brow2])[r2+k];
			((float*)A->blockData[brow2])[r2+k] = temp;

			Printf2( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
		}

		k1x++;
	}
}

void slaswp_blocked( unsigned int nb, PyArrayObject *A, unsigned int offsetb, unsigned int yb, unsigned int *ipiv )
{
	unsigned int brow1 = yb*A->numberOfBlocksXDim+offsetb, brow2;
	unsigned int r1, r2;

	unsigned int i, j, k, ip;

	float temp;

	// For each row in the block
	for( i = yb*A->blockSize ; i < (yb+1)*A->blockSize ; i++ )
	{

		ip = ipiv[i];

		Printf2( "I=%u, ip=%u\n", i, ip );

		r1 = ( i % A->blockSize ) * A->blockSize;
		r2 = ( ip % A->blockSize ) * A->blockSize;

		brow2 = ip / A->blockSize * A->numberOfBlocksXDim + offsetb;

		// For each block row
		for( j = 0 ; j < nb ; j++ )
		{
			Printf2( "J=%u\n", j );
			for( k = 0 ; k < A->blockSize ; k++ )
			{
				Printf2( "ROW1: b=%u, c=%u, val=%f\n", brow1+j, r1+k, ((float*)A->blockData[brow1+j])[r1+k] );
				Printf2( "ROW1: b=%u, c=%u, val=%f\n", brow2+j, r2+k, ((float*)A->blockData[brow2+j])[r2+k] );
				temp = ((float*)A->blockData[brow1+j])[r1+k];
				((float*)A->blockData[brow1+j])[r1+k] = ((float*)A->blockData[brow2+j])[r2+k];
				((float*)A->blockData[brow2+j])[r2+k] = temp;
			}
		}
	}
}
