#include <common.h>

/*
 * SSWAP(PPE)
 *
 * Must be full rows
 */

void FHB_sswap_row( PyArrayObject *A, unsigned int row1, unsigned int row2 )
{
	unsigned int j, k;

	float temp;

	unsigned int brow1 = row1 / A->blockSize * A->numberOfBlocksXDim;
	unsigned int brow2 = row2 / A->blockSize * A->numberOfBlocksXDim;
	unsigned int r1    = row1 % A->blockSize * A->blockSize;
	unsigned int r2    = row2 % A->blockSize * A->blockSize;

	// Exchange row k1x with ip
	for( j = 0 ; j < A->numberOfBlocksXDim ; j++ )
	{
		for( k = 0 ; k < A->blockSize ; k++ )
		{
			temp = ((float*)A->blockData[brow1])[r1+k];
			((float*)A->blockData[brow1])[r1+k] = ((float*)A->blockData[brow2])[r2+k];
			((float*)A->blockData[brow2])[r2+k] = temp;

			//printf( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
		}

		brow1++;
		brow2++;
	}

}

void FHB_sswap_row_blocked( PyArrayObject *A, unsigned int blockcol, unsigned int row1, unsigned int row2 )
{
	unsigned int j, k;

	float temp;

	unsigned int brow1 = row1 / A->blockSize * A->numberOfBlocksXDim;
	unsigned int brow2 = row2 / A->blockSize * A->numberOfBlocksXDim;
	unsigned int r1    = row1 % A->blockSize * A->blockSize;
	unsigned int r2    = row2 % A->blockSize * A->blockSize;

	for( k = 0 ; k < A->blockSize ; k++ )
	{
		temp = ((float*)A->blockData[brow1])[r1+k];
		((float*)A->blockData[brow1])[r1+k] = ((float*)A->blockData[brow2])[r2+k];
		((float*)A->blockData[brow2])[r2+k] = temp;

		//printf( "K=%u ; 1=%f, 2=%f\n", k, ((float*)A->blockData[brow1])[r1+k], ((float*)A->blockData[brow2])[r2+k] );
	}


}
