//This code puts FHB data into 1d
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include "commen_for_this_project.h"

typedef struct
{
  //JMI - just elide this for now. PyObject_HEAD
   char *data;             /* pointer to raw data buffer */
   int nd;                 /* number of dimensions, also called ndim */
        
        //int flags;              /* Flags describing array -- see below*/
  unsigned int *dimensions;
  unsigned int *strides;

  char **blockData;
  unsigned int numberOfBlocks;
  //These two variables indicate the number of blocks in x- and y dim.
  //Blocks are layed out in row-mayor format
  unsigned int numberOfBlocksXDim;
  unsigned int numberOfBlocksYDim;
  unsigned int blockSize;
  unsigned int size_in_bytes; // Wolfgang: Maybe???
  unsigned int startIndexForTheRelevantDimension;//Where does the relevant blockData start en the data array?
  unsigned int lengthForTheRelevantDimension;
  unsigned int paddingx;
  unsigned int paddingy;

} PyArrayObject __attribute__((aligned (16)));

void printObject(PyArrayObject *obj1)
{
	printf("Matrix is:\n");
	//Print a PyArrayObject
	unsigned int i;
	for(i=0;i<64;i++)
	{
		printf("%d, ",((unsigned int*)obj1->data)[i]);
	}
	printf("\n");
}

void copy(char *from, char *to, unsigned int length)
{
	unsigned int i;
	for(i = 0; i<length;i++)
	{
		printf("copyBegin\n");
		printf("to is: %d\n",to+i);
		printf("from is: %d\n",from+i);
		printf("length is: %d\n",length);
		to[i] = from[i];
		printf("Copied a................................ : %d\n",to[i]);
		printf("copyEnd\n");
	}
	//Remeber to increment the pointers outside this function, or implement it here with references instead.
}

void FHB2Numpy(PyArrayObject *obj)
{
	printf("%d and testing data \n",1);
	printf("%d\n",obj->blockData[0][3]);
	
	
	
	unsigned blockSize; //elementsX, elementsY; //blockSize in bytes.
	unsigned xBlocks, yBlocks; //How many blocks in x- and y dimension.
	unsigned paddingX, paddingY; //How much padding to the right and bottom - in bytes.
	
	unsigned int totalNumberOfBLocks = obj->numberOfBlocks;
	unsigned int numberOfBlocksPerMatrix = obj->lengthForTheRelevantDimension;
	//unsigned int indexOfTheFirstRelevantBlock = obj->startIndexForTheRelevantDimension;
	unsigned int NumberOfMatricesInNDArray = totalNumberOfBLocks/numberOfBlocksPerMatrix;
	
	char* fromPointer, *toPointer;
	
	toPointer = obj->data;
	blockSize = obj->blockSize;
	xBlocks = obj->numberOfBlocksXDim;
	yBlocks = obj->numberOfBlocksYDim;
	paddingX = obj->paddingx;
	paddingY = obj->paddingy;
	
	//We count the toPointer up contigously and move the fromPointer around so it matches.
	
	//for each yblock row, except last
	//  - For each block in row, except last
	//  - For last block in row
	
	unsigned int i;
	unsigned int j;
	unsigned int u;
	
	//Set the pointer to point to the first block of blockrow yBlock
	fromPointer = obj->blockData[i*xBlocks];
	for(i=0;i<yBlocks-1;i++)//Per blockRow, except last
	{
		printf("%d\n",2);
		printf("Outerloop executes: %d, %d\n",i,yBlocks-1);
		
		
		
		
		for(u=0;u<blockSize/4;u++)//For each row, where a row goes across all blocks in a blockRow
		{
			printf("%d\n",3333);
			printf("Total row: %d,%d\n", u,blockSize/4);
			
			for(j=0;j<xBlocks-1;j++)//For each block in the blockrow, except last
			{
				printf("%d\n",4);
				//Take the current row of the block and move the pointer to the current row in the next block
				copy(fromPointer,toPointer,blockSize);
				toPointer=toPointer+blockSize;
				
				printf("%d\n",45);
				
				//move pointer to the current row in the next block
				fromPointer = obj->blockData[(i*xBlocks)+(j+1)] + u*blockSize;
			}
			
			printf("%d\n",44);
			//for last block in blockRow
			copy(fromPointer,toPointer,blockSize-paddingX);
			toPointer=toPointer+blockSize-paddingX;


			//Take the last row and move the pointer to point to, either:
			//  - the next row of the first block
			//(make sure the pointer is set correctly in the endcases - this should probably be taken
			//care of in the beginning the next outer iteration.)
			fromPointer = obj->blockData[i*xBlocks] + (u+1)*blockSize;
			printf("0000000000000000000001 setting pointer for new blockrow: %d\n", fromPointer);
			printf("0000000000000000000001 setting pointer for new blockrow: %d\n", i*xBlocks);
			printf("0000000000000000000001 setting pointer for new blockrow: %d\n", (u+1)*blockSize);
		}
		
		//
		fromPointer = obj->blockData[(i+1)*xBlocks];
		
		printf("0000000000000000000000 setting pointer for new blockrow: %d\n", fromPointer);
		printf("0000000000000000000000 setting pointer for new blockrow: %d\n", (i+1)*xBlocks);
		
		
	}
	
	//For the last blockRow
	fromPointer = obj->blockData[(yBlocks-1)*xBlocks];
	printf("fromPointer sat for the final blockrow: %d\n",fromPointer);
	printf("(yBlocks-1)*xBlocks sat for the final blockrow: %d\n",(yBlocks-1)*xBlocks);
	
	for(u=0;u<(blockSize/4)-paddingY;u++)//For each row, where a row goes across all blocks in a blockRow
		{
			printf("%d\n",5);
			printf("%d\n",(yBlocks-1));
			printf("%d\n",(xBlocks));
			printf("u: %d\n",u);
			printf("(blockSize/4)-paddingY: %d",(blockSize/4)-paddingY);
			
			for(j=0;j<xBlocks-1;j++)//For each block in the blockrow, except last
			{
				printf("%d\n",6);
				//Take the current row of the block and move the pointer to the current row in the next block
				copy(fromPointer,toPointer,blockSize);
				printf("%d\n",6);
				toPointer=toPointer+blockSize;
				
				//move pointer to the current row in the next block
				fromPointer = obj->blockData[(i*xBlocks)+(j+1)] + u*blockSize;
			}
			printf("%d\n",61);
			//for last block in blockRow
			copy(fromPointer,toPointer,blockSize-paddingX);
			toPointer=toPointer+blockSize-paddingX;
			printf("%d\n",62);

			//Take the last row and move the pointer to point to, either:
			//  - the next row of the first block
			//(make sure the pointer is set correctly in the endcases - this should probably be taken
			//care of in the beginning the next outer iteration.)
			fromPointer = obj->blockData[i*xBlocks] + (u+1)*blockSize;
			printf("%d\n",7);
			printf("99999999999999999999999999999999999999999999999999999999999999999\n");
		}
	
	
	//...
}

int main()
{
	//Create an array
	PyArrayObject *obj1 = _malloc_align2(sizeof(PyArrayObject),7);
	
	obj1->blockData = _malloc_align2(sizeof(char**)*4,7);
	obj1->data = _malloc_align2(256,7);
	obj1->numberOfBlocksXDim = 2;
	obj1->numberOfBlocksYDim = 2;
	obj1->blockSize = 16;//bytes
	obj1->paddingx = 4;//bytes
	obj1->paddingy = 2;//rows
	
	unsigned int *array1 = _malloc_align2(64,7);
	unsigned int *array2 = _malloc_align2(64,7);
	unsigned int *array3 = _malloc_align2(64,7);
	unsigned int *array4 = _malloc_align2(64,7);
	
	//Fill data in our test matrix  //REDO THIS!!!
	unsigned int i;
	for(i=0;i<16;i++)
	{
		array1[i] = 1;
		//array3[i] = 0;
	}
	
	for(i=0;i<3;i++)
	{
		array2[i] = 2;
		array3[i] = 3;
		array4[i] = 4;
	}
	for(i=3;i<4;i++)
	{
		array2[i] = 0;
		array3[i] = 0;
		array4[i] = 4;
	}
	for(i=4;i<7;i++)
	{
		array2[i] = 2;
		array3[i] = 3;
		array4[i] = 4;
	}
	for(i=7;i<8;i++)
	{
		array2[i] = 0;
		array3[i] = 0;
		array4[i] = 4;
	}
	for(i=8;i<11;i++)
	{
		array2[i] = 2;
		array3[i] = 3;
		array4[i] = 4;
	}
	for(i=11;i<12;i++)
	{
		array2[i] = 0;
		array3[i] = 0;
		array4[i] = 4;
	}
	for(i=12;i<15;i++)
	{
		array2[i] = 2;
		array3[i] = 0;
		array4[i] = 0;
	}
	for(i=15;i<16;i++)
	{
		array2[i] = 0;
		array3[i] = 0;
		array4[i] = 0;
	}
	
	
	
	obj1->blockData[0] = (char*)array1;
	obj1->blockData[1] = (char*)array2;
	obj1->blockData[2] = (char*)array3;
	obj1->blockData[3] = (char*)array4;
	
	//printObject(&obj1);
	
	//printf("%d and testing data \n",1);
	//printf("11111111111111%d\n",array1[0]);
	
	FHB2Numpy(obj1);
	
	printObject(obj1);
	
	
	return 1;	
}
