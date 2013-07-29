/*
 * fhbtool.c
 *
 *  Created on: Apr 7, 2009
 *      Author: wolle
 */

#include <stdio.h>
#include "fhbtool.h"
#include <malloc_align.h>
#include <free_align.h>




/*
 * Misc
 */



void _PrintFloatBool( unsigned int size, void *value )
{
	// Ugly as hell:
	if( size == 1 )
	{
		// Bool
		printf( "%u  ", *(char*)value );
	}
	else
	{
		// Float32
		printf( "%.02f  ", *(float*)value );
	}
}

/*
 * Prints
 */


void PrintNDArray( PyArrayObject *pyobj1, unsigned int flags )
{
	//if( pyobj1->dimensions[0] == 1 || pyobj1->dimensions[1] == 1 )
	if( pyobj1->nd == 1 )
	{
		PrintVector2( pyobj1, flags );
	}
	else
	{
		PrintMatrix2( pyobj1, flags );
	}
}

void PrintVector( PyArrayObject *pyobj1, unsigned int flags )
{
	printf( "---------- PRINT VECTOR -----------\n" );
	printf( "Block dimensions: %u, %u\n", pyobj1->numberOfBlocksYDim, pyobj1->numberOfBlocksXDim );
	printf( "Number of blocks: %u\n", pyobj1->numberOfBlocks );
	printf( "Element size: %u\n", pyobj1->size_in_bytes );
	printf( "Block data address %#x\n", (unsigned int)pyobj1->blockData );
	printf( "Padding: %u, %u\n", pyobj1->paddingy, pyobj1->paddingx  );

	unsigned int col, i;
	//unsigned int pa_mask = 1;

	void *p;

	for( col = 0 ; col < pyobj1->numberOfBlocksXDim ; col++ )
	{
		printf( "---------------------\n" );
		printf( "Block col=%u\n", col );

		p = pyobj1->blockData[col];

		if ( flags & _PA_FLAG )
		{
			printf( "Block address %#x)\n", (unsigned int)pyobj1->blockData[col] );
		}

		for( i = 0 ; i < pyobj1->blockSize*pyobj1->blockSize ; i++ )
		{
			if ( col == pyobj1->numberOfBlocksXDim - 1 && i >= pyobj1->blockSize*pyobj1->blockSize - pyobj1->paddingx )
			{
				continue;
			}
			//_PrintFloatBool( pyobj1->size_in_bytes, &((float*)pyobj1->blockData[col])[i] );
			_PrintFloatBool( pyobj1->size_in_bytes, p );
			p += pyobj1->size_in_bytes;
		}

		printf( "\n" );
	}
}


void PrintMatrix( PyArrayObject *pyobj1, unsigned int flags )
{
	printf( "Block dimensions: %u, %u\n", pyobj1->numberOfBlocksYDim, pyobj1->numberOfBlocksXDim );
	printf( "Number of blocks: %u\n", pyobj1->numberOfBlocks );
	printf( "Element size: %u\n", pyobj1->size_in_bytes );
	printf( "Block data address %#x\n", (unsigned int)pyobj1->blockData );
	printf( "Padding: %u, %u\n", pyobj1->paddingy, pyobj1->paddingx  );

	unsigned int row, col, i, j;
	char *c;

	//unsigned int pa_mask = 1;

	for( row = 0 ; row < pyobj1->numberOfBlocksYDim ; row++ )
	{
		for( col = 0 ; col < pyobj1->numberOfBlocksXDim ; col++ )
		{
			printf( "---------------------\n" );
			printf( "Block col=%u, row=%u(%u)\n", col, row, row );
			if ( flags & _PA_FLAG )
			{
				printf( "Block col=%u, row=%u(%#x)\n", col, row, (unsigned int)pyobj1->blockData[row*pyobj1->numberOfBlocksXDim+col] );
			}

			for( i = 0 ; i < pyobj1->blockSize ; i++ )
			{
				c = pyobj1->blockData[row*pyobj1->numberOfBlocksXDim+col] + i * pyobj1->blockSize * pyobj1->size_in_bytes;

				for( j = 0 ; j < pyobj1->blockSize ; j++ )
				{
					// Last column
					if ( col == pyobj1->numberOfBlocksXDim - 1 )
					{
						if ( j >= pyobj1->blockSize - pyobj1->paddingx )
						{
							continue;
						}
					}
					// Last row
					if ( row == pyobj1->numberOfBlocksYDim - 1 )
					{
						if ( i >= pyobj1->blockSize - pyobj1->paddingy )
						{
							i = pyobj1->blockSize;
							continue;
						}
					}

					_PrintFloatBool( pyobj1->size_in_bytes, c );
					c += pyobj1->size_in_bytes;

				}
				printf( "\n" );
			}

			printf( "\n" );
		}

	}
}

void PrintMatrix2( PyArrayObject *pyobj1, unsigned int flags )
{
	Printf1( "Block dimensions: %u, %u\n", pyobj1->numberOfBlocksYDim, pyobj1->numberOfBlocksXDim );
	Printf1( "Number of blocks: %u\n", pyobj1->numberOfBlocks );
	Printf1( "Element size: %u\n", pyobj1->size_in_bytes );
	Printf1( "Block data address %#x\n", (unsigned int)pyobj1->blockData );
	Printf1( "Padding: %u, %u\n", pyobj1->paddingy, pyobj1->paddingx  );

	unsigned int row, col, i, j, rb, cb, r, c;

	//unsigned int pa_mask = 1;

	for( row = 0 ; row < pyobj1->numberOfBlocksYDim * pyobj1->blockSize - pyobj1->paddingy ; row++ )
	{
		printf( "Row %u:\t [ ", row );
		for( col = 0 ; col < pyobj1->numberOfBlocksXDim * pyobj1->blockSize - pyobj1->paddingx ; col++ )
		{
			rb = row / pyobj1->blockSize*pyobj1->numberOfBlocksXDim;
			cb = col / pyobj1->blockSize;
			r  = row % pyobj1->blockSize * pyobj1->blockSize;
			c  = col % pyobj1->blockSize;
			if( flags & _SCINOTATION_FLAG )
			{
				if( flags & _HIGHPRECISION_FLAG )
				{
					printf( "%.30e\n", ((float*)pyobj1->blockData[rb+cb])[r+c] );
				}
				else
				{
					printf( "%e ", ((float*)pyobj1->blockData[rb+cb])[r+c] );
				}
			}
			else
			{
				printf( "%6.2f ", ((float*)pyobj1->blockData[rb+cb])[r+c] );
			}
		}

		printf( " ]\n\n" );
	}
}

void PrintVector2( PyArrayObject *pyobj1, unsigned int flags )
{
	Printf1( "Block dimensions: %u, %u\n", pyobj1->numberOfBlocksYDim, pyobj1->numberOfBlocksXDim );
	Printf1( "Number of blocks: %u\n", pyobj1->numberOfBlocks );
	Printf1( "Element size: %u\n", pyobj1->size_in_bytes );
	Printf1( "Block data address %#x\n", (unsigned int)pyobj1->blockData );
	Printf1( "Padding: %u, %u\n", pyobj1->paddingy, pyobj1->paddingx  );

	unsigned int block, elem;
	unsigned int max = 20, counter = 0;

	//unsigned int pa_mask = 1;

	printf( "[" );

	for( block = 0 ; block < pyobj1->numberOfBlocks-1 ; block++ )
	{
		for( elem = 0 ; elem < pyobj1->blockSize * pyobj1->blockSize ; elem++ )
		{


			printf( "%6.2f ", ((float*)pyobj1->blockData[block])[elem] );
			if ( counter % max == max-1 )
			{
				printf( "\n " );
			}
			counter++;
		}
	}

	for( elem = 0 ; elem < pyobj1->blockSize * pyobj1->blockSize - pyobj1->paddingx ; elem++ )
	{


		printf( "%6.2f ", ((float*)pyobj1->blockData[block])[elem] );

		if ( counter % max == max-1 )
		{
			printf( "\n " );
		}

		counter++;
	}

	printf( " ] \n" );
}


/*
 * Fill FHB
 */

void FHBFillScalarUInt( PyArrayObject *obj, unsigned int value )
{
	((unsigned int*)obj->blockData[0])[0] = value;
}

void FHBFillVector( PyArrayObject *pyobj1, unsigned int esize, void *value )
{
	unsigned int col, i;
	char *c;
	for( col = 0 ; col < pyobj1->numberOfBlocksXDim ; col++ )
	{
		c = pyobj1->blockData[col];
		for( i = 0 ; i < pyobj1->blockSize*pyobj1->blockSize ; i++ )
		{
			if ( col == pyobj1->numberOfBlocksXDim - 1 && i >= pyobj1->blockSize*pyobj1->blockSize - pyobj1->paddingx )
			{
				continue;
			}
			memcpy( c, value, esize );
			c += esize;
		}
	}
}

void FHBFillVectorRange_float( PyArrayObject *pyobj1, float from, float steps )
{
	unsigned int col, i;
	char *c;
	for( col = 0 ; col < pyobj1->numberOfBlocksXDim ; col++ )
	{
		c = pyobj1->blockData[col];
		for( i = 0 ; i < pyobj1->blockSize*pyobj1->blockSize ; i++ )
		{
			if ( col == pyobj1->numberOfBlocksXDim - 1 && i >= pyobj1->blockSize*pyobj1->blockSize - pyobj1->paddingx )
			{
				continue;
			}
//			memcpy( c, value, esize );
//			c += esize;
			((float*)pyobj1->blockData[col])[i] = from;
			from += steps;
		}
	}
}

void FHBFillMatrix( PyArrayObject *pyobj1, unsigned int esize, void *value )
{
	unsigned int row, col, i, j;
	char *c;

	for( row = 0 ; row < pyobj1->numberOfBlocksYDim ; row++ )
	{
		for( col = 0 ; col < pyobj1->numberOfBlocksXDim ; col++ )
		{
			for( i = 0 ; i < pyobj1->blockSize ; i++ )
			{
				c = pyobj1->blockData[row*pyobj1->numberOfBlocksXDim+col] + i * pyobj1->blockSize * pyobj1->size_in_bytes;
				for( j = 0 ; j < pyobj1->blockSize ; j++ )
				{
					// Last column
					if ( col == pyobj1->numberOfBlocksXDim - 1 )
					{
						if ( j >= pyobj1->blockSize - pyobj1->paddingx )
						{
							continue;
						}
					}
					// Last row
					if ( row == pyobj1->numberOfBlocksYDim - 1 )
					{
						if ( i >= pyobj1->blockSize - pyobj1->paddingy )
						{
							i = pyobj1->blockSize;
							continue;
						}
					}
					//((unsigned int*)pyobj1->blockData[row*pyobj1->numberOfBlocksXDim+col])[i*pyobj1->blockSize+j] = *(unsigned int*)value;
					//memcpy( &pyobj1->blockData[row*pyobj1->numberOfBlocksXDim+col][i*pyobj1->blockSize+j], value, esize );
					// memcpy( &((unsigned int *)pyobj1->blockData[row*pyobj1->numberOfBlocksXDim+col])[i*pyobj1->blockSize+j], value, esize );
					memcpy( c, value, esize );
					c += esize;
					//printf( "(%u,%u) ", i, j );
					//memcpy( c, value, esize );
					//c += esize;
				}
			}
		}

	}
}

void FHBFillRow( PyArrayObject *pyobj1, unsigned int row, unsigned int esize, void *value )
{
	unsigned int col, i, colstart = row / pyobj1->blockSize * pyobj1->numberOfBlocksXDim;;
	char *c;
	//printf( "colstart=%u, row=%u\n", colstart, row );
	for( col = colstart ; col < pyobj1->numberOfBlocksXDim+colstart ; col++ )
	{
		c = pyobj1->blockData[col] + ( row % pyobj1->blockSize * pyobj1->blockSize * esize );
		for( i = 0 ; i < pyobj1->blockSize ; i++ )
		{
			if ( col % ( pyobj1->numberOfBlocksXDim - 1 ) == 0 && i >= pyobj1->blockSize*pyobj1->numberOfBlocksXDim - pyobj1->paddingx )
			{
				continue;
			}
			memcpy( c, value, esize );
			c += esize;
		}
	}
}

/*
 *  CREATE FHB
 */

void *sMakeBlockVectorPAD( unsigned int blocksize, unsigned int pad, unsigned int esize )
{
	unsigned int bb = blocksize * blocksize;
	//void *block = (void *)memalign( 16, ( 4 * elements + 15 ) & ~15 );
	void *block = (void*)_malloc_align( bb*esize, 7 );
	//printf( "SIZE=%u, pad=%u\n", bb*4, pad );
	char *p = block;
	unsigned int i;
	for( i = 0 ; i < bb ; i++ )
	{
		if ( i < bb-pad )
		{
			//printf( "%u=%f, ", i, value );
			//p[i] = 0.0f;
			*p = 0;
		}
		else
		{
			//p[i] = 0.0f;
			*p = 0;
		}
		p += esize;
	}

	return block;
}

void *sMakeBlockVectorPAD_noinit( unsigned int blocksize, unsigned int esize )
{
	unsigned int bb = blocksize * blocksize;
	void *block = (void*)_malloc_align( bb*esize, 7 );

	return block;
}

void *sMakeBlockMatrixPAD( unsigned int blocksize, unsigned int esize )
{
	printf( "ESIZE=%u,%u\n", esize, blocksize );
	unsigned int bb = blocksize * blocksize;
	//void *block = (void *)memalign( 16, ( 4 * elements + 15 ) & ~15 );
	void *block = (void*)_malloc_align( bb*esize, 7 );

	char *p = block;

	unsigned int i;
	for( i = 0 ; i < bb ; i++ )
	{
		*p = 0;
		p += esize;
	}

	return block;
}



void _MakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, PyArrayObject *obj, unsigned int esize, unsigned int create, unsigned int oneblock )
{
	unsigned int xblocks, yblocks;
	unsigned int paddingy = 0, paddingx = 0;
	int ismatrix = 0;
	unsigned int nd = 0, dims[2];

	// Scalar
	if( m == 1 && n == 1 )
	{
		nd = 0;
		xblocks = 1;
		yblocks = 1;
		paddingx = blocksize - 1;
		paddingy = blocksize - 1;
	}
	// Vector
	else if ( m == 1 || n == 1 )
	{
		unsigned int max = n * m;
		unsigned int bb = blocksize * blocksize;
		xblocks = max / bb;

		//printf( "xblocks=%u\n", xblocks );
		if( max % bb != 0 )
		{
			xblocks++;
			//printf( "PADDING: b*b=%u, m*n=%u, m*n mod b*b=%u\n", bb, max, max % bb );
			paddingx = bb - max % bb;
		}
		yblocks = 1;
		//printf( "TEST: blocks(%u,%u), paddingx=%u\n", yblocks, xblocks, paddingx );
		nd = 1;
		dims[0] = m;
		if( n > m )
		{
			dims[0] = n;
		}
	}
	else
	{
		xblocks = n / blocksize;
		if( n % blocksize != 0 )
		{
			xblocks++;
			paddingx = blocksize - n % blocksize;
		}
		yblocks = m / blocksize;
		if( m % blocksize != 0 )
		{
			yblocks++;
			paddingy = blocksize - m % blocksize;
		}
		ismatrix = 1;
		nd = 2;
		dims[0] = m;
		dims[1] = n;
	}

	Printf1( "DIM: m=%u, n=%u\n", m, n );
	Printf1( "BLOCKS: x=%u, y=%u\n", xblocks, yblocks );

	//nn = ( xblocks + 31 ) &~ 31;
	//printf( "n=%u, nn=%u\n", xblocks, nn );
	obj->blockData = (char **)_malloc_align( 4 * yblocks * xblocks, 7 );
	obj->dimensions = (unsigned int*)_malloc_align( nd * 4, 4 );

	obj->nd                 = nd;
	obj->numberOfBlocks     = xblocks * yblocks;
	obj->numberOfBlocksXDim = xblocks;
	obj->numberOfBlocksYDim = yblocks;
	obj->paddingx           = paddingx;
	obj->paddingy           = paddingy;
	obj->size_in_bytes      = esize;
	obj->blockSize          = blocksize;

	unsigned int i, j;

	// Set dimensions
	// Highest dimension first!!!
	for( i = 0 ; i < nd ; i++ )
	{
		obj->dimensions[i] = dims[i];
	}

	char *ONEBLOCK;

	if( oneblock )
	{
		ONEBLOCK = sMakeBlockVectorPAD( blocksize, 0, esize );
	}

	for( i = 0 ; i < yblocks ; i++ )
	{
		for( j = 0 ; j < xblocks ; j++ )
		{
			if( create )
			{
				if( oneblock )
				{
					obj->blockData[i*xblocks+j] = ONEBLOCK;
				}
				else
				{
					obj->blockData[i*xblocks+j] = sMakeBlockVectorPAD( blocksize, 0, esize );
				}
			}
			else
			{
				obj->blockData[i*xblocks+j] = sMakeBlockVectorPAD_noinit( blocksize, esize );
			}
		}
	}

}

void MakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, PyArrayObject *obj, unsigned int esize )
{
	_MakeFHB( m, n, blocksize, obj, esize, 1, 0 );
}

void MakeFHB_noinit( unsigned int m, unsigned int n, unsigned int blocksize, PyArrayObject *obj, unsigned int esize )
{
	//Printf( "MAKEFHB NO INIT\n" );
	_MakeFHB( m, n, blocksize, obj, esize, 0, 0 );
}

void MakeFHB_oneblock( unsigned int m, unsigned int n, unsigned int blocksize, PyArrayObject *obj, unsigned int esize )
{
	_MakeFHB( m, n, blocksize, obj, esize, 1, 1 );
}

void sMakeFHB_noinit( unsigned int m, unsigned int n, unsigned int blocksize, PyArrayObject *obj )
{
	MakeFHB_noinit( m, n, blocksize, obj, 4 );
}

void sMakeFHB_shape( PyArrayObject *shape, PyArrayObject *dest )
{
	//Printf1( "m=%u, n=%u, blocksize=%u\n", shape->dimensions[0], shape->dimensions[1], shape->blockSize );
	if( shape->nd == 1 )
	{
		MakeFHB_noinit( shape->dimensions[0], 1, shape->blockSize, dest, 4 );
	}
	else
	{
		MakeFHB_noinit( shape->dimensions[0], shape->dimensions[1], shape->blockSize, dest, 4 );
	}
}

void sMakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, float value, PyArrayObject *obj )
{
	MakeFHB( m, n, blocksize, obj, 4 );
	if( m == 1 || n == 1 )
	{
		FHBFillVector( obj, 4, &value );
	}
	else
	{
		FHBFillMatrix( obj, 4, &value );
	}
}

void sMakeFHB_arange( unsigned int blocksize, float from, float to, float steps, PyArrayObject *obj )
{
	unsigned int n = ( to - from ) / steps;
	MakeFHB( 1, n, blocksize, obj, 4 );
	FHBFillVectorRange_float( obj, from, steps );
}

void sMakeFHB_oneblock( unsigned int m, unsigned int n, unsigned int blocksize, float value, PyArrayObject *obj )
{
	MakeFHB_oneblock( m, n, blocksize, obj, 4 );
	if( m == 1 || n == 1 )
	{
		FHBFillVector( obj, 4, &value );
	}
	else
	{
		FHBFillMatrix( obj, 4, &value );
	}
}

void uMakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, unsigned int value, PyArrayObject *obj )
{
	MakeFHB( m, n, blocksize, obj, 4 );
	if( m == 1 && n == 1 )
	{
		FHBFillScalarUInt( obj, value );
	}
}

void bMakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, int value, PyArrayObject *obj )
{
	MakeFHB( m, n, blocksize, obj, 1 );
	char v = 0;
	if( value > 0 )
	{
		v = _TRUE;
	}
	printf( "VALUE=%c - %i - %#x\n", v, v, v );
	if( m == 1 || n == 1 )
	{
		FHBFillVector( obj, 1, &v );
	}
	else
	{
		FHBFillMatrix( obj, 1, &v );
	}
}


void sMakeScalar( float value, PyArrayObject *obj )
{
	obj->blockData = (char **)_malloc_align( 4, 4 );
	obj->numberOfBlocks     = 1;
	obj->numberOfBlocksXDim = 1;
	obj->numberOfBlocksYDim = 1;
	obj->paddingx           = 0;
	obj->size_in_bytes      = 4;
	obj->blockSize          = 1;
	obj->blockData[0]       = sMakeBlock( 1, value );
}


void DeleteObject( PyArrayObject *obj )
{
	unsigned int i;
	for( i = 0 ; i < obj->numberOfBlocks ; i++ )
	{
		_free_align( obj->blockData[i] );
	}
	_free_align( obj->blockData );
	_free_align( obj->dimensions );
}


void *sMakeBlock( unsigned int elements, float value )
{
	//void *block = (void *)memalign( 16, ( 4 * elements + 15 ) & ~15 );
	void *block = (void*)_malloc_align( 4 * elements, 7 );
	void *p = block;
	unsigned int i;
	for( i = 0 ; i < elements ; i++ )
	{
		//*(float*)p = i;
		*(float*)p = value;
		//printf( "%f, ", *(float*)p );
		p += 4;
	}
	return block;
}

void sMakeMatrix( unsigned int x, unsigned int y, unsigned int blocksize, float value, PyArrayObject *obj )
{
	//obj->blockData = (char **)memalign( 16, ( 4 * x * y + 15 ) & ~15 );

	unsigned int yy, xx;
	// Column vector
	if ( x == 1 )
	{
		yy = ( y + 31 ) &~ 31;
		printf( "y=%u, yy=%u\n", y, yy );
		obj->blockData = (char **)_malloc_align( 4 * x * yy, 7 );
	}
	// Row vector or matrix
	else
	{
		xx = ( x + 31 ) &~ 31;
		printf( "x=%u, xx=%u\n", x, xx );
		obj->blockData = (char **)_malloc_align( 4 * xx * y, 7 );
	}

	//printf( "TEST=%u\n", ( 4 * x * y + 15 ) & ~15 );
	obj->numberOfBlocks     = x * y;
	obj->numberOfBlocksXDim = x;
	obj->numberOfBlocksYDim = y;
	//obj->paddingx           = paddingx;
	obj->size_in_bytes      = 4;
	obj->blockSize          = blocksize;

	// Column vector, make it a row vector
	if ( x == 1 )
	{
		x  = y;
		y  = 1;
		xx = yy;
	}

	unsigned int i,j;
	if ( _CHEAT_LEVEL == 0 )
	{
		for( i = 0 ; i < y ; i++ )
		{
			for( j = 0 ; j < x ; j++ )
			{
				obj->blockData[i*xx+j] = sMakeBlock( blocksize*blocksize, value );
				//printf( "1.=%f\n", *(float*)obj->blockData[i] );
			}
		}
	}
	else
	{
		void *tmp[_CHEAT_LEVEL];

		for( i = 0 ; i < _CHEAT_LEVEL ; i++ )
		{
			tmp[i] = sMakeBlock( blocksize*blocksize, value );
		}

		for( i = 0 ; i < y ; i++ )
		{
			for( j = 0 ; j < x ; j++ )
			{
				obj->blockData[i*xx+j] = tmp[(i*xx+j)%_CHEAT_LEVEL];
				//printf( "1.=%f\n", *(float*)obj->blockData[i] );
			}
		}
	}
}

/*
 * CREATION
 */

void MakeVectorFromMatrixCol( PyArrayObject *A, PyArrayObject *x, unsigned int col )
{
	unsigned int i, j;

	unsigned int c_offset = col / A->blockSize;
	unsigned int c        = col % A->blockSize;

	// For each row
	for( i = 0 ; i < A->numberOfBlocksYDim ; i++ )
	{
		// For each row in a block
		for( j = 0 ; j < A->blockSize ; j++ )
		{
			//printf( "%u,%u: [%u][%u]=[%u][%u]\n", i, j, i/x->blockSize, i*x->blockSize+j, i*A->numberOfBlocksXDim+c_offset, j*A->blockSize+c );
			((float*)x->blockData[i/x->blockSize])[(i*x->blockSize+j)%(x->blockSize*x->blockSize)] =
				((float*)A->blockData[i*A->numberOfBlocksXDim+c_offset])[j*A->blockSize+c];
		}
	}
}

/*
 * EXTRA
 */

void PrintVectorData( PyArrayObject *pyobj1 )
{
	unsigned int col, i;

	unsigned int *c;

	for( col = 0 ; col < pyobj1->numberOfBlocks ; col++ )
	{
		printf( "---------------------\n" );
		printf( "Block col=%u\n", col );

		c = (unsigned int)pyobj1->blockData[col];

		for( i = 0 ; i < pyobj1->blockSize*pyobj1->blockSize*pyobj1->size_in_bytes ; i += 4 )
		{
			printf( "(%#x) ", *c );
			c++;
		}

		printf( "\n" );
	}
}

void PrintBlocks( PyArrayObject *pyobj1 )
{
	unsigned int i, j;
	//unsigned int xpad = ( pyobj1->numberOfBlocksXDim + 31 ) &~ 31;
	printf( "%#x : \n", (unsigned int)pyobj1->blockData );
	for( i = 0 ; i < pyobj1->numberOfBlocksYDim ; i++ )
	{
		printf( "(%u)%#x : ", i, (unsigned int)&pyobj1->blockData[i*pyobj1->numberOfBlocksXDim] );
		for( j = 0 ; j < pyobj1->numberOfBlocksXDim ; j++ )
		{
			printf( "%#x  ", (unsigned int)pyobj1->blockData[i*pyobj1->numberOfBlocksXDim+j] );
		}
		printf( "\n" );
	}
	printf( "\n" );
}
