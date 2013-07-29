/*
 * common.h
 *
 *  Created on: Jan 27, 2009
 *      Author: wolle
 */

#ifndef COMMON_H_
#define COMMON_H_

// Maximum number of blocks per SPE
#define MAX_NUM_BLOCKS 3
//#define _DEBUG
#define _DBGLVL 6 // 1: Low, 10: High

#include <stdio.h>

typedef struct
{
  unsigned int slice_type;//0:row, 1:col
  unsigned int slice_start;//start row/col
  unsigned int slice_end;//end row/col
  //Is it also possible to slice only half a row etc.?
  unsigned int padding;
} SliceObject __attribute__((aligned (16)));

typedef struct
{
  //If the head is 8 bytes we have 20 bytes - otherwise is padding needed?

  //PyObject_HEAD
  unsigned int padding[2];//This is just to represent a common header-expansion.
  char *data;             /* pointer to raw data buffer */
  int nd;                 /* number of dimensions, also called ndim */
  unsigned int *dimensions;       /* size in each dimension */
  unsigned int *strides;          /* bytes to jump to get to the
                                   next element in each dimension */
  unsigned int *base;         /* This object should be decref'd
                                   upon deletion of array */
                                /* For views it points to the original array */
                                /* For creation from buffer object it points
                                   to an object that shold be decref'd on
                                   deletion */
                                /* For UPDATEIFCOPY flag this is an array
                                   to-be-updated upon deletion of this one */
  unsigned int *descr;   /* Pointer to type structure */
  int flags;              /* Flags describing array -- see below*/
  unsigned int *weakreflist;  /* For weakreferences */

  char **blockData;
  unsigned int numberOfBlocks;
  //These two variables indicate the number of blocks in x- and y dim.
  //Blocks are layed out in row-mayor format
  unsigned int numberOfBlocksXDim;
  unsigned int numberOfBlocksYDim;
  unsigned int blockSize; // Number of elements per row
  unsigned int size_in_bytes; // Wolfgang: Maybe???
//  unsigned int *blockRowSize;//Might include padding
//  unsigned int *blockColSize;

  // Exchange this:
  //unsigned int padding[2];
  // To this:
  unsigned int paddingx;
  unsigned int paddingy;

  //This is slicing information - is this obejct representing a slice?

  SliceObject *slice_info;
  unsigned int padding2;
} PyArrayObject __attribute__((aligned (16)));

typedef struct
{
	unsigned int *EA_shader;  // 4
	unsigned int shaderSize;  // 4
	PyArrayObject *obj[3];    // 12
	PyArrayObject *scalar[2]; // 8
	unsigned int num_SPES;    // 4
//	char *scalarresult;       // 4
//	unsigned int padding[3]   // 12
} Operation_t __attribute__((aligned (16)));


#endif /* COMMON_H_ */
