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

//JMI:26/03/09 - this must be changed anyway and is already present in the working file (e.g.)
//Be sure there's no problems - eg with the new field "size_in_bytes" or others...

//typedef struct
//{
        //JMI - just elide this for now. PyObject_HEAD
        //char *data;             /* pointer to raw data buffer */
        //int nd;                 /* number of dimensions, also called ndim */

        //int flags;              /* Flags describing array -- see below*/

//  char **blockData;
//  unsigned int numberOfBlocks;
  //These two variables indicate the number of blocks in x- and y dim.
  //Blocks are layed out in row-mayor format
//  unsigned int numberOfBlocksXDim;
//  unsigned int numberOfBlocksYDim;
//  unsigned int blockSizes;
//  unsigned int size_in_bytes; // Wolfgang: Maybe???
//  unsigned int *blockRowSize;//Might include padding
//  unsigned int *blockColSize;
//  unsigned int padding[2];

//} PyArrayObject __attribute__((aligned (16)));


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
