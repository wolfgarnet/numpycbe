#include "../spefuncs.h"

#include "functions/##header##"

/*
 * Generic binary function for vectors, ie one dimensional arrays(can be applied to nd arrays)
 *
 * Notes:
 * 1) The minimum number of block pointers transfered from blockData, must be at least the number of SPEs
 * 2) A bus error occurs if the number of SPES is higher than the number of total blocks
 */

void genericbinary( unsigned int SPE_id, LS_ShaderInfo *info, Operation_t *myop, Functions_t *funcs )
{
	//typedef unsigned int (PrintUInt)(unsigned int);
	//PrintUInt* printInt = (PrintUInt*)funcs->printuint;//__attribute__ ((aligned(128)));

	// Make room for python meta data
	PyArrayObject *x1 = (PyArrayObject *)info->LS_shaderMemory;
	PyArrayObject *x2 = (PyArrayObject *)info->LS_shaderMemory+sizeof( PyArrayObject );
	PyArrayObject *r  = (PyArrayObject *)x2+sizeof( PyArrayObject );

	// Make room for the sliceObject
	SliceObject *slice_info = (SliceObject *)r+sizeof( SliceObject );


	// Get meta data
	_GetPyArrayMetaData( myop->obj[0], x1, 0 );
	_GetPyArrayMetaData( myop->obj[1], x2, 0 );
	_GetPyArrayMetaData( myop->obj[2],  r, 0 );

	_Wait(0); // WAIT!!!

	//_DBGP_UINT( 2 );

	//unsigned int xpad = _CalcMaxBlockArraySize( x1->numberOfBlocks );

	// Make room for data block arrays
	char **blocks_x1 = (char**)( ( (unsigned int)( r + sizeof( PyArrayObject )) + 127 ) &~ 127 );
	char **blocks_x2 = (char**)( blocks_x1 + _Pad16bytes( x1->numberOfBlocks ) );
	char **blocks_r  = (char**)( blocks_x2 + _Pad16bytes( x2->numberOfBlocks ) );

	//Fetch the SliceObject metadata
	//printInt((unsigned int)slice_info);
	//printInt((unsigned int)x1->slice_info);
	//printInt(sizeof(SliceObject));
	spu_mfcdma32(slice_info,(unsigned int)x1->slice_info, sizeof(SliceObject), 1, MFC_GET_CMD);
	spu_writech(MFC_WrTagMask, 1 << 1);
	(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);

	// Fetch pointers to first __ datablocks for vectors
//	_GetBlockPointersPart( x1, blocks_x1, 0, xpad, 0 );
//	_GetBlockPointersPart( x2, blocks_x2, 0, xpad, 0 );
//	_GetBlockPointersPart(  r,  blocks_r, 0, xpad, 0 );

	_GetBlockPointers( x1, blocks_x1, x1->numberOfBlocks, _BTAG );
	_Wait( _BTAG );


	_GetBlockPointers( x2, blocks_x2, x2->numberOfBlocks, _BTAG );
	_Wait( _BTAG );


	_GetBlockPointers(  r,  blocks_r,  r->numberOfBlocks, _BTAG );

	// Must wait for the block array to finish
	_Wait( _BTAG );

	//Get the sliceObject




	// Begin double buffering
	//_GetBlock( blocks_x1, info->LS_blockDataArea[0], SPE_id, x1->blockSize*x1->blockSize, 0 );
	//NB! For now this shader assumes that there is a sliceObject! This should be checked on the PPE with the current setup.

	//We should only get the rows relevant for the "current" SPE. This is relevant for fetching rows and for computing,
	//fix it in the get loop both arg1 and arg2 and in the Function file.
	//For now: Fix it for arg1 and in the Function file. Fix arg2 later. Also, how much time is really saved by using multiple DMAs to fetch only part og the vector?
	//myop->num_SPES.

	unsigned int numberOfRowsToFetchInTotal = x1->numberOfBlocksXDim;//64
	unsigned int numberOfRowsForThisParticularSPE;

	unsigned int divNumber = numberOfRowsToFetchInTotal/myop->num_SPES;
	unsigned int modNumber = numberOfRowsToFetchInTotal%myop->num_SPES;

	numberOfRowsForThisParticularSPE = divNumber;

	if(modNumber>0)
	{
		if((SPE_id)<modNumber)
		{
			numberOfRowsForThisParticularSPE = numberOfRowsForThisParticularSPE+1;
		}
	}

	//Now we have the number of rows for the specific SPE's.
	//Now we also need to calculat the offsets of where each SPE should start to fetch its first row.

	unsigned int firstRowToFetch;

	//We know how many SPEs, we know how many rows in total,
	if (modNumber>SPE_id)//correct with <= here?
	{
		firstRowToFetch = SPE_id*numberOfRowsForThisParticularSPE;//Because SPE_id is indedexed from zero.
	}
	else
	{
		firstRowToFetch = modNumber*(numberOfRowsForThisParticularSPE+1) +
		(SPE_id-modNumber)*numberOfRowsForThisParticularSPE;
	}

	//Data looks good ;-)

	unsigned int k;
	unsigned int startBlock = slice_info->slice_startBlock+firstRowToFetch;
	unsigned int startIndexInBlock = slice_info->slice_start;
	unsigned int LS_pointer = 0;

	if(numberOfRowsForThisParticularSPE>0)
	{

	//Next project: 1. Fetch only the rows specific to this SPE and 2. Make the Function file only work with these rows.

	//printInt(x1->blockSize);


	//for(k=0;k<x1->blockSize/x1->numberOfBlocksXDim;k++)
	for(k=0;k<numberOfRowsForThisParticularSPE;k++)
	{

		spu_mfcdma32(info->LS_blockDataArea[0]+LS_pointer, (unsigned int)blocks_x1[startBlock]+startIndexInBlock*x1->blockSize*4, x1->blockSize*4, 5, MFC_GET_CMD);

		//Try to fetch the arg2 rows we need in the same way
		//_GetBlock( blocks_x2, info->LS_blockDataArea[1], SPE_id, x2->blockSize*x2->blockSize, 0 );
		//For arg2 there is only one block, and we only need the startindex then increment the count one row per iteration.
		//15/06/09-added numberOfBlocksXDim to the calc.
		//ERROR HERE!!! firstRowToFetch is used to indicate which part of the specific row to fetch!
		
		//JMI: Moving this out of the loop and do it for a whole block of data instead of for seperate rows.
		//spu_mfcdma32(info->LS_blockDataArea[1]+LS_pointer, (unsigned int)blocks_x2[0]+firstRowToFetch*x1->blockSize*4+LS_pointer, x1->blockSize*4, 5, MFC_GET_CMD);

		//startIndexInBlock = startIndexInBlock + x1->blockSize*4;
		LS_pointer = LS_pointer + x1->blockSize*4;
		startBlock++;
	}
		spu_mfcdma32(info->LS_blockDataArea[1], (unsigned int)blocks_x2[0]+firstRowToFetch*x1->blockSize*4, x1->blockSize*4*numberOfRowsForThisParticularSPE, 5, MFC_GET_CMD);

		spu_writech(MFC_WrTagMask, 1 << 5);

		(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);


	//_GetBlock( blocks_x2, info->LS_blockDataArea[1], SPE_id, x2->blockSize*x2->blockSize, 0 );


	// No need for the result block

	unsigned int i=0, j;  //JMI:This is a hack, but works as long as the result-vector can be fitted inside a single block. As it is, this should be possible!
	unsigned int idx = 0, nextidx;
	unsigned int numelements = x1->blockSize * x1->blockSize;

	vector float *row1, *row2, *res;

	_Wait( idx );




	_compute( ##args1##, slice_info->slice_startBlock+firstRowToFetch, numberOfRowsForThisParticularSPE, x1->blockSize );//We know all needed blocks are row-major.


	//Set metadata on arg1. They should be identical to the arguments of arg2. JMI: Check that arg2 metadata is set correctly to begin with.
	//JMI:Also, handle the return value correctly!!! We have to return a new object, instead of A. A is being used over and over, and should not be changed!
	//Also, we copy data to the new object instead of referencing it, because the datalayout is different. This can be handles by freeing the object
	//in eg. SUM.

	//Current solution:
	//Make an eksplicit delete function.
	//Fill zeros in the remainder og the vector returned from multiply.
	//Max a block is used to hold each vector.
	//Move functions to the 'real' NumPyCBE and in the process extend the existing multiply using a switch to see of the slice_object is null or not (or use a sentinel from creation!)

	//JMI:We should not change these at all, since we actually return a fresh object.
	//x1->numberOfBlocksXDim = x2->numberOfBlocksXDim;
	//x1->numberOfBlocksYDim = x2->numberOfBlocksYDim;
	//x1->numberOfBlocks = x2->numberOfBlocks;

	// Send back the result. Make sure it's put in a new object.
	//Send only the relevant rows from the buffer and transfer them to the currect rows in the resultvector.
	//_PutAsync( (unsigned int)blocks_r[i], numelements * 4, info->LS_blockDataArea[idx*3+2], idx );

		startIndexInBlock = slice_info->slice_start;
		LS_pointer = 0;


		//for(k=0;k<numberOfRowsForThisParticularSPE;k++)
		//{
			//15/06/09-added numberOfBlocksXDim to the calc. There is an issue here!!! The place to write to must also be dependent on which SPE we are...
		//	spu_mfcdma32(info->LS_blockDataArea[2]+LS_pointer, (unsigned int)blocks_r[0]+firstRowToFetch*x1->blockSize*4+LS_pointer, x1->blockSize*4, 5, MFC_PUT_CMD);

			//startIndexInBlock = startIndexInBlock + x1->blockSize*4;
		//	LS_pointer = LS_pointer + x1->blockSize*4;
		//}

		spu_mfcdma32(info->LS_blockDataArea[2], (unsigned int)blocks_r[0]+firstRowToFetch*x1->blockSize*4, x1->blockSize*4*numberOfRowsForThisParticularSPE, 5, MFC_PUT_CMD);
		spu_writech(MFC_WrTagMask, 1 << 5);

		(void)spu_mfcstat(MFC_TAG_UPDATE_ALL);

		//printInt(424242424);
		_Wait( idx );
	}
}
