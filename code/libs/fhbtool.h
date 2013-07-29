/*
 * fhbtool.h
 *
 *  Created on: Apr 7, 2009
 *      Author: wolle
 */

#ifndef FHBTOOL_H_
#define FHBTOOL_H_

#include <common.h>
#include <string.h>


#define _TRUE 0xff
#define _FALSE 0x0

#define _CHEAT_LEVEL 0

//#define _MASK_PA 0x1;
enum
{
	_PA_FLAG            = 1,
	_SCINOTATION_FLAG   = 2,
	_HIGHPRECISION_FLAG = 4
} __FLAGS;

/*
 * Index stuff
 */


#define getblockidx( row, col, blocksxdim, blocksize ) (row/blocksize*blocksxdim+col/blocksize)
//#define getidx( col, row, cols ) ((cols)*(row)+(col))
#define getidx( row, col, cols ) ((row%cols)*cols+(col%cols))

void PrintNDArray( PyArrayObject *pyobj1, unsigned int flags );
void PrintVector( PyArrayObject *pyobj1, unsigned int flags );
void PrintMatrix( PyArrayObject *pyobj1, unsigned int flags );

void PrintMatrix2( PyArrayObject *pyobj1, unsigned int flags );
void PrintVector2( PyArrayObject *pyobj1, unsigned int flags );

void FHBFillScalarUInt( PyArrayObject *obj, unsigned int value );
void FHBFillMatrix( PyArrayObject *pyobj1, unsigned int esize, void *value );
void FHBFillVector( PyArrayObject *pyobj1, unsigned int esize, void *value );
void FHBFillVectorRange_float( PyArrayObject *pyobj1, float from, float steps );

void FHBFillRow( PyArrayObject *pyobj1, unsigned int row, unsigned int esize, void *value );

void *sMakeBlockVectorPAD( unsigned int blocksize, unsigned int pad, unsigned int esize );
void *sMakeBlockMatrixPAD( unsigned int blocksize, unsigned int esize );
void sMakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, float value, PyArrayObject *obj );
void bMakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, int value, PyArrayObject *obj );
void uMakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, unsigned int value, PyArrayObject *obj );

void sMakeFHB_noinit( unsigned int m, unsigned int n, unsigned int blocksize, PyArrayObject *obj );
void sMakeFHB_oneblock( unsigned int m, unsigned int n, unsigned int blocksize, float value, PyArrayObject *obj );
void sMakeFHB_arange( unsigned int blocksize, float from, float to, float steps, PyArrayObject *obj );
void sMakeFHB_shape( PyArrayObject *shape, PyArrayObject *dest );

void MakeFHB( unsigned int m, unsigned int n, unsigned int blocksize, PyArrayObject *obj, unsigned int esize );
void MakeFHB_noinit( unsigned int m, unsigned int n, unsigned int blocksize, PyArrayObject *obj, unsigned int esize );

void DeleteObject( PyArrayObject *obj );

void *sMakeBlock( unsigned int elements, float value );
void sMakeMatrix( unsigned int x, unsigned int y, unsigned int blocksize, float value, PyArrayObject *obj );

void MakeVectorFromMatrixCol( PyArrayObject *A, PyArrayObject *x, unsigned int col );

void PrintVectorData( PyArrayObject *pyobj1 );
void PrintBlocks( PyArrayObject *pyobj1 );

#endif /* FHBTOOL_H_ */
