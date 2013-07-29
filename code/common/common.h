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
#define _DBGLVL 2 // 1: Low, 10: High

#define _DEBUG_TIME

//#define _64X64BLOCKS

#include <stdio.h>

typedef struct
{
	unsigned int slice_type;//0:row, 1:col
	unsigned int slice_start;//start row/col
	unsigned int slice_end;//end row/col
	//Is it also possible to slice only half a row etc.?
	unsigned int slice_startBlock;//Index of the first block.
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



/*
 * Solve structure
 */

typedef struct
{
	Operation_t *sger;
	Operation_t *sgemm;
	Operation_t *strsm_llnu;
	Operation_t *strsm_llnu_lu;
	Operation_t *strsm_lunn;
} Solve_t;


// Debug thingy
#define DEBUG_FP dbg_out.txt

#ifdef _DEBUG

#if _DBGLVL > 0

#define Printf1( str, ... ) printf( str, __VA_ARGS__ )
#define Print1( str ) printf( str )

#if _DBGLVL > 1

#define Printf2( str, ... ) printf( str, __VA_ARGS__ )
#define Print2( str ) printf( str )

#if _DBGLVL > 2

#define Printf3( str, ... ) printf( str, __VA_ARGS__ )
#define Print3( str ) printf( str )

#else // > 2

#define Printf3( str, ... )
#define Print3( str )

#endif // > 2

#else // > 1

#define Printf2( str, ... )
#define Print2( str )

#endif // > 1

#else // > 0

#define Printf1( str, ... )
#define Print1( str )

#endif // > 0

#else // ! Debug

#define Printff( str, ... ) printf( DEBUG_FP, "[%-12s:%3d] " str, __FILE__, __LINE__, __VA_ARGS__ );

#define Printf1( str, ... )
#define Printf2( str, ... )
#define Printf3( str, ... )

#define Print1( str )
#define Print2( str )
#define Print3( str )

#endif

#ifdef _DEBUG_TIME

#define _StartTimer( timer ) StartTimer( timer )
#define _StopTimer( timer ) StopTimer( timer )
#define _PrintTimer( timer ) PrintTimer( timer )

#else

#define _StartTimer( timer )
#define _StopTimer( timer )
#define _PrintTimer( timer )

#endif

#endif /* COMMON_H_ */




