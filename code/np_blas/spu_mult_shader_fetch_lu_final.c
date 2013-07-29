
#include <stdio.h>
#include <spu_intrinsics.h>
#include <spu_mfcio.h>
#include "/root/Desktop/master/code/common/common_spu.h"
#include "/root/Desktop/master/code/common/common.h"
#include <spu_internals.h>

#define SIXTYFOUR

//JMI: 20_05/09. Outcommented SliceObject. If needed we must get it from the repository instead.
//typedef struct
//{
//	unsigned int slice_type;//0:row, 1:col
//	unsigned int slice_start;//start row/col
//	unsigned int slice_end;//end row/col
	//Is it also possible to slice only half a row etc.?
//	unsigned int padding;
//} SliceObject __attribute__((aligned (16)));

//JMI: 20_05/09. Outcommented PyArrayobject and got it from repository file instead.
//typedef struct
//{
  //If the head is 8 bytes we have 20 bytes - otherwise is padding needed?

  //PyObject_HEAD
//  unsigned int padding1;//This is just to represent a common header-expansion.
//  unsigned int padding2;
  
//  char *data;             /* pointer to raw data buffer */
//  int nd;                 /* number of dimensions, also called ndim */
//  unsigned int *dimensions;       /* size in each dimension */
//  unsigned int *strides;          /* bytes to jump to get to the
//                                   next element in each dimension */
//  unsigned int *base;         /* This object should be decref'd
//                                   upon deletion of array */
                                /* For views it points to the original array */
                                /* For creation from buffer object it points
                                   to an object that shold be decref'd on
                                   deletion */
                                /* For UPDATEIFCOPY flag this is an array
                                   to-be-updated upon deletion of this one */
//  unsigned int *descr;   /* Pointer to type structure */
//  int flags;              /* Flags describing array -- see below*/
//  unsigned int *weakreflist;  /* For weakreferences */
  
//  int pad1;
//  int pad2;

//  char **blockData;
//  unsigned int numberOfBlocks;
  //These two variables indicate the number of blocks in x- and y dim.
  //Blocks are layed out in row-mayor format
//  unsigned int numberOfBlocksXDim;
//  unsigned int numberOfBlocksYDim;
//  unsigned int blockSize; // Number of elements per row
//  unsigned int size_in_bytes; // Wolfgang: Maybe???
//  unsigned int *blockRowSize;//Might include padding
//  unsigned int *blockColSize;

  // Exchange this:
  //unsigned int padding[2];
  // To this:
//  unsigned int paddingx;
//  unsigned int paddingy;

  //This is slicing information - is this obejct representing a slice?

//  SliceObject *slice_info;
//  unsigned int padding7;//included two more to make it aligned
//  unsigned int padding8;
//  unsigned int padding9;
//} PyArrayObject __attribute__((aligned (16)));

//This is the struc that is used to map data in the shaders memoryblock, which has been assigned to it
//from the dispatcher.
//Make sure this size does not conflict with elements of the METADATA being alligned.
#define SIZEOMETADABUFFER 4096 //This says how much space we allocate for buffers - new metadata are fetched accordingly.
//#define BLOCKSIZE 16384 //This must be taken from the struct of course. In bytes
typedef struct METADATA
{
	float *LS_DATA_AREA_1_Ptr;
	float *LS_DATA_AREA_2_Ptr;
	float *LS_DATA_AREA_3_Ptr;
	float *LS_DATA_AREA_4_Ptr;
	float *LS_DATA_AREA_5_Ptr;
	float *LS_DATA_AREA_6_Ptr;
	int padding[1];
	unsigned int numberOfBlockJobs;
	unsigned int LS_METADATA_AREA_1_BLOCKDATA[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_1_BLOCKSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_1_BLOCKROWSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_1_BLOCKCOLSIZES[SIZEOMETADABUFFER];
	unsigned int LS_METADATA_AREA_2_BLOCKDATA[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_2_BLOCKSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_2_BLOCKROWSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_2_BLOCKCOLSIZES[SIZEOMETADABUFFER];
	//The doublebuffer block for argument 1
	//unsigned int LS_METADATA_AREA_4_BLOCKDATA[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_4_BLOCKSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_4_BLOCKROWSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_4_BLOCKCOLSIZES[SIZEOMETADABUFFER];
	//The doublebuffer block for argument 2
	//unsigned int LS_METADATA_AREA_5_BLOCKDATA[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_5_BLOCKSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_5_BLOCKROWSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_5_BLOCKCOLSIZES[SIZEOMETADABUFFER];
	//The result block
	unsigned int LS_METADATA_AREA_3_BLOCKDATA[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_3_BLOCKSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_3_BLOCKROWSIZES[SIZEOMETADABUFFER];
	//unsigned int LS_METADATA_AREA_3_BLOCKCOLSIZES[SIZEOMETADABUFFER];
	int currentBlockWereWorkingOn;
	int currentXBlockWereWorkingOn;
	int currentYBlockWereWorkingOn;
	int padding2[1];
	PyArrayObject pyArrayObject[3];
	unsigned int BLOCKSIZE;
}METADATA __attribute__((aligned (16)));

typedef unsigned int (Malloc)(unsigned int, unsigned int);
//Malloc*  malloc;
typedef unsigned int (PrintInt)(unsigned int);

//JMI: 20_05/09 - Outcommented because struct is read from repository instead.
//Fjerner desuden referencer til alpha og beta i resten af koden.
//typedef struct
//{
//	unsigned int *EA_shader;
//	unsigned int shaderSize;
//	PyArrayObject *obj[3];
//	float scalar[2];//JMI:NB! Must be changed back. How exactly do we handle scalars with the PyArrayObject/dispatcher.
//	unsigned int num_SPES;
//} Operation_t __attribute__((aligned (16)));//Fix this error by taking the scalars from the beginning of the first block instead.

void setNumberOfBlockJobs(unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs)
{
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));
	
	unsigned int divNumber = ((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocks/myop->num_SPES;
	unsigned int modNumber = ((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocks%myop->num_SPES;
	
	((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs = divNumber;
	if(modNumber>0)
	{
		if((SPE_id)<modNumber)
		{
			((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs = ((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs+1;
		}
	}
	//setCurrentBlock counter
	((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn = 5000;
	//printInt(666661);
	//printInt(divNumber);
	//printInt(modNumber);
	//printInt(((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs);
	//printInt(((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs);
	//printInt(myop->num_SPES);
	//printInt(666661);
}

//Be aware if the way the adr of metadataarrays are calculated correctly (thinking about 'startIndex').
//JMI: 03/04/09 - This should only be done for blockpointers and it must be done individually
//for result and arguments, assuming that both arguments can fetch pointers contiguously (i.e. that
//the PyArrayObject stores pointers in both row and col order).
void fetchMetaData(LS_ShaderInfo *info, unsigned int startIndex, Functions_t *funcs)
{
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));
	//This function now only works for arguments.
	//Call it appropriately.
	
	//TIP: Do this with double buffering if there is performance problems -
	//by using seperate ID and checking on this ID before using metadata in loop. Also fetch data a few cycles before needed.
	//Of course, a second buffer is needed.
	//printInt(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKDATA);
	//printInt((unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockData);
	//printInt(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKDATA);
	//printInt((unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockData);
	//printInt(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKDATA);
	//printInt((unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockData);
	spu_writech(MFC_WrTagMask, 1);
	//Get stuff from pyObject1
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKDATA, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockData+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockSizes+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKROWSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockRowSize+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKCOLSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]).blockColSize+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//Get stuff from pyObject2
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKDATA, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockData+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockSizes+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKROWSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockRowSize+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKCOLSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]).blockColSize+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	//Get stuff from resultObject
	
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKDATA, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockData+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockSizes+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKROWSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockRowSize+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKCOLSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockColSize+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	
	
	spu_writech(MFC_WrTagMask, 1 << 1);
	(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
	
}

void fetchMetaDataForResult(LS_ShaderInfo *info, unsigned int startIndex)
{
	//TIP: Do this with double buffering if there is performance problems -
	//by using seperate ID and checking on this ID before using metadata in loop. Also fetch data a few cycles before needed.
	//Of course, a second buffer is needed.
	spu_writech(MFC_WrTagMask, 1);
	spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKDATA, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockData+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockSizes+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKROWSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockRowSize+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	//spu_mfcdma32(((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKCOLSIZES, (unsigned int)(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]).blockColSize+startIndex, SIZEOMETADABUFFER*4, 1, MFC_GET_CMD);
	spu_writech(MFC_WrTagMask, 1 << 1);
	(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
}

void resetValuesInResultBuffer(LS_ShaderInfo *info, unsigned int res_idx)
{
	unsigned int i;
	if(res_idx==3)
	{
		for(i = 0;i<SIZEOMETADABUFFER;i++)
		{
			((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr[i] = 0;
		}
	}
	else
	{
		for(i = 0;i<SIZEOMETADABUFFER;i++)
		{
			((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_6_Ptr[i] = 0;
		}
	}
}

//JMI: So we should make additional pointers to the datablocks in LS used for doublebuffering
//and switch between the two sets? Yes, but not for the resultblock - only for the two arguments are needed.
//We alterate between transferring data to databuffers 1,2 and 4,5.
//The index indicates which buffer set to use: 0 is 1,2, while 1 is 4,5.
//If the if-statement for this check must be avoided, we could possible create a second function instead?
void doCalculationsWithinTheBlocks(LS_ShaderInfo *info, unsigned int index, Functions_t *funcs, float *resPointer, float scalarA, float scalarB)
{
	//JMI:10/04/09 - Check if scalarA and scalarB, respectively, are equal to 1 and call optimized function that does not
	//multiply each block before using it.
	
	
	//JMI:10/04/09 - Many of these vars could be set outside this scope!
	
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;
	typedef float (PrintFloat)(float);
	PrintFloat* printFloat = (PrintFloat*)funcs->printfloat;
	
	unsigned int blockRowSizeInInts = ((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].blockSize;//Is this correct? Test it!
	//unsigned int blockRowSizeInVectors = blockRowSizeInInts/4;
  	unsigned int blockColSize = ((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].blockSize;
  	unsigned int innerRowCount = 0;
  	unsigned int innerColCount = 0;
  	unsigned int innerInnerCounter = 0;
  	//unsigned int indexIntoPyObject1Block = 0;
  	//unsigned int indexIntoPyObject2Block = 0;
  	unsigned int actualIndex1 = 0;
  	unsigned int actualIndex2 = 0;
  	unsigned int actualIndex3 = 0;
  	vector float vector1;
  	float value1;
  	//vector float *vector2;
  	vector float *vector3 = (vector float *)resPointer;
  	
  	//printInt(121212);
  	//printInt(blockRowSizeInInts); 
  	//printInt(blockColSize);
  	//printInt(121212);
  	
  	//vector float vector2Actual;
  	
  	vector float *vec2, zeroVec;
  	zeroVec = (vector float){0,0,0,0};
  	//scalar2 = (vector float){scalarB,scalarB,scalarB,scalarB};
  	
  	//unsigned int testInnerInnerLoopResultCounter = 0;
  	if(index==0)
	{
		vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  			//This is calculation we do with each block
  			for(innerRowCount = 0; innerRowCount < blockRowSizeInInts;innerRowCount++)
  			{
  				//printInt(innerColCount);
  				//printInt(blockColSize/4);
  				//actualIndex1 = blockRowSizeInInts*innerRowCount;
  				//printInt(777);
  				//printInt(actualIndex1);
  				//printInt(777);
  				for(innerColCount = 0; innerColCount < blockColSize/4;innerColCount++)//Changed to plus 4 for vectorization
  				{
  					actualIndex1 = blockRowSizeInInts*innerRowCount;
  					actualIndex2 = innerColCount;
  					//vector3[actualIndex3] = spu_madd(vector3[actualIndex3],scalar2,zeroVec);//Whats the problem with having it here?
  					
  					//If SIXTYFOUR is defined, use the unrolled version of this loop
  					#ifdef SIXTYFOUR
  					for(innerInnerCounter = 0; innerInnerCounter < blockColSize/16; innerInnerCounter++)
  					{
  						//1
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//2
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//3
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//4
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//5
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//6
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//7
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//8
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//9
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//10
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//11
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//12
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//13
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//14
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//15
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						
  						//16
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						
  						actualIndex1++;
  						actualIndex2 = actualIndex2 + blockColSize/4;
  						
  					}
  					actualIndex3++;//Changed to +4 for vectorization
  					#endif
  					
  					
  					#ifndef SIXTYFOUR
   					for(innerInnerCounter = 0; innerInnerCounter < blockColSize; innerInnerCounter++)
  					{
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[actualIndex1] * scalarA;
  						
  						vector1 = (vector float){value1,value1,value1,value1};
  						
  						//if(value1==0)
  						//{
  							//printFloat(scalarA);
  							//printInt(actualIndex3);
  							//printInt(5);
  						//}
  						
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						//if(spu_extract(vector3[actualIndex3],0)<-4095)
  						//{
  							//printInt(actualIndex1);
  							//printInt(actualIndex2);
  							//printInt(actualIndex3);
  							//printFloat(spu_extract(vector1,0));
  							//printFloat(spu_extract(vec2[actualIndex2],0));
  							//printFloat(spu_extract(vector3[actualIndex3],0));
  							//printInt((actualIndex2 + blockColSize)/4);
  						//}
  						
  						actualIndex1++;
  						actualIndex2 = actualIndex2 + blockColSize/4;
  					}
  					actualIndex3++;//Changed to +4 for vectorization
  					#endif
  				}
  			}
	}
	else
	{
		vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
		//This is calculation we do with each block
  			for(innerRowCount = 0; innerRowCount < blockRowSizeInInts;innerRowCount++)
  			{	
  				//actualIndex1 = blockRowSizeInInts*innerRowCount;
  				//printInt(778);
  				//printInt(actualIndex1);
  				//printInt(778);
  				for(innerColCount = 0; innerColCount < blockColSize/4;innerColCount++)//Changed to plus 4 for vectorization
  				{
  					actualIndex1 = blockRowSizeInInts*innerRowCount;
  					actualIndex2 = innerColCount;
  					//vector3[actualIndex3] = spu_madd(vector3[actualIndex3],scalar2,zeroVec);//whats the problem with having it here?
  					
  					#ifdef SIXTYFOUR
  					for(innerInnerCounter = 0; innerInnerCounter < blockColSize/16; innerInnerCounter++)
  					{
  						//1
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//2
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//3
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//4
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//5
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//6
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//7
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//8
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//9
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//10
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//11
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//12
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//13
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//14
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//15
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = (actualIndex2 + blockColSize)/4;
  						//16
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						//vector1 = (vector float){value1,value1,value1,value1};
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  		
  						actualIndex1++;
  						actualIndex2 = actualIndex2 + blockColSize/4;
  						
  					}
  					actualIndex3++;//Changed to +4 for vectorization
  					#endif
  					
  					
  					#ifndef SIXTYFOUR
  					for(innerInnerCounter = 0; innerInnerCounter < blockColSize; innerInnerCounter++)
  					{
  						//vec2 = (vector float *)(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr);
  						
  						value1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr[actualIndex1] * scalarA;
  						
  						vector1 = (vector float){value1,value1,value1,value1};
  						//if(value1==0)
  						//{
  							//printFloat(scalarA);
  							//printInt(actualIndex3);
  							//printInt(6);
  						//}
  						
  						vector3[actualIndex3] = spu_madd(vector1,vec2[actualIndex2],vector3[actualIndex3]/*vec3*/);
  						//printFloat(spu_extract(vector3[actualIndex3],0));
  						//printInt(5);
  						//if(spu_extract(vector3[actualIndex3],0)<-4095)
  						//{
  							//printInt(actualIndex1);
  							//printInt(actualIndex2);
  							//printInt(actualIndex3);
  							//printFloat(spu_extract(vector1,0));
  							//printFloat(spu_extract(vec2[actualIndex2],0));
  							//printFloat(spu_extract(vector3[actualIndex3],0));
  							//printInt((actualIndex2 + blockColSize)/4);
  		
  						actualIndex1++;
  						actualIndex2 = actualIndex2 + blockColSize/4;
  					}
  					actualIndex3++;//Changed to +4 for vectorization
  					#endif
  				}
  			}
	}
}

void doTheMultThingy(unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs)
{
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));
	typedef float (PrintFloat)(float);
	PrintFloat* printFloat = (PrintFloat*)funcs->printfloat;
	unsigned int i;
	//printInt(555555);
	setNumberOfBlockJobs(SPE_id, info, myop, funcs);
	//printInt(555556);
	
	//JMI:Test to read of the scalars we just added...
	
	//NEEDS TO REDONE, SEE THE NOTE IN THE LOOP BELOW REGARDING FETCH OF METADATA
	//So we must actually have a check if we should fetch more metadata.
	//The last int is the index (must be incremented in four bytes)from where we should begin to fetch data from the arrays.
	//Find out where we must insert a check on when to fetch more metadata.
	
	//Here we fetch the first parts of metadata!
	//Change so it calls both result and argument fetches.
	//printInt(555557);
	fetchMetaData(info, 0, funcs);
	//printInt(555558);
	
	//unsigned int fetchCounter = SIZEOMETADABUFFER;
	//JMI:03/04/09 - include an outerloop here that executes n times for each datafetch!
	//unsigned int k;
	
	
	i = 0;
	
	//JMI: 03/04/09 - Change it, so it dosen't use modulo for result metadata, but splits the array in contiguous parts.
	//Calculate current block accordingly and also check if the metadatabuffer should be refilled.
	//This is the outer loop, where we check for result metadata.
	
	//////////////////////Moved these away from inside the loop
	unsigned int innerLoopCounter = 0;//Used in setting indexes on pyObject1 and pyObject2 blocks
		
	unsigned int outerLoopCounter = 0;
		
	unsigned int indexIntoPyObject1 = 0;
	unsigned int indexIntoPyObject2 = 0;
	
	//unsigned int indexIntoPyObject3 = 0;
		
	float *bufPointer1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr;
	float *bufPointer2 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr;
		
	float *resPointer = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr;
		
	unsigned int zero_one = 0;
	///////////////////////////////////////////////////////////
	
	//ID used for double buffering
	unsigned int first_idx = 0, second_idx = 0;
	//Use 3 and 4 for result transfer ID's
	int res_idx = 3;
	int res_idx_second = 4;
	
	//printInt(3333333);
	
	for(i = 0;i<((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs;i++)
	{	
		int testInt = ((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn;
		if(!(testInt<5000))
		{
			
			((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn = SPE_id;
			//Set info on which X,Y block it is were working on in the result matrix
			((METADATA*)info->LS_shaderMemory)->currentYBlockWereWorkingOn = (SPE_id)/((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocksXDim;
			((METADATA*)info->LS_shaderMemory)->currentXBlockWereWorkingOn = (SPE_id)%((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocksXDim;
			//JMI:03/04/09 - Removed this decrement since the loop invariant already takes care of this!!!
			//((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs--;
		}
		else
		{
			
			((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn = ((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn + myop->num_SPES;
			
			//Also set the pointer for currentYBlockWereWorkingOn and currentXBlockWereWorkingOn!!!
			((METADATA*)info->LS_shaderMemory)->currentYBlockWereWorkingOn = (((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn)/((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocksXDim;
			((METADATA*)info->LS_shaderMemory)->currentXBlockWereWorkingOn = (((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn)%((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocksXDim;
			
			//JMI:03/04/09 - Removed this decrement since the loop invariant already takes care of this!!!
			//((METADATA*)info->LS_shaderMemory)->numberOfBlockJobs--;
		}
		
		//Assuming that it has already been tested, that the two apperand matrices can be multiplied together,
		//we just need to keep track of the row and col block numbers.
		
		
		//Start the first DMA
		indexIntoPyObject1 = ((METADATA*)info->LS_shaderMemory)->currentYBlockWereWorkingOn*((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksXDim+0;
		indexIntoPyObject2 = 0*((METADATA*)info->LS_shaderMemory)->pyArrayObject[1].numberOfBlocksXDim+((METADATA*)info->LS_shaderMemory)->currentXBlockWereWorkingOn;
		
		//spu_writech(MFC_WrTagMask, 1);
		////printInt(333331);
	    spu_mfcdma32(bufPointer1, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKDATA[indexIntoPyObject1], ((METADATA*)info->LS_shaderMemory)->BLOCKSIZE, first_idx, MFC_GET_CMD);
	    
		spu_mfcdma32(bufPointer2, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKDATA[indexIntoPyObject2], ((METADATA*)info->LS_shaderMemory)->BLOCKSIZE, first_idx, MFC_GET_CMD);
		
		spu_mfcdma32(resPointer, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKDATA[((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn], ((METADATA*)info->LS_shaderMemory)->BLOCKSIZE, 5, MFC_GET_CMD);
		//For now we have to wait for this transfer in order to multiply with beta.
		spu_writech(MFC_WrTagMask, 1 << 5);
		//printInt(1233);
		(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
		//printInt(1233);
		
		//printInt(987);
		//printFloat(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[0]);
		//printFloat(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr[4096]);
		
		//printInt(987);
		
		//This part of the code does not matter, since beta is always 1.
		//vector float scalar2 = (vector float){myop->scalar[1],myop->scalar[1],myop->scalar[1],myop->scalar[1]};
		
		//unsigned int u;
		
		//This loop does not matter, since beta is always 1
		//for(u=0;u<BLOCKSIZE/16;u++)
		//{
			//((vector float *)resPointer)[u] = ((vector float *)resPointer)[u]*scalar2;
		//}
		
		
		
		//JMI:01/04/09 - outerLoopCounter is controlled so we take in into that we have already transferred the first block.
		//Thats why we add one to the loop counte.
		//printInt(outerLoopCounter);
		//printInt((((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksXDim  ));
		for(outerLoopCounter = 1; outerLoopCounter < (((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksXDim  );outerLoopCounter++)
		{
			//printInt(outerLoopCounter);
			//printInt((((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksXDim  ));
			second_idx = first_idx ^ 1;
			//If this check sufficient?
			if(bufPointer1==(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr))
			{
				bufPointer1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr;
				bufPointer2 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr;
				
				zero_one = 0;
			}
			else
			{
				bufPointer1 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr;
				bufPointer2 = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr;
				
				zero_one = 1;
			}
			
			indexIntoPyObject1 = ((METADATA*)info->LS_shaderMemory)->currentYBlockWereWorkingOn*((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksXDim+outerLoopCounter;
			indexIntoPyObject2 = outerLoopCounter*((METADATA*)info->LS_shaderMemory)->pyArrayObject[1].numberOfBlocksXDim+((METADATA*)info->LS_shaderMemory)->currentXBlockWereWorkingOn;
			
			//This is fetching the blocks we need.
			//This needs to be done using double buffering.
			//For now don't worry about using double-buffering accross cols/rows -
			//so we only double-buffer per result block so to speak.)
			//This should be expanded, so double buffering works "from the first to the next resultblock" so to speak.
			
			//The first transfer is started outside the loop - here we must alternate between buffersets to support double buffering.
			spu_writech(MFC_WrTagMask, 1);
			spu_mfcdma32(bufPointer1, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_1_BLOCKDATA[indexIntoPyObject1], ((METADATA*)info->LS_shaderMemory)->BLOCKSIZE, second_idx, MFC_GET_CMD);
			spu_mfcdma32(bufPointer2, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_2_BLOCKDATA[indexIntoPyObject2], ((METADATA*)info->LS_shaderMemory)->BLOCKSIZE, second_idx, MFC_GET_CMD);
			//This is not necessary, since we work with one resultblock at a time.
			//(the for optimization with doublebuffering on result put order is current not in use, because we need to add the "C" matrix.)
			//spu_mfcdma32(resFetchPoiner, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKDATA[((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn], BLOCKSIZE, first_idx, MFC_GET_CMD);
			
			spu_writech(MFC_WrTagMask, 1 << first_idx);
			(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
			//The second parameter must alterate between 0 and 1!
			//The function might be able to do this just as well, if we have a global variable to look at.
			//Print out first_idx in the functin to make sure it alterates between zero and one.
			//Make sure that this matches!!!
			
			
			//We HAVE to wait for this data to be ready. It's because of the current design of the algorithm -
			//a fix would be to actually do 2 x the algorithm with two different result blocks.
			//JMI:10/04/09 - A better suggestion for at fix:
			//basicly start 2 transfers to begin with and 1 in each consecutive loop...)
			//spu_writech(MFC_WrTagMask, 1 << 5);
			//(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
			//printInt(4444444);
			doCalculationsWithinTheBlocks(info,zero_one,funcs, resPointer, -1, 1);
			
			first_idx = second_idx;
			
		innerLoopCounter++;
		}
		//Wait for the last transfer to complete and do the necessary calculations
		spu_writech(MFC_WrTagMask, 1 << first_idx);
		(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
		//The second parameter must alterate between 0 and 1!
		//The function might be able to do this just as well, if we have a global variable to look at.
		//if(zero_one==0){zero_one = 1;}
		if(bufPointer1==(((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr))
			{
				zero_one = 0;
			}
			else
			{
				zero_one = 1;
			}
		//printInt(5555555);
		doCalculationsWithinTheBlocks(info,zero_one,funcs, resPointer, -1, 1);
		
		//Since we only use one result data block, we know that the additional resultblock is not used,
		//and we can transfer the one result block back as it is.
		//Flush the resultBlock back to m//spu_writech(MFC_WrTagMask, 3);
		//Wait for the former result transfer
		//Double buffering on result block is still missing! We must use the second one too, if 2x buffering is to be used.
		
		//JMI:04/04/09 - Correction. We should use the second result buffer, but only to ensure
		//that we can reset a resultbuffer to zero without trouble and still have calculations overlap tranfers.
		
		spu_mfcdma32(resPointer, (unsigned int)((METADATA*)info->LS_shaderMemory)->LS_METADATA_AREA_3_BLOCKDATA[((METADATA*)info->LS_shaderMemory)->currentBlockWereWorkingOn], ((METADATA*)info->LS_shaderMemory)->BLOCKSIZE, res_idx, MFC_PUT_CMD);
		/*
		 * For now there is no "real" double buffering on result data.
		 * However, calculations on arguments can still overlap with tranfers of resultdata to main memory, so
		 * it is deemed that it not necessary to use two result buffers.
		if(resPointer==((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr)
			{
				resPointer = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_6_Ptr;
			}
			else
			{
				resPointer = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr;
			}
			*/
		
		//Do this with a particualer result pointer.
		spu_writech(MFC_WrTagMask, 1 << res_idx_second);
		(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
		//resetValuesInResultBuffer(info, res_idx_second);
		if(res_idx==3)
		{
			res_idx = 4;
			res_idx_second = 3;
			resPointer = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_6_Ptr;
		}
		else
		{
			res_idx = 3;
			res_idx_second = 4;
			resPointer = ((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr;
		}
		
		//Here we must check if we should fetch more metadata for the arguments!!!
		//A check could be inserting in the beginning to see if this check is even necessary and a cloned function could be called.
		//Double buffering could be used, but try to test without it first...
	}
	//Wait for the last result transfer
	spu_writech(MFC_WrTagMask, 1 << res_idx);
	(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
}

void printValues(LS_ShaderInfo *info, Functions_t *funcs)
{
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));
	
    printInt(33);
    
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].blockSize);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocks);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksXDim);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].numberOfBlocksYDim);
    
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1].blockSize);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1].numberOfBlocks);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1].numberOfBlocksXDim);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1].numberOfBlocksYDim);
    
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].blockSize);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocks);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocksXDim);
    printInt(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2].numberOfBlocksYDim);
      
    printInt(33);
}

int main(unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs)
{	
	typedef unsigned int (PrintUInt)(unsigned int);
	PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));
	
	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_1_Ptr = info->LS_blockDataArea[0];
	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_2_Ptr = info->LS_blockDataArea[1];
	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_3_Ptr = info->LS_blockDataArea[2];
  	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_4_Ptr = info->LS_blockDataArea[3];//double buffering for argument 1
	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_5_Ptr = info->LS_blockDataArea[4];//double buffering for argument 2
	((METADATA*)info->LS_shaderMemory)->LS_DATA_AREA_6_Ptr = info->LS_blockDataArea[5];
	
  spu_writech(MFC_WrTagMask, 1);
  //printInt(&(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]));
  //printInt(&(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]));
  //printInt((unsigned int)myop->obj[0]);
  //printInt(sizeof(PyArrayObject));
  spu_mfcdma32(&(((METADATA*)info->LS_shaderMemory)->pyArrayObject[0]), (unsigned int)myop->obj[0], sizeof(PyArrayObject), 1, MFC_GET_CMD);
  
  spu_writech(MFC_WrTagMask, 1 << 1);
  (void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
  
  spu_mfcdma32(&(((METADATA*)info->LS_shaderMemory)->pyArrayObject[1]), (unsigned int)myop->obj[1], sizeof(PyArrayObject), 1, MFC_GET_CMD);
  
  spu_writech(MFC_WrTagMask, 1 << 1);
  (void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
  
  spu_mfcdma32(&(((METADATA*)info->LS_shaderMemory)->pyArrayObject[2]), (unsigned int)myop->obj[2], sizeof(PyArrayObject), 1, MFC_GET_CMD);
  
  spu_writech(MFC_WrTagMask, 1 << 1);
  (void)spu_mfcstat(MFC_TAG_UPDATE_ALL);
 
 //Testing blockData value
 //printValues(info, funcs);
 
 //Set BLOCKSIZE
 ((METADATA*)info->LS_shaderMemory)->BLOCKSIZE = 4*((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].blockSize*((METADATA*)info->LS_shaderMemory)->pyArrayObject[0].blockSize;
 //printInt(((METADATA*)info->LS_shaderMemory)->BLOCKSIZE);
 //printInt(67676767);
 
  doTheMultThingy(SPE_id, info, myop, funcs);
  
  
  return 0;
}

