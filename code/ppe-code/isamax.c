
#include <common.h>

/*
 * Find the index for the largest element in a row
 */

unsigned int FHB_isamax_row( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A )
{
	unsigned int brow = row / A->blockSize * A->numberOfBlocksXDim;
	unsigned int r    = row % A->blockSize * A->blockSize;
	unsigned int b;
	unsigned int i;
	unsigned int idx = 0;
	float max;

	b = brow + col / A->blockSize;
	max = A->blockData[b][r+col%A->blockSize];

	for( i = col+1 ; i < col + offset ; i++ )
	{
		b = brow + i / A->blockSize;
		if ( A->blockData[b][r+i%A->blockSize] > max )
		{
			max = A->blockData[b][r+i%A->blockSize];
			idx = i;
		}
	}

	return idx;
}

/*
 * Find the index for the largest element in a col
 */

unsigned int FHB_isamax_col( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A )
{
	unsigned int c    = col % A->blockSize;
	unsigned int bcol = col / A->blockSize;
	unsigned int i;
	unsigned int idx = row;
	float max;

	unsigned int b = row / A->blockSize * A->numberOfBlocksXDim + bcol;
	max = ((float*)A->blockData[b])[row%A->blockSize*A->blockSize+c];

	//printf( "i=%u, ", row );

	for( i = row+1 ; i < row + offset ; i++ )
	{
		//printf( "i=%u, ", i );
		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;
		if ( ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] > max )
		{
			max = ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c];
			idx = i;
		}
	}
	//printf( ". i=%u\n", idx );

	return idx;
}


unsigned int FHB_isamax_col2( unsigned int col, unsigned int row, PyArrayObject *A )
{
	unsigned int c    = col % A->blockSize;
	unsigned int bcol = col / A->blockSize;
	unsigned int i;
	unsigned int idx = row;
	float max;

	unsigned int b = row / A->blockSize * A->numberOfBlocksXDim + bcol;
	max = ((float*)A->blockData[b])[row%A->blockSize*A->blockSize+c];

	//printf( "i=>%u(%f), ", row, max );

	for( i = row+1 ; i < A->dimensions[0] ; i++ )
	{
		//printf( "i=%u, ", i );
		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;
		if ( ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] > max )
		{
			max = ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c];
			idx = i;
			//printf( "i=%u(%f), ", row, max );
		}
	}
	//printf( ". i=%u\n", idx );

	return idx;
}


//
//unsigned int FHB_isamax_col_blocked( unsigned int j, unsigned int b, PyArrayObject *A )
//{
//	unsigned int c    = col % A->blockSize;
//	unsigned int bcol = col / A->blockSize;
//	unsigned int i;
//	unsigned int idx = row;
//	float max;
//
//	unsigned int b = row / A->blockSize * A->numberOfBlocksXDim + bcol;
//	max = ((float*)A->blockData[b])[row%A->blockSize*A->blockSize+c];
//
//	//printf( "i=%u, ", row );
//
//	for( i = row+1 ; i < row + offset ; i++ )
//	{
//		//printf( "i=%u, ", i );
//		b = i / A->blockSize * A->numberOfBlocksXDim + bcol;
//		if ( ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c] > max )
//		{
//			max = ((float*)A->blockData[b])[i%A->blockSize*A->blockSize+c];
//			idx = i;
//		}
//	}
//	//printf( ". i=%u\n", idx );
//
//	return idx;
//}
