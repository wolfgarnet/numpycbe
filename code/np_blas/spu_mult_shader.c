#include <stdio.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include <common_spu.h>

//NOTE! 
typedef struct PyArrayObject {
        //JMI - just elide this for now. PyObject_HEAD
        //char *data;             /* pointer to raw data buffer */
        //int nd;                 /* number of dimensions, also called ndim */
        
        //int flags;              /* Flags describing array -- see below*/
        
  char **blockData;
  unsigned int numberOfBlocks;
  //These two variables indicate the number of blocks in x- and y dim.
  //Blocks are layed out in row-mayor format
  unsigned int numberOfBlocksXDim;
  unsigned int numberOfBlocksYDim;
  unsigned int *blockSizes;
  unsigned int *blockRowSize;//Might include padding
  unsigned int *blockColSize;
  unsigned int padding[1];

}PyArrayObject __attribute__((aligned (16)));

//This is the struc that is used to map data in the shaders memoryblock, which has been assigned to it
//from the dispatcher.
#define SIZEOMETADABUFFER 16
#define LS_METADATA ((METADATA*)info->LS_shaderMemory)->
typedef struct METADATA
{
	//These pointer must be initialized to point into the LS_DATA_AREA
	int *LS_DATA_AREA_1_Ptr;// = (unsigned int*)&LS_DATA_AREA[0];
	int *LS_DATA_AREA_2_Ptr;// = (unsigned int*)&LS_DATA_AREA[16];
	int *LS_DATA_AREA_3_Ptr;// = (unsigned int*)&LS_DATA_AREA[32];
	int padding[1];
	//JMI:27/03/09 - Change these to be "solid" instead of just pointers.
	unsigned int LS_METADATA_AREA_1_BLOCKDATA[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKDATA[0];
	unsigned int LS_METADATA_AREA_1_BLOCKSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKSIZES[0];
	unsigned int LS_METADATA_AREA_1_BLOCKROWSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKROWSIZES[0];
	unsigned int LS_METADATA_AREA_1_BLOCKCOLSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKCOLSIZES[0];
	unsigned int LS_METADATA_AREA_2_BLOCKDATA[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKDATA[4];
	unsigned int LS_METADATA_AREA_2_BLOCKSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKSIZES[4];
	unsigned int LS_METADATA_AREA_2_BLOCKROWSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKROWSIZES[4];
	unsigned int LS_METADATA_AREA_2_BLOCKCOLSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKCOLSIZES[4];
	unsigned int LS_METADATA_AREA_3_BLOCKDATA[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKDATA[8];
	unsigned int LS_METADATA_AREA_3_BLOCKSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKSIZES[8];
	unsigned int LS_METADATA_AREA_3_BLOCKROWSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKROWSIZES[8];
	unsigned int LS_METADATA_AREA_3_BLOCKCOLSIZES[SIZEOMETADABUFFER];// = (unsigned int*)&LS_METADATA_AREA_BLOCKCOLSIZES[8];
	
	unsigned int numberOfBlockJobs;// = 0;

	int currentBlockWereWorkingOn;// = -1;
	int currentXBlockWereWorkingOn;// = 0;
	int currentYBlockWereWorkingOn;// = 0;
	PyArrayObject pyArrayObject[3]; //Used to hold the actual PyArrayObjects of metadata
	
	//int padding[1];
}METADATA __attribute__((aligned (16)));

typedef unsigned int (Malloc)(unsigned int, unsigned int);
//Malloc*  malloc;
typedef unsigned int (PrintInt)(unsigned int);

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

void setNumberOfBlockJobs(unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs)
{
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));
	
	unsigned int divNumber = ((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocks/myop->num_SPES;
	unsigned int modNumber = ((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocks%myop->num_SPES;
	
	((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs = divNumber;
	if(modNumber>0)
	{
		//The new id's start from 0
		if((SPE_id)<modNumber)
		{
			((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs = ((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs+1;
		}
	}
	//setCurrentBlock counter
	((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn = 999;
}

void doTheMultThingy(unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs)
{
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));
	unsigned int i;
	
	setNumberOfBlockJobs(SPE_id, info, myop, funcs);
	
	//NEEDS TO REDONE, SEE THE NOTE IN THE LOOP BELOW REGARDING FETCH OF METADATA
	spu_writech(MFC_WrTagMask, 1);
	//Get stuff from pyObject1
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKDATA, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockData, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockSizes, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKROWSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockRowSize, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKCOLSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockColSize, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//Get stuff from pyObject2
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKDATA, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockData, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockSizes, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKROWSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockRowSize, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKCOLSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockColSize, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//Get stuff from resultObject
	
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKDATA, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockData, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockSizes, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKROWSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockRowSize, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKCOLSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockColSize, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	spu_writech(MFC_WrTagMask, 1 << 1);
	(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
	
	
	
	i = 0;
	printInt(i);
	for(i = 0;i<((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs;i++)
	{
		int testInt = ((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn;
		printInt(testInt);
		if(!(testInt<999))
		{
			((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn = SPE_id;
			//Set info on which X,Y block it is were working on in the result matrix
			((METADATA*)info->LS_shaderMemory)->currentYBlockWereWorkingOn = (SPE_id)/((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocksXDim;
			((METADATA*)info->LS_shaderMemory)->currentXBlockWereWorkingOn = (SPE_id)%((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocksXDim;
			((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs--;
		}
		else
		{
			((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn = ((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn + myop->num_SPES;
			((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs--;
		}
		
		//Assuming that it has already been tested, that the two apperand matrices can be multiplied together,
		//we just need to keep track of the row and col block numbers.
		unsigned int innerLoopCounter = 0;//Used in setting indexes on pyObject1 and pyObject2 blocks
		
		unsigned int outerLoopCounter = 0;
		
		unsigned int indexIntoPyObject1 = 0;
		unsigned int indexIntoPyObject2 = 0;
		
		for(outerLoopCounter = 0; outerLoopCounter < ((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksXDim;outerLoopCounter++)
		{
			indexIntoPyObject1 = ((METADATA*)info->LS_shaderMemory)->currentYBlockWereWorkingOn*((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksXDim+outerLoopCounter;
			printInt(33336);
			printInt(indexIntoPyObject1);
			indexIntoPyObject2 = outerLoopCounter*((METADATA*)info->LS_shaderMemory)->pyArrayObject[1].numberOfBlocksXDim+((METADATA*)info->LS_shaderMemory)->currentXBlockWereWorkingOn;
			printInt(33337);
			printInt(indexIntoPyObject2);
			
			spu_writech(MFC_WrTagMask, 1);
			spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKDATA[indexIntoPyObject1], 64, 1, MFC_GET_CMD);
			spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKDATA[indexIntoPyObject2], 64, 1, MFC_GET_CMD);
			spu_writech(MFC_WrTagMask, 1 << 1);
			(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
			printInt(33339);
		
  			unsigned int blockRowSizeInInts = ((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKROWSIZES[0]/4;//Is this correct? Test it!
  			unsigned int blockColSize = ((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKCOLSIZES[0];
  			unsigned int innerRowCount = 0;
  			unsigned int innerColCount = 0;
  			unsigned int innerInnerCounter = 0;
  			unsigned int indexIntoPyObject1Block = 0;
  			unsigned int indexIntoPyObject2Block = 0;
  			unsigned int actualIndex1 = 0;
  			unsigned int actualIndex2 = 0;
  			//unsigned int indexIntoPyObject3Block = 0;
  			unsigned int actualIndex3 = 0;
  			
  			unsigned int testInnerInnerLoopResultCounter = 0;
  			
  			for(innerRowCount = 0; innerRowCount < blockRowSizeInInts;innerRowCount++)
  			{
  				printInt(55555551);
  				printInt(innerColCount);
  				printInt(blockColSize);
  				
  				for(innerColCount = 0; innerColCount < blockColSize;innerColCount++)
  				{
  					//For now we calculate the index here. This could be changed to make performance better.
  					indexIntoPyObject1Block = blockRowSizeInInts*innerRowCount;
  					indexIntoPyObject2Block	= innerColCount;
  					actualIndex1 = indexIntoPyObject1Block;
  					actualIndex2 = indexIntoPyObject2Block;
  					for(innerInnerCounter = 0; innerInnerCounter < blockColSize; innerInnerCounter++)
  					{
  						((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr[actualIndex3] = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr[actualIndex3] + ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1]*((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr[actualIndex2];
  						
  						actualIndex1 = actualIndex1+1;
  						actualIndex2 = actualIndex2 + blockColSize;
  						if(testInnerInnerLoopResultCounter==3)
  						{
  							testInnerInnerLoopResultCounter = 0;
  						}
  						else
  						{
  							testInnerInnerLoopResultCounter++;
  						}
  					}
  					actualIndex3++;
  					printInt(777);
  					printInt(actualIndex3);
  					printInt(777);
  				}
  			}
		innerLoopCounter++;
		}
		//Flush the resultBlock back to main memory
		spu_writech(MFC_WrTagMask, 1);
		spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKDATA[((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn], 64, 1, MFC_PUT_CMD);
		spu_writech(MFC_WrTagMask, 1 << 1);
		(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
	}
}

int main(unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs)
{	
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));

	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr = info->LS_blockDataArea[0];
	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr = info->LS_blockDataArea[1];
	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr = info->LS_blockDataArea[2];
  
  spu_writech(MFC_WrTagMask, 1);
 
  spu_mfcdma32(&(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]), (unsigned int)myop->obj[0], sizeof(PyArrayObject), 1, MFC_GET_CMD);
  
  
  spu_mfcdma32(&(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]), (unsigned int)myop->obj[1], sizeof(PyArrayObject), 1, MFC_GET_CMD);
  
  spu_mfcdma32(&(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]), (unsigned int)myop->obj[2], sizeof(PyArrayObject), 1, MFC_GET_CMD);
  
  spu_writech(MFC_WrTagMask, 1 << 1);
  (void)spu_mfcstat(MFC_TAG_UPDATE_ALL); // Wait for previous transfer done
 
  doTheMultThingy(SPE_id, info, myop, funcs);
  
  return 0;
}
