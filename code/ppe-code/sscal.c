#include <common.h>

/*
 * SSCAL(PPE)
 */

void FHB_sscal_col( unsigned int offset, unsigned int col, unsigned int row, float scale, PyArrayObject *A )
{
	unsigned int c    = col % A->blockSize;
	unsigned int bcol = col / A->blockSize;
	unsigned int i, b;

	for( i = row ; i < row + offset ; i++ )
	{
		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;
		((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] *= scale;

	}
}

void FHB_sscal_col_blocked( unsigned int col, unsigned int row, float scale, PyArrayObject *A )
{
	unsigned int c    = col % A->blockSize;
	unsigned int bcol = col / A->blockSize;
	unsigned int i, b;

	for( i = row ; i < A->dimensions[0] ; i++ )
	{
		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;
		((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] *= scale;

	}
}
