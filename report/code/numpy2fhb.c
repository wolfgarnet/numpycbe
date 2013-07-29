#define PY_SSIZE_T_CLEAN
//#include "Python.h"
//#include "structmember.h"

#define _MULTIARRAYMODULE
#define NPY_NO_PREFIX
//#include "/root/Desktop/jmi-numpy-dev-working-files/devwork_48/ndarrayobject.h"
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>
#include "commen_for_this_project.h"

unsigned int sizesOfDimensions[32];//An array that keeps information about the 2dDataLength per dimension.
unsigned int instancesOfDimensions[32];//This data could also be seen directly from the PyArrayObject.
unsigned int numberOfDimensions;//This data could also be seen directly in the PyArrayObject.
unsigned int lengthOfDimensions[32];//Holds the complete lenght of data for the given dimension (including all
//instances in that dimension.

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

//memprint
void mprint(char *data,unsigned int len)
{
  unsigned int* dataPointer = (unsigned int*)data;
  int i;
  for(i = 0;i<len;i++)
    {
      printf("mprint here.................: Data at [%d] is: %d\n",i,dataPointer[i]);
    }
}


void findXYCuts(unsigned int y, unsigned int x, unsigned int maxBlockSize,
		unsigned int *number_of_y_cuts, unsigned int *number_of_x_cuts,
		unsigned int *originalX, unsigned int *originalY)
{
 

  //Store the original pointers:
  *originalX = x;
  *originalY = y;

  //assuming non-1d array!! If adday i 1-dim, we should be calling another function
  unsigned int newy = y;
  unsigned int newx = x + (x%4);//Modulo to make sure padding is included in the estimate
  unsigned int physicalBlockSize = newy * newx * 4;//note: 4 entails int's!
  float y_x_relationship = (float)newy/(float)newx;
  float block_relation = (float)physicalBlockSize/(float)maxBlockSize;

  unsigned int newBlockSize = physicalBlockSize;
  unsigned int numberOfYCuts = 0;
  unsigned int numberOfXCuts = 0;
  
  //basis situation for block split
  //find out if it's ok to return zero splits if the blocksize is equal to the max size-
  //should there be any buffer here?
  if(!(block_relation>1))//Note: maybe insert the size information directly to make it nicer?
    {
      //no op - number of cuts on both dims are zero.
    }
  else
    {
      //non-basis situation for block split
      while(newBlockSize>maxBlockSize)//while the new block size is bigger than the allowed size
	{
	  if(y_x_relationship<1)
	    {
	      //split on x-dim
	      numberOfXCuts++;
	      
	      //set new x value(pseudo)
	      newx=ceil((float)newx/(float)(numberOfXCuts+1));//this could be a problem if testing on small max_sizes

	      //calculate the new block size(pseudo)
	      newBlockSize = newy*newx*4;//note: 4 entails int's!

	      //calculate new y_x_relationship
	      y_x_relationship = (float)newy/(float)newx;
	    }
	  else
	    {
	      //split on y-dim
	      numberOfYCuts++;
	      
	      //set new y value(pseudo)
	      newy=ceil((float)newy/(float)(numberOfYCuts+1));//this could be a problem if testing on small max_sizes
	      
	      //calculate the new block size(pseudo)
	      newBlockSize = newy*newx*4;//note: 4 entails int's!

	      //calculate new y_x_relationship
	      y_x_relationship = (float)newy/(float)newx;
	    }
	}
    }
  *number_of_y_cuts = numberOfYCuts;
  *number_of_x_cuts = numberOfXCuts;
}

unsigned int getSizeOfNDArray(PyArrayObject *array)
{
  //returns size of array measured in int sizes
  int dataSize = 0;
  int numberOfDims = array->nd;

  if(numberOfDims>0)
    {
      dataSize = 1;
    }
  int i;
  for(i = 0; numberOfDims>i;i++)
    {
      dataSize = dataSize*array->dimensions[i];
      //printf("from->dimensions[%d]: %d  .......................\n",i,from->dimensions[i]);
      //printf("from->strides for dimension %d is: %d ...........\n",i,from->strides[i]);
    }
  return dataSize;
}


//To facilitate 2d blocks generation from nd-arrays, we must include arguments that give
//the startindex of the oldDataPointer and the startindex to the blockArray (newDataPointer).
//(plus maybe also indexes for end or length).
//Also, make sure that the number of blocks that is computed mathces the indexes manipulated in the data.
void createDataBlocks(unsigned int y_cuts, unsigned int x_cuts, PyArrayObject *from,
		      unsigned int data_size, unsigned int org_ySize, unsigned org_XSize, char *dataPointerOld,
		      unsigned int dataBlockIndex, char *dataPointerOriginal)
{
	 //Note on args:
  //int sizes are assumed for all sizes.

  //The alignment demand
  unsigned int alignment = 16;//In bytes

  //2. The order of business must be:
  //   1. Know the current data pointer, "new data pointer", rowsize count (to know when to pad).

  //char *oldData = dataPointerOld;//JMI:30/03/09 - this data must be taken from a new input instead.
  //char *newData = 0;//must run though each new block in turn.
  unsigned int newRowSize = 0;//The row size of the new block
  unsigned int oldRowSize = org_XSize;//The old row size
  //The two former variables are used to determine when to pad, while
  //looping through the data copying process.

  //JMI: Is it not correct that all blocks must be of the same size
  //(this is actually, more correctly put, an effect of using padding)

  //Calculate the new row size
  x_cuts++;
  y_cuts++;
  unsigned int x_cuts_divide = x_cuts;
  //if(x_cuts_divide == 0){x_cuts_divide = 1;}
  printf("TEST................... org_XSize and x_cuts_divide: %f, %f", (float)(org_XSize*4),(float)x_cuts_divide);
  int tempRowSize = ceil(((float)(org_XSize*4)/(float)x_cuts_divide));//messaured in bytes
  //we must find the closest upper int that is aligned from the tempRowSize
  while((tempRowSize%alignment)!=0)
    {
      tempRowSize++;
    }
  newRowSize = tempRowSize;//Still in bytes
  //Let's do the same for col-sizes, so each block has the same size.
  //Cols are not subject to alignment issues in 

  //This function does the following:
  //Malloc space for data blocks.
  //Decide the number of datablocks and sets metadata for each one in the extended PyArrayObject structure.
  
  //blocksize can be inferred.
  unsigned int blockSize = 0;//make all blocks equal size for now
  unsigned int numberOfBlocks = y_cuts*x_cuts;
  from->numberOfBlocks = numberOfBlocks;
  
  //calculate the blocksize
  unsigned int newColSize = 0;//In this process we need to know that new col size.
  //make the blocks equal size for now

  unsigned int y_cuts_divide = y_cuts;
  //if(y_cuts == 0){y_cuts_divide = 1;}

  //JMI:04/03/09 newColSize = ceil((float)org_ySize/(float)y_cuts_divide);
  newColSize = newRowSize/4;

  blockSize = newColSize * newRowSize;

  //JMI:03/03/09 - change number of cols to make square block if needed.
  //In relation to this, change number of blocks as needed.
  //Also, check against max blocksize and modify...
  //(Also, check cases where there are too many blocks assigned because
  //of modification of cols.)

  //JMI:03/03/09 - test newcol size against org y and y-cuts and modify cuts is needed
  unsigned int new_y_cuts = y_cuts;
  newColSize = newRowSize/4;//Assuming ints
  if(newColSize*new_y_cuts<org_ySize)
    {
      //We need more blocks to support square blocks
      while(newColSize*new_y_cuts<org_ySize)
	{
	  new_y_cuts++;
	}
    }
  else
    {
      //We have enough blocks - do something here, if you
      //want to make sure we're not wasting space on unsused blocks.
      //JMI: 05/03/09 - make sure that there's only on row of blocks with padding
      //(not thinking of the right-side "last block padding" of course)
      //See if blocks looks ok after this change.
      //Pt the array([[1,2,3,4,5,6,7,8,9],[1,2,3,4,5,6,7,8,9],[1,2,3,4,5,6,7,8,9],[1,2,3,4,5,6,7,8,9]])
      //looks like this:
    }
  y_cuts = new_y_cuts;
  numberOfBlocks = y_cuts*x_cuts;

  oldRowSize = org_XSize;

  newRowSize = 16;//set this with a define or something

  
  newColSize = 4;//set this with a define or something

  //decide the number of blocks needed
  unsigned int blocksInRow = 0;
  unsigned int blocksInCol = 0;

  unsigned int test2 = 12%16;
  
  blocksInRow = (org_XSize*4)/newRowSize;
  
  if(!((org_XSize*4)%newRowSize==0))
    {
      unsigned int test = (12%16);
      
      blocksInRow++;
    }
  blocksInCol = org_ySize/newColSize;
  if(!(org_ySize%newColSize==0))
    {
      blocksInCol++;
    }
  
  numberOfBlocks = blocksInRow*blocksInCol;

  blockSize = newRowSize*newColSize;
  x_cuts = blocksInRow;
  y_cuts = blocksInCol;

  from->numberOfBlocks = numberOfBlocks;
  
  //Set variables for number of blocks per dim

  from->numberOfBlocksXDim = x_cuts;
  from->numberOfBlocksYDim = y_cuts;

  int i = 0;
  for(i = 0;i<numberOfBlocks;i++)
    {
      //malloc space for the block and store the pointer in the PyArrayObject
      //(note: be sure that each block is aligned/has room for padding!)
      //--change so it's aligned!
      from->blockData[i] = (char*)_malloc_align2(blockSize,4);
    }
    from->blockSize = 16;//This is now measured in ints and is is used everywhere we need matrix size information.

  //Copy data to the new structure :-)
  //We have:
  //1. pointer to original data
  //2. pointer to new data (moving though each block)
  //3. old row length
  //4. new row length
  //5. new col height

  //start the copy loop.
  //Iterate through each

  unsigned int blockCount = numberOfBlocks;
  unsigned int oldRowCount = oldRowSize;
  unsigned int newRowCount = newRowSize;
  unsigned int newColCount = newColSize;

  char *oldDataPointer = oldDataPointer;//JMI:30/03/09 - we must get this from the new argument pointer.
  char *newDataPointer = from->blockData[dataBlockIndex];//Pointing to the first block //JMI:30/03/09 - we must use the new argument to set index.
  
  char *pointerToZero = (char*)_malloc_align2(1,4);
  *pointerToZero = 0;

  //printf(".................................Printing old layout\n");
  //unsigned oldLength = 0;
  //for(oldLength = 0;oldLength<(org_ySize*org_XSize*4);oldLength++)
  //  {
  //    printf("%d",oldDataPointer[oldLength]);
  //  }
  printf("\n");

  unsigned oldRowCopySize = oldRowSize;

  //****************************************************************
  //JMI: Here we make our new copyfunction
  //****************************************************************

  //03/03/09: Make the new variables so that the blocks are square.
  //i.e. cols = ints pr. newrow (assuming 4 bytes pr int now).
  //Make sure this does only leaves one potential block at end and bottom
  //Is this ok? Answer: 

  //While this is ok, make sure that bottom are handled correctly (have "ghosts" row).
  //The last in rows should be ok...

  //While setting the block variables - make sure that blocks does not violate the max block
  //size constraint.

  //make sure all these are in bytes!
  unsigned int _oldRowLength = oldRowSize*4;//JMI:because we need it in bytes!
  unsigned int _newRowLength = newRowSize;
  unsigned int _newColLength = newRowSize/4;//JMI:03/03/09 newColSize;

  unsigned int _lastRowLength = _oldRowLength%_newRowLength;
  unsigned int _padding = _newRowLength - _lastRowLength;
  unsigned int _blocksPrX = x_cuts;
  unsigned int _blocksPrY = y_cuts;
  char *_pointerToZero = (char*)_malloc_align2(1,4);
  *_pointerToZero = (char)0;
  unsigned int _totalNumberOfBytesInOldRow = oldRowSize*4;

  printf("The constants are: \n");
  printf("_oldRowLength: %d\n",_oldRowLength);
  printf("_newRowLength: %d\n",_newRowLength);
  printf("_newColLength: %d\n",_newColLength);
  printf("_lastRowLength: %d\n",_lastRowLength);
  printf("_padding: %d\n",_padding);
  printf("_blocksPrX: %d\n",_blocksPrX);
  printf("_blocksPrY: %d\n",_blocksPrY);
  printf("_totalNumberOfBytesInOldRow: %d\n", _totalNumberOfBytesInOldRow);

  printf("_THE OLD COLSIZE WAS: %d\n",newColSize);
  
  //These variables are adjusting continually in the loop
  unsigned int _currentBlockX = 0;//from 0 to _blocksPrX-1
  unsigned int _currentBlockY = 0;//from 0 to _blocksPrY-1
  unsigned int _currentRowInBlock = 0;//from 0 to _newRowLength-1
  unsigned int _currentColInBlock = 0;//from 0 to _newColLength-1
  unsigned int _isLastBlockInRow = 0; //boolean. 0 = false.
  char *_oldDataPointer = dataPointerOld;//JMI:30/03/09 - here the pointer is again. Make sure it is controlled correctly.
  char *_originalOldDataPointer = dataPointerOriginal;//JMI!HERE 01/04/09 - must we include the originalDataPointer in the call?//We also set the pointer relative to this
  char *_newDataPointer = from->blockData[dataBlockIndex];//Pointing to the first block
  unsigned int _copyingIsDone = 0;//Flag indication whether the copying is done. 0 = false.

  unsigned int _numberOfElementsInCurrentRow = 0;
  printf("2.............................\n");
  //Set initial values (might need to include special cases here later)
  _copyingIsDone = 0;

  if(_blocksPrX>1)
    {
      _numberOfElementsInCurrentRow = _newRowLength;
      _isLastBlockInRow = 0;
    }
  else
    {
      if(_lastRowLength == 0)
	{
	  _numberOfElementsInCurrentRow = _newRowLength;
	}
      else
	{
	  _numberOfElementsInCurrentRow = _lastRowLength;
	}
      _isLastBlockInRow = 1;
    }
	
  _oldDataPointer = dataPointerOld;
  _newDataPointer = from->blockData[dataBlockIndex];
  
  _currentBlockX = 0;
  _currentBlockY = 0;
  _currentRowInBlock = 0;
  _currentColInBlock = 0;

  unsigned int numberOfColsInLastBlockRow = _newColLength - (org_ySize%_newColLength);
  //JMI:05/03/09 - Moved the setBlock variables initialization to here...
  unsigned int setBlockX = 0;
  unsigned int setBlockY = 0;

  
  //Start outer copy loop
  unsigned int _outerLoopCounterVar = 0;
  while(!_copyingIsDone)
    {
      for(_outerLoopCounterVar = 0; _outerLoopCounterVar<_numberOfElementsInCurrentRow;_outerLoopCounterVar++)
	{
	      //There's still more regular elements left
	      memcpy( (void*)_newDataPointer, (void*)_oldDataPointer, 1);
	      _newDataPointer++;
	      _oldDataPointer++;
	}
      if(_isLastBlockInRow || (_numberOfElementsInCurrentRow==0))
	{
	  unsigned int tempRowCount = _newRowLength -_numberOfElementsInCurrentRow;
	  unsigned int counterA = 0;
	  //Insert padding if we are inserting the last element
	  
	  for(counterA = 0; counterA < tempRowCount;counterA++)
	    {
	      memcpy( (void*)_newDataPointer, (void*)_pointerToZero, 1);
	      _newDataPointer++;
	    }
	}
      else
	{
	  //Do not insert any padding at all
	}
	  //Set the pointers and vars correctly for the next loop iteration
	  //(set _copyingIsDone also - this var is really not needed, since we
	  //always need to control the loop setting the other vars anyway.)

	  //Find out what block is set next, if any

	  //Set initial values (might need to include special cases here later)
	  if(((_blocksPrX-1)==(_currentBlockX))&&((_blocksPrY-1)==(_currentBlockY)&&(/*IsThereMoreCols?*/((_newColLength-1)==_currentColInBlock))))
	    {
	      //Remember to check if there are more cols!!!
	      _copyingIsDone = 1;
	    }
	  else
	    {
		  if(_currentColInBlock==(_newColLength-1))
		    {
		      //We just operated on the last col in the block - set data for the next one
		      
		      //Is the next block a lastrowblock?
		      if((_currentBlockX==(_blocksPrX-2))||(_blocksPrX==1))
			{
			  _isLastBlockInRow = 1;
			  if(_lastRowLength==0)
			    {
			      _numberOfElementsInCurrentRow = _newRowLength;
			    }
			  else
			    {
			      _numberOfElementsInCurrentRow = _lastRowLength;
			    }

			}
		      else
			{
			  _numberOfElementsInCurrentRow = _newRowLength;
			  _isLastBlockInRow = 0;
			}
		    }
		  else
		    {
		      //There's still more cols left in this block - set data for the current one
		      //Is the current block a lastrowblock?

		      if(_currentBlockX==(_blocksPrX-1))
			{
			  //lastrowblock
			  _isLastBlockInRow = 1;
			  if(_lastRowLength==0)
                            {
                              _numberOfElementsInCurrentRow = _newRowLength;
                            }
                          else
                            {
                              _numberOfElementsInCurrentRow = _lastRowLength;
                            }
			}
		      else
			{
			  //Not lastrowblock
			  _numberOfElementsInCurrentRow = _newRowLength;
                          _isLastBlockInRow = 0;
			}
		    }
	    }
	  
	  //Are we moving to the next dataBlock?
	  if(((_newColLength-1)==_currentColInBlock))
	    {
	      //We are moving to the next block

	      _currentColInBlock = 0;

	      //Set _oldDataPointer to point correctly
	      //Set pointer to point to first row of next block!
	      //This must propably be calculated.
	      //Calculate the oldDataPointer based on the originalOldDataPointer
	      //char *_oldDataPointer = from->data;
          //char *_originalOldDataPointer = from->data;/

	      //Calculate the y,x we have to deal with next
	      
	      //Find the offset from org to the begging of this data
	      unsigned int bytesIntoOldRow = 0;
	      
	      //Set bytesIntoOldRow with a nice loop...
	      unsigned int counterB = 0;
	      for(counterB = 0;counterB<(_currentBlockX+1);counterB++)
		{
			
		  if((_currentBlockX+1)==_blocksPrX)
		    {
		      //It's the last block
		      bytesIntoOldRow = bytesIntoOldRow + _lastRowLength;
		    }
		  else
		    {
		      //It's not the last block
		      bytesIntoOldRow = bytesIntoOldRow + _newRowLength;
		    }
		}
	      //To be able to set the oldDataPointer easily:
	      unsigned int sizeOfRegularBlock = 0;
	      unsigned int sizeOfLastBlock = 0;
	      unsigned int colsPerBlock = 0;

	      //Note: For now we assume equal col length per block and at least one block!!!
	      //(There may be a problem with the last row of blocks, if blocksizes differ)     
	      colsPerBlock = from->blockSize;

	      sizeOfRegularBlock = colsPerBlock * _newRowLength;
	      sizeOfLastBlock = colsPerBlock * _lastRowLength;

	      //JMI:05/03/09 - Are setBlockY and setBlockX initialized correctly?
	      if((_currentBlockX+1) == _blocksPrX)
		{
		  setBlockY = _currentBlockY + 1;
		  setBlockX = 0;
		}
	      else
		{
		  setBlockY = _currentBlockY;
		  setBlockX++;
		}
	      //unsigned int doTheNewPointerMove = 1;//JMI - Added 24/02/09. Decide if new movement routine is used
	      

	      //Find XBlock
	      if(((_currentBlockX+1)==_blocksPrX)||(_blocksPrX==1))
		{
		  //Next block is Y block
		  //Set the pointer based on which block we need to hit next
		  
		      //Do the new move
		      //Find out how to set block (setBlockX,setBlockY)
		      if((_currentBlockX==setBlockX)||((!(_currentBlockX==setBlockX)&&(!_currentBlockY==setBlockY))))
			{
			  //Move to block directly below
			  //_oldDataPointer++;
			}
		      
		      if(_currentBlockY==setBlockY)
			{
			  //Move to block to the right
			  //In this case we must add all blocksizes before the current
			  //row of blocks + rowLength times blocks before setBlockX in current block row.
			  
			  //Number of blockRows before this one
			  unsigned blockRowsBefore = _blocksPrY - (_blocksPrY - _currentBlockY);
			  unsigned sizeOfDataInPreviousBlocks =
			    blockRowsBefore*((_blocksPrX-1)*sizeOfRegularBlock + sizeOfLastBlock);
			  unsigned sizeOfDataInThisBlockRow =
			    (_blocksPrX - (_blocksPrX - setBlockX)) * _newRowLength;
			  
			  _oldDataPointer = _originalOldDataPointer +
			    sizeOfDataInPreviousBlocks +
			    sizeOfDataInThisBlockRow;
			}
		}
	      else
		{
		  //Next block is X block
          //Do the new move
          //Find out how to set block (setBlockX,setBlockY)
          if((_currentBlockX==setBlockX)||((!(_currentBlockX==setBlockX)&&(!_currentBlockY==setBlockY))))
             {
               //Move to block directly below
               _oldDataPointer++;
             }

          if(_currentBlockY==setBlockY)
             {
             //Move to block to the right
             //In this case we must add all blocksizes before the current
             //row of blocks + rowLength times blocks before setBlockX in current block row.
             //Number of blockRows before this one
                 unsigned blockRowsBefore = _blocksPrY - (_blocksPrY - _currentBlockY);
                 unsigned sizeOfDataInPreviousBlocks =
                        blockRowsBefore*((_blocksPrX-1)*sizeOfRegularBlock + sizeOfLastBlock);
                 unsigned sizeOfDataInThisBlockRow =
                        (_blocksPrX - (_blocksPrX - setBlockX)) * _newRowLength;

			     _oldDataPointer = _originalOldDataPointer +
                            sizeOfDataInPreviousBlocks +
                            sizeOfDataInThisBlockRow;
			  }
		}
	      //Set _newDataPointer to point correctly. JMI: Check for correctness!
	      //In theory this pointer should never be moved like this, becuase it should not necesarry,
	      //on the account that the datablocks allocated lie next to each other.
	      _newDataPointer = from->blockData[(_blocksPrX*_currentBlockY+_currentBlockX+1)+dataBlockIndex];
	      //Set current block information
	      if((_blocksPrX-1)==_currentBlockX)
		{
		  _currentBlockX = 0;
		  _currentBlockY++;
		}
	      else
		{
		  _currentBlockX++;
		}
	    }
	  else
	    {
	      //We are not moving to the next block
	      _currentColInBlock++;

	      unsigned int rowsLeft = _blocksPrX - (_currentBlockX+1);
	      
	      //increment oldDataPointer (it ok to increment the non-original datapointer here)
	      //Note!!! This increment is not constant!
	      //It must always go to the next column of the current block!

	      unsigned int constantElementsPerRow = 0;
	      unsigned int _tempLastRowLength = 0;
	      if(_lastRowLength == 0)
		{
		  _tempLastRowLength = _newRowLength;
		}
	      else
		{
		  _tempLastRowLength = _lastRowLength;
		}
	      constantElementsPerRow = (_blocksPrX-1)*_newRowLength+_tempLastRowLength;

	      //Am I the last block?

	      //if yes, add blocks-1 times newRowLengtth

	      //if no, add blocks-2 times newRowLength plus lastRowLength

	      if(!rowsLeft)
		{
		  //Last block
		  _oldDataPointer = _oldDataPointer + (_blocksPrX-1)*_newRowLength;
		}
	      else
		{
		  //Not the last block
		  _oldDataPointer = _oldDataPointer + (_blocksPrX-2)*_newRowLength + _tempLastRowLength;
		  
		}
	    }

	  //JMI:04/03/09 - try to insert the new code here and see if it works...
	  unsigned int numberOfColsInLastRowBlock = org_ySize%_newColLength;
	  if(numberOfColsInLastRowBlock==0)
	    {
	      numberOfColsInLastRowBlock = _newColLength;
	    }
	  //JMI:05/03/09 - Here there's a problem. If zero, we normally don't have
	  //the that last block. So 4 might be a valid answer. There
	  if(_currentBlockY+1==_blocksPrY /*&& (_blocksPrY>1)*/)//If it's the last row of blocks
	    {
	      if((numberOfColsInLastRowBlock<=_currentColInBlock))
		{
		  //The rest of the cols should be padding
		  _numberOfElementsInCurrentRow = 0;//This should make the entire row be filled with padding.
		}
	    }
    }

  //print out the new data layout:
  unsigned int length2 = 0;
  unsigned int block2 = dataBlockIndex;
  unsigned int outerLoopCounter = 0;

  for(outerLoopCounter = 0;outerLoopCounter<numberOfBlocks;outerLoopCounter++)
    {
      printf("numberOfBlocks: %d\n",numberOfBlocks);
      printf("blockSize: %d\n",blockSize);
      printf(".............Print block: %d\n", block2);
      for(length2 = 0; length2<blockSize;length2++)
	{
	  printf("%d",from->blockData[block2][length2]);
	}
      printf("\n");
      block2++;
    }
}

void copyDataToNewLayout(PyArrayObject *from)
{
  //Copy the data from the old to the new format using the stored metadata.
  //Deallocate the old data area (if possible).
}

void SetMetaDataArray(PyArrayObject ndarray)
{
	//Calculate the number of blocks and allocate the array.
	//Decide how many blocks are needed per 2d instance.
	//Multiply up, so it mathces the total ndarray.
}

unsigned int NumberOfBlocksIn2D(unsigned int org_XSize, unsigned int org_ySize, PyArrayObject *ndarray)
{
  unsigned int oldRowSize;
  unsigned int newRowSize;
  unsigned int newColSize;
  unsigned int numberOfBlocks;
  
  oldRowSize = org_XSize;

  //tempRowSize = 0;

  newRowSize = ndarray->blockSize*4;//set this with a define or something
  newColSize = ndarray->blockSize;//set this with a define or something

  //decide the number of blocks needed
  unsigned int blocksInRow = 0;
  unsigned int blocksInCol = 0;

  blocksInRow = (org_XSize*4)/newRowSize;
 
  if(!((org_XSize*4)%newRowSize==0))
    {
      
      unsigned int test = (12%16);
      
      blocksInRow++;
    }
  blocksInCol = org_ySize/newColSize;
  if(!(org_ySize%newColSize==0))
    {
      blocksInCol++;
    }

  numberOfBlocks = blocksInRow*blocksInCol;
  
  return numberOfBlocks;
}

void SetPaddingInfoInTheNDArray()
{
	//Set the two variable paddingX and paddingY on the NDArray.
	
}

//JMI:07/04/09 - we must create the metadataarea for datablocks before the leaf-calls to
//this funtion. We do this in the beginning and use the index and size arguments to
//decide where to set the relevant pointers to data.
//Also: remember new stuff in the PyArrayObject:
//1. Info on index and size.

void numpy2FHB(PyArrayObject *from, PyArrayObject *to, unsigned int index)
{
  //Sizes in bytes
  int _maxBlockSize = 64;//This should probably be more like 45000 IRL.

  //Find size of data area in c-order numpy array
  int dataSize = 0;
  int numberOfDims = from->nd;

  if(numberOfDims>0)
    {
      dataSize = 1;
    }
  int i;
  for(i = 0; from->nd>i;i++)
    {
      dataSize = dataSize*from->dimensions[i];
    }
  
  //Find the two last dimensions (x,y) and list their sizes.
  //NB! Remember to make special case, when only one dimension exists!!!
  unsigned int ysize = from->dimensions[numberOfDims-2];
  unsigned int xsize = from->dimensions[numberOfDims-1];
  
  unsigned int physicalBlockSize = 0;
  physicalBlockSize = ysize * xsize * 4;
  float y_x_relation = 0;
  float block_relation = 0;
  y_x_relation = (float)ysize/(float)xsize;
  block_relation = (float)physicalBlockSize/(float)_maxBlockSize;
  
  unsigned int number_of_y_cuts = 0;
  unsigned int number_of_x_cuts = 0;
  
  //These two are used for determining the size of the original datablock sizes
  //(the size as seen from the oldDataPointer's perspective),
  //and the size of the original lastblocksize.
  //This information is used when setting the oldDataPointer to point
  //to the beggining of a new datablock.
  unsigned int originalX = 0;
  unsigned int originalY = 0;

  findXYCuts(ysize, xsize, _maxBlockSize, &number_of_y_cuts, &number_of_x_cuts, &originalX, &originalY);  
  
//These variables must be set correctly.... This is purely for testing
char *oldies = from->data+index;
char *oldiesOriginal = from->data+index;
//unsigned int blocksIndex = 0;
//unsigned int indexLength = 16;

unsigned int indexIntoArrayOfBlocks = NumberOfBlocksIn2D(from->dimensions[2], from->dimensions[1], from);

SetPaddingInfoInTheNDArray(from);

  //Create datablocks and copy data to new layout
  createDataBlocks(number_of_y_cuts, number_of_x_cuts, from, getSizeOfNDArray(from), ysize, xsize, oldies,indexIntoArrayOfBlocks,oldiesOriginal);
}


void createTestArray(PyArrayObject *pyArrayObject)
{
	//For now we create a 3d array and test with that.
	unsigned int numberOfData = 20;
	
	pyArrayObject->dimensions = _malloc_align2(4*3,7);
	pyArrayObject->dimensions[0] = 2;
	pyArrayObject->dimensions[1] = 2;
	pyArrayObject->dimensions[2] = 5;
	
	pyArrayObject->strides = _malloc_align2(4*3,7);
	pyArrayObject->strides[0] = 40;
	pyArrayObject->strides[1] = 20;
	pyArrayObject->strides[2] = 4;
	
	pyArrayObject->nd = 3;
	
	pyArrayObject->blockSize = 4;
	
	pyArrayObject->data = _malloc_align2(4*numberOfData,7);
	
	unsigned int i = 0;
	unsigned int j = 3;
	for(i = 0 ; i<numberOfData ; i++)
	{
		pyArrayObject->data[j] = i;
		j=j+4;
	}
	
	//Create datablocks for the ndarray:
	pyArrayObject->blockData = _malloc_align2(sizeof(char*)*4,7);
	pyArrayObject->blockData[0] = _malloc_align2(64,7);
	pyArrayObject->blockData[1] = _malloc_align2(64,7);
	pyArrayObject->blockData[2] = _malloc_align2(64,7);
	pyArrayObject->blockData[3] = _malloc_align2(64,7);
}

void SetMetaDataForNDArrayManipulation(PyArrayObject *pyArrayObject)
{
	numberOfDimensions = pyArrayObject->nd;
	unsigned int i;
	for(i = 0; i<numberOfDimensions;i++)
	{
		sizesOfDimensions[i] = pyArrayObject->strides[i];
		instancesOfDimensions[i] = pyArrayObject->dimensions[i];
	}
	
}

unsigned int findDimensionIndex(unsigned int dimension)
{
	//dimension argument is measured from 0 to n, with n being the highest dimension.
	return (numberOfDimensions - dimension);
}

//the relevant blocks are. The length can be seen in the nice and polite array sizesOfDimensions.
//This function takes an nd-array and creates 2d blocks from it.
void NDArrayToFHB(PyArrayObject *ndarray, unsigned int dimension, unsigned int index)
{
	unsigned int dimensionIndex = findDimensionIndex(dimension);
	
	unsigned int i = 0;
	unsigned int newIndex = 0;
	unsigned int size = 0;
	//Call the function recursively with new dimension and index

	if(dimension==2)
	{
		size = ndarray->strides[dimensionIndex]*ndarray->dimensions[dimensionIndex];
		//Call our regular numpy2FHB function
		numpy2FHB(ndarray,ndarray,index);
	}
	else
	{
		for(i = 0; i < ndarray->dimensions[dimensionIndex];i++)
		{
			NDArrayToFHB(ndarray, dimension-1,newIndex);
		
			//increment the index
			newIndex = newIndex + ndarray->strides[dimensionIndex];
		}
	}
}

int main ()
{
	//Create PyArrayObject to use for testing numpy2FHB
	PyArrayObject *pyArrayObject = _malloc_align2(sizeof(PyArrayObject),7);
	createTestArray(pyArrayObject);
	SetMetaDataForNDArrayManipulation(pyArrayObject);
	unsigned int dimension = 2;
	unsigned int index = 0;
	NDArrayToFHB(pyArrayObject,3,0);
	printf("5\n");
	
	return 1;
}
