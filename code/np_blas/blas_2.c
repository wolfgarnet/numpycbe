//#include "../../../code/dispatcher/dispatcher/common.h"
#include "../common/common.h"
//#include "../../../code/dispatcher/dispatcher/common_spu.h"
#include "../common/common_spu.h"

//#include <fmaf4.h>

#define _GET_NUM_BLOCKS 256

// Tags
#define _ATAG 0
#define _XTAG 2
#define _YTAG 4
#define _BTAG 6

unsigned int _GetIndex( unsigned int x, unsigned int y, unsigned int numcols )
{
     return ( x + y * numcols );
}

void _GetAsync( unsigned int ea, unsigned int size, void *buffer, unsigned int tag )
{
	while( size > __PROG_GET_MAX_SIZE )
	{
		spu_mfcdma32( buffer, ea, __PROG_GET_MAX_SIZE, tag, MFC_GET_CMD );
		buffer += __PROG_GET_MAX_SIZE;
		ea     += __PROG_GET_MAX_SIZE;
		size   -= __PROG_GET_MAX_SIZE;
	}

	if( size == 0 )
	{
		return;
	}
	size = ( size + 15 ) &~ 15;
	spu_mfcdma32( buffer, ea, size, tag, MFC_GET_CMD );
}

void _PutAsync( unsigned int ea, unsigned int size, void *buffer, unsigned int tag )
{
	while( size > __PROG_GET_MAX_SIZE )
	{
		spu_mfcdma32( buffer, ea, __PROG_GET_MAX_SIZE, tag, MFC_PUT_CMD );
		buffer += __PROG_GET_MAX_SIZE;
		ea     += __PROG_GET_MAX_SIZE;
		size   -= __PROG_GET_MAX_SIZE;
	}

	if( size == 0 )
	{
		return;
	}
	size = ( size + 15 ) &~ 15;
	spu_mfcdma32( buffer, ea, size, tag, MFC_PUT_CMD );
}

void _Wait( unsigned int tag )
{
	spu_writech( MFC_WrTagMask, 1 << tag );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void _GetPyArrayMetaData( PyArrayObject *ea, void *mem, unsigned int tag )
{
	spu_mfcdma32( mem, (unsigned int)ea, sizeof( PyArrayObject ), tag, MFC_GET_CMD );
}

// atidx and numidxs must be divisable by four!
void _GetBlocksArrayPart2( PyArrayObject *pyobj, char **mem, unsigned int atidx, unsigned int numidxs, unsigned int tag )
{
	//size = ( size + 15 ) &~ 15;
	spu_mfcdma32( mem, ((unsigned int)pyobj->blockData)+(atidx*4), ((numidxs + 3 ) &~ 3 ) * 4, tag, MFC_GET_CMD );
}

void _GetBlocksArrayPart( PyArrayObject *pyobj, char **mem, unsigned int row, unsigned int xpad, unsigned int tag )
{
	//size = ( size + 15 ) &~ 15;
	spu_mfcdma32( mem, ((unsigned int)pyobj->blockData)+((row*xpad)*4), xpad * 4, tag, MFC_GET_CMD );
}

void _GetScalar( PyArrayObject *pyobj, char **mem, float *var, unsigned int tag )
{
	spu_mfcdma32( mem, (unsigned int)(pyobj->blockData), 4, tag, MFC_GET_CMD );
	spu_writech( MFC_WrTagMask, 1 << tag );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );

	spu_mfcdma32( var, (unsigned int)mem[0], 4, tag, MFC_GET_CMD );
	spu_writech( MFC_WrTagMask, 1 << tag );
	spu_mfcstat( MFC_TAG_UPDATE_ALL );
}

void _GetBlock( char **blocks, char *block, unsigned int blockid, unsigned int numelements, unsigned int tag )
{
	_GetAsync( (unsigned int)blocks[blockid], 4 * numelements, block, tag );
	//_Wait(tag);
//     spu_mfcdma32( block, (unsigned int)blocks[blockid], 4 * numelements, tag, MFC_GET_CMD );
}

//void _GetBlockRow( char **blocks, char *block, unsigned int blockid, unsigned int row, unsigned int numelements, unsigned int tag )
//{
//	//_GetAsync( (unsigned int)blocks[blockid], 4 * numelements, block, tag );
//	spu_mfcdma32( mem, (unsigned int)(pyobj->blockData), 4, tag, MFC_GET_CMD );
//}

unsigned int _FindPadding( PyArrayObject *pyobj )
{
	return pyobj->blockSize - pyobj->paddingx;
}

/*
 * Single Precision General Matrix Vector Multiplication
 *
 * Trans = 'n'
 * x has the same number of rows as A has in columns
 * y has the same number of columns as A has in rows
 */

void sgemv( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	// Make room for python meta data
	PyArrayObject *A     = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *x     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *y     = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*2;
	PyArrayObject *alpha = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*3;
	PyArrayObject *beta  = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject )*4;

	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], A, 0 );
	_GetPyArrayMetaData( myop->obj[1], x, 0 );
	_GetPyArrayMetaData( myop->obj[2], y, 0 );
	_GetPyArrayMetaData( myop->scalar[0], alpha, 0 );
	_GetPyArrayMetaData( myop->scalar[1], beta, 0 );
	_Wait(0); // WAIT!!!

	unsigned int xpad = ( A->numberOfBlocksXDim + 31 ) &~ 31;

	// Make room for data block arrays
	char **blocks_A     = (char**)( ( (unsigned int)( beta + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_x     = (char**)( blocks_A     + 4 * xpad );
	char **blocks_y     = (char**)( blocks_x     + 4 * xpad );
	char **blocks_alpha = (char**)( blocks_y     + 4 * xpad );
	char **blocks_beta  = (char**)( blocks_alpha + 4 );

	// Intermediate results
	vector float *ires = (vector float*)( blocks_beta + 4 );
	// Final results
	float *res         = (float*)( ires + 16 * A->blockSize );

	vector float *row1, *row2, *row3;

	// Local vars
	unsigned int idx = 0, nextidx;
	unsigned int xidx = 0, xnextidx;
	unsigned int i, j, row, col, element;
	unsigned int nextblc = _GET_NUM_BLOCKS;
	unsigned int blc = 0;

	unsigned int c = 0;

	unsigned int Afourth  = A->blockSize / 4;
	unsigned int xfourth  = x->blockSize / 4;
	unsigned int AAfourth = A->blockSize*A->blockSize / 4;
	unsigned int t = 0;

	// Uploadet scalars
	float alpha_f, beta_f;
	_GetScalar( alpha, blocks_alpha, &alpha_f, 1 );
	_GetScalar( beta, blocks_beta, &beta_f, 1 );
	vector float alphav = spu_splats(alpha_f);
	vector float betav = spu_splats(beta_f);



	// Fetch pointers to first __ datablocks for vectors
	_GetBlocksArrayPart( x, blocks_x, 0, xpad, _BTAG );
	_GetBlocksArrayPart( y, blocks_y, 0, xpad, _BTAG );

	// Foreach block row assigned to the SPE
	for( i = SPE_id ; i < A->numberOfBlocksYDim ; i += myop->num_SPES )
	{
		idx  = 0;
		xidx = 0;
		// Fetch next batch of block pointers
		_GetBlocksArrayPart( A, blocks_A, i, xpad, _BTAG );
		// Reset intermediate results
		for( row = 0 ; row < A->blockSize ; row++ )
		{
			ires[row] = spu_splats(0.0f);
			//res[row] = 0.0f;
		}

		// Wait for the block arrays to transfer
		_Wait( _BTAG );

		// Get the first part of x
		_GetBlock( blocks_x, info->LS_blockDataArea[1+xidx*3], 0, x->blockSize*x->blockSize, xidx );


		// Get the first block from the matrix A
		_GetBlock( blocks_A, info->LS_blockDataArea[0+idx*3], 0, A->blockSize*A->blockSize, idx );

		// Foreach block column for the specific block row
		for( j = 0 ; j < A->numberOfBlocksXDim-1 ; j++ )
		{
			nextidx = idx ^ 1;

			//row2 = (vector float*)( info->LS_blockDataArea[1+xidx*3] + ( j % x->blockSize ) * x->blockSize * 4 );

			// Do we need another block from the vector x?
			if( j % x->blockSize == 0 )
			{
				row2 = (vector float*)( info->LS_blockDataArea[1+xidx*3] );
				xnextidx = xidx ^ 1;
				// _GetBlock( blocks_x, info->LS_blockDataArea[1], j / x->blockSize, x->blockSize*x->blockSize, 2 );
				// The the next part of x
				funcs->printuint(1234);
				funcs->printuint((j+x->blockSize) / x->blockSize);
				_GetBlock( blocks_x, info->LS_blockDataArea[1+xnextidx*3], (j+x->blockSize) / x->blockSize, x->blockSize*x->blockSize, xnextidx );
				_Wait( xidx );
				funcs->printuint(1234);
				xidx = xnextidx;
			}

			_GetBlock( blocks_A, info->LS_blockDataArea[0+nextidx*3], j+1, A->blockSize*A->blockSize, nextidx );


			row2 += j * x->blockSize * 4;

//			// Preprocess x
//			for( col = 0 ; col < Afourth ; col++ )
//			{
//				row2[col] = spu_mul( row2[col], alphav );
//			}
//
			_Wait( idx );
//
//			// Foreach element in the block
//			for( row = 0 ; row < A->blockSize ; row++ )
//			{
//				row1 = (vector float*)( info->LS_blockDataArea[0+idx*3] + row*A->blockSize*4 );
//
//				for( col = 0 ; col < Afourth ; col++ )
//				{
//					ires[row] += spu_mul( row1[col], row2[col] );
//				}
//			}

			idx = nextidx;
		}


		// Finish the block row
		// Find potential padding.... Not done yet...

		// Do we need another block from the vector x?
//		if( j % x->blockSize == 0 )
//		{
//			_GetBlock( blocks_x, info->LS_blockDataArea[1], j / x->blockSize, x->blockSize*x->blockSize, 2 );
//		}
		if( j % x->blockSize == 0 )
		{
			row2 = (vector float*)( info->LS_blockDataArea[1+xidx*3] );
		}

		//row2 = (vector float*)( info->LS_blockDataArea[1] + ( ( j % x->blockSize ) * x->blockSize * 4 ) );
		row2 += j * x->blockSize * 4;

		// Do we need another block from the vector y?
		if( i % y->blockSize == 0 )
		{
			_GetBlock( blocks_y, info->LS_blockDataArea[2], i / y->blockSize, y->blockSize*y->blockSize, 0 );
		}
//
//		// Preprocess x
//		for( col = 0 ; col < Afourth ; col++ )
//		{
//			row2[col] = spu_mul( row2[col], alphav );
//		}
//
//		_Wait( 0 );

		// The % can be optimized if the number of blocks is equal to the number of columns!!!

//		float *yy = (float*)( info->LS_blockDataArea[2] + ( i % y->blockSize ) * y->blockSize * 4 );
//
//		for( row = 0 ; row < A->blockSize ; row++ )
//		{
//			row1 = (vector float*)( info->LS_blockDataArea[0+idx*3] + row*A->blockSize*4 );
//
//			for( col = 0 ; col < Afourth ; col++ )
//			{
//				ires[row] += spu_mul( row1[col], row2[col] );
//			}
//
//			yy[row] = spu_extract(ires[row],0) + spu_extract(ires[row],1) + spu_extract(ires[row],2) + spu_extract(ires[row],3) + yy[row] * beta_f;
//		}

		// _PutAsync( unsigned int ea, unsigned int size, void *buffer, unsigned int tag )
//		_PutAsync( (unsigned int)blocks_y[i/y->blockSize]+((i%y->blockSize)*y->blockSize*4), y->blockSize * 4, yy, idx );

		//_Wait( idx );
	}

}
