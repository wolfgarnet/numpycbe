#include <Python.h>
#include <stdio.h>
#include <malloc.h>
#include <time.h>

#include <stdlib.h>
#include <sys/time.h>
#include <math.h>
#include <timer.h>

#include <malloc_align.h>

#include <stdlib.h>
#include <string.h>


#include "include/common.h"
#include "include/common_ppu.h"
#include "include/libppu.h"
#include <fhbtool.h>

// Shaders
#include <shaders.h>


// Solve
//#include <solve.h>

//
#include <runshader.h>



unsigned int speThreads = 6;
//#define MAX_SPE_THREADS 6
#define MAX_SPU_NUM 6

#define _USE_NUM_SPES 6




unsigned int _numberOfBlocks = 64;
unsigned int _blockSize = 64;

//float resultFloats[MAX_SPU_NUM][4] __attribute__((aligned(16)));
//
//unsigned int spe_pointer_addr[MAX_SPU_NUM][4] __attribute__((aligned(16)));
//PpuPthreadData_t speData[MAX_SPU_NUM];

typedef struct
{
	int a;
	int b;
}PyArrayObjectTest;

//Temp sliceObject - we only need one for SOR - thats why...
SliceObject *sliceObject;

// Helpers

#define _OPS_ARRAYARRAY  0
#define _OPS_ARRAYSCALAR 1
#define _OPS_SCALARARRAY 2

// 000 = array array
// 001 = array scalar
// 010 = scalar array
inline unsigned int _DetermineShaderType( PyArrayObject *op1, PyArrayObject *op2 )
{
	unsigned int res = 0;

	//Op1 is vector
	if(op1->nd==0)
	{
		res ^= 2;
	}

	//Op2 is vector
	if(op2->nd==0)
	{
		res ^= 1;
	}

	// Op1 is vector
	//if( op1->dimensions[0] == 1 || op1->dimensions[1] == 1 )
	//{
	//	res ^= 1;
	//}

	// Op2 is vector
	//if( op2->dimensions[0] == 1 || op2->dimensions[1] == 1 )
	//{
	//	res ^= 2;
	//}

	return res;
}

void testFunction(unsigned int a)
{
  //printf("testFunction is called with unsigned int: %d\n",a);
}

PyObject *MyCommand(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;
  long a, b;

  if (PyArg_ParseTuple(args, "ii", &a, &b)) {
    result = Py_BuildValue("i", a + b);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //Call something interesting
  testFunction(5);

  return result;
}

PyObject *_initializeVector(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  long a;

  PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  result = Py_BuildValue("i", resultObj);
  /* otherwise there is an error,
   * the exception already raised by PyArg_ParseTuple, and NULL is
   * returned.
   */

  //Setting the PyArrayObject
  resultObj->numberOfBlocks = _numberOfBlocks*_numberOfBlocks;
  resultObj->numberOfBlocksXDim = _numberOfBlocks;
  resultObj->numberOfBlocksYDim = _numberOfBlocks;
  resultObj->blockSize = _blockSize;//in elements
  resultObj->dimensions = (unsigned int*)_malloc_align(sizeof(unsigned int)*2,7);
  resultObj->dimensions[0] = 2;
  resultObj->dimensions[1] = 2;
  resultObj->nd = 1;
  resultObj->blockData = (char**)_malloc_align(sizeof(char*)*_numberOfBlocks*_numberOfBlocks,7);
  unsigned int i = 0;
  //printf("start blockData[0] : %d,%x\n",resultObj->blockData[0],resultObj->blockData[0]);
  for(i = 0;i<_numberOfBlocks*_numberOfBlocks;i++)
    {
      resultObj->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
      //printf("In loop\n");
      //printf("blockData[i] : %d,%x\n",resultObj->blockData[i],resultObj->blockData[i]);
      //printf("blockData[0] : %d,%x\n",resultObj->blockData[0],resultObj->blockData[0]);
      //printf("In loop out\n");
    }
  //printf("blockData[0] : %d,%x\n",resultObj->blockData[0],resultObj->blockData[0]);
  //printf("blockData[1] : %d,%x\n",resultObj->blockData[1],resultObj->blockData[1]);
  //printf("Based on sizes: %d and %d: \n",sizeof(char*)*_numberOfBlocks*_numberOfBlocks,sizeof(char)*_blockSize*_blockSize*4);

  //Fill zeros in the block
  //unsigned int k;
  //for(k=0;k<_numberOfBlocksX*_numberOfBlocksY;k++)
  //  {
  //    for(i=0;i<_blockSize*_blockSize;i++)
  //	{
  //	  ((float*)resultObj->blockData[k])[i] = 1;
  //	}
  // }


  return result;
}


/*
 * Set blocksize
 */

PyObject * _setblocksize( PyObject *self, PyObject *args )
{

	unsigned int size;

    if (!PyArg_ParseTuple(args, "l", &size))
    {
        return;
    }

    printf( "Current block size is now %u\n", size );

    _blockSize = size;

    return Py_None;
}


/*
 * Shape
 */
PyObject *_Shape( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;
	long a;

	if ( !PyArg_ParseTuple(args, "i", &a ) )
	{
		return Py_None;
	}

	PyArrayObject *obj1 = (PyArrayObject*)a;
	PyArrayObject *obj2 = _malloc_align( sizeof( PyArrayObject ), 7 );

	sMakeFHB_shape( obj1, obj2 );

	result = Py_BuildValue( "i", (long)obj2 );

	return result;
}


/*
 * ARANGE
 */

PyObject *_arange( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;
	//unsigned int size;
	float v1, v2, v3;

    if ( !PyArg_ParseTuple( args, "f|ff", &v1, &v2, &v3 ) )
    {
        return NULL;
    }


	PyArrayObject *resultObj = _malloc_align( sizeof( PyArrayObject ), 7 );
	result = Py_BuildValue( "i", (long)resultObj );
	/* otherwise there is an error,
	* the exception already raised by PyArg_ParseTuple, and NULL is
	* returned.
	*/


	//sMakeFHB( 1, size, _blockSize, 1.0f, resultObj );
	sMakeFHB_arange( _blockSize, v1, v2, v3, resultObj );

	return result;
}

/*
 * ARRAY
 */

PyObject *_array( PyObject *self, PyObject *args )
{
	PyObject *result = NULL, *a;
	//unsigned int size;

    if ( !PyArg_ParseTuple( args, "O!", &a ) )
    {
        return NULL;
    }


	PyArrayObject *resultObj = _malloc_align( sizeof( PyArrayObject ), 7 );
	result = Py_BuildValue( "i", (long)resultObj );



	//sMakeFHB( 1, size, _blockSize, 1.0f, resultObj );
	//sMakeFHB_arange( _blockSize, v1, v2, v3, resultObj );

	return result;
}

/*
 * CREATE for SOR. JMI:Temporary create for testing while developing SOR. Should be replaced with somethin else.
 */

PyObject *_createSOR(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  long numBlocksX;
  long numBlocksY;

  PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  if (PyArg_ParseTuple(args, "ii", &numBlocksX, &numBlocksY)) {
    result = Py_BuildValue("i", resultObj);
  }
  /* otherwise there is an error,
   * the exception already raised by PyArg_ParseTuple, and NULL is
   * returned.
   */

  //Setting the PyArrayObject
  resultObj->numberOfBlocks = numBlocksX*numBlocksY;
  resultObj->numberOfBlocksXDim = numBlocksX;
  resultObj->numberOfBlocksYDim = numBlocksY;
  resultObj->blockSize = _blockSize;//in elements
  resultObj->nd = 2;
  resultObj->blockData = (char**)_malloc_align(sizeof(char*)*numBlocksX*numBlocksY,7);
  unsigned int i = 0;
  //printf("start blockData[0] : %d,%x\n",resultObj->blockData[0],resultObj->blockData[0]);
  for(i = 0;i<numBlocksX*numBlocksY;i++)
    {
      resultObj->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
      //printf("In loop\n");
      //printf("blockData[i] : %d,%x\n",resultObj->blockData[i],resultObj->blockData[i]);
      //printf("blockData[0] : %d,%x\n",resultObj->blockData[0],resultObj->blockData[0]);
      //printf("In loop out\n");
    }
  //printf("blockData[0] : %d,%x\n",resultObj->blockData[0],resultObj->blockData[0]);
  //printf("blockData[1] : %d,%x\n",resultObj->blockData[1],resultObj->blockData[1]);
  //printf("Based on sizes: %d and %d: \n",sizeof(char*)*_numberOfBlocks*_numberOfBlocks,sizeof(char)*_blockSize*_block\
    Size*\4);

//Fill zeros in the block
unsigned int k;
for(k=0;k<numBlocksX*numBlocksY;k++)
  {
	((float*)resultObj->blockData[k])[0] = 4;
	((float*)resultObj->blockData[k])[1] = 1;
	((float*)resultObj->blockData[k])[2] = 1;
	((float*)resultObj->blockData[k])[3] = 1;
	((float*)resultObj->blockData[k])[4] = 4;
	((float*)resultObj->blockData[k])[5] = 1;
	((float*)resultObj->blockData[k])[6] = 1;
	((float*)resultObj->blockData[k])[7] = 1;
	((float*)resultObj->blockData[k])[8] = 4;
	((float*)resultObj->blockData[k])[9] = 1;
	((float*)resultObj->blockData[k])[10] = 1;
	((float*)resultObj->blockData[k])[11] = 1;
	((float*)resultObj->blockData[k])[12] = 4;
	((float*)resultObj->blockData[k])[13] = 1;
	((float*)resultObj->blockData[k])[14] = 1;
	((float*)resultObj->blockData[k])[15] = 1;
    //for(i=0;i<_blockSize*_blockSize;i++)
    //  {
    //    ((float*)resultObj->blockData[k])[i] = k+i+1;
    //  }
  }

//A=array((4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,   4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1,4,1,1,1),float)





return result;
}

/*
 * EQUALSOR
 */

PyObject *_equalSOR( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;
	long a;
	long b;

    if ( !PyArg_ParseTuple(args, "ll", &a, &b) )
    {
        return NULL;
    }

	/* otherwise there is an error,
	* the exception already raised by PyArg_ParseTuple, and NULL is
	* returned.
	*/

	//Set values in a equal to values in b
	unsigned _numBlocks = ((PyArrayObject*)a)->numberOfBlocks;
	unsigned _blockSize = ((PyArrayObject*)a)->blockSize;
	unsigned int i;
	unsigned int j;
	for(i=0;i<_numBlocks;i++)
	{
		float *aFloatPointer = ((float*)((PyArrayObject*)a)->blockData[i]);
		float *bFloatPointer = ((float*)((PyArrayObject*)b)->blockData[i]);
		for(j=0;j<_blockSize*_blockSize;j++)
		{
			aFloatPointer[j] = bFloatPointer[j];
		}
	}
	return Py_BuildValue("");
}


/*
 * CREATE
 */

PyObject *_create( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;
	unsigned int x, y;
	float v;

    if ( !PyArg_ParseTuple(args, "llf", &x, &y, &v ) )
    {
        return NULL;
    }


	PyArrayObject *resultObj = _malloc_align( sizeof( PyArrayObject ), 7 );
	result = Py_BuildValue( "i", (long)resultObj );
	/* otherwise there is an error,
	* the exception already raised by PyArg_ParseTuple, and NULL is
	* returned.
	*/

	unsigned int bs2 = _blockSize * _blockSize;


	sMakeFHB( x, y, _blockSize, v, resultObj );

	return result;
}

/*
 * SLICEROW - JMI: Maybe this should be replaced with another function in this file?
 */

PyObject *_sliceRow(PyObject *self, PyObject *args)
{
  //printf("sliceRow\n");
  PyObject *result = NULL;

  long a;
  long b;

  if (PyArg_ParseTuple(args, "ii", &a, &b)) {
    result = Py_BuildValue("i", a);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //Slice PyArrayObject
  //printf("Calculating row slice:\n");
  //printf("blockRow is: %d\n",b/((PyArrayObject*)a)->blockSize);
  //printf("row in block: %d\n",b%((PyArrayObject*)a)->blockSize);

  //printf("startBlock: %d\n",(b/((PyArrayObject*)a)->blockSize)*((PyArrayObject*)a)->numberOfBlocksXDim);

  //This checks out ok. Now, set the sliceObject accordingly on the PyArrayObject



  //example of sliceobject:
  //typedef struct
  //{
  //  unsigned int slice_type;//0:row, 1:col
  //  unsigned int slice_start;//start row/col
  //  unsigned int slice_end;//end row/col
    //Is it also possible to slice only half a row etc.?
  //  unsigned int padding;
  //} SliceObject __attribute__((aligned (16)));

  //Moved to the beginning og the file.
  //SliceObject *sliceObject = (SliceObject*)_malloc_align(sizeof(SliceObject),7);

  //Set variables:
  sliceObject->slice_type = 0;
  sliceObject->slice_start = b%((PyArrayObject*)a)->blockSize;
  sliceObject->slice_end = b%((PyArrayObject*)a)->blockSize;
  sliceObject->slice_startBlock = (b/((PyArrayObject*)a)->blockSize)*((PyArrayObject*)a)->numberOfBlocksXDim;

  ((PyArrayObject*)a)->slice_info = sliceObject;
  //printf("blockSize on A: %d\n",((PyArrayObject*)a)->blockSize);
  return result;

}

/*
 * CREATEVECTOR - JMI: This should probably be replaced by another function in this file. It is used for developing SOR.
 */

PyObject *_createVector(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  long numBlocksX;
  long numBlocksY;

  PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  if (PyArg_ParseTuple(args, "ii", &numBlocksX, &numBlocksY)) {
    result = Py_BuildValue("i", resultObj);
  }
  /* otherwise there is an error,
   * the exception already raised by PyArg_ParseTuple, and NULL is
   * returned.
   */

  //Setting the PyArrayObject
  resultObj->numberOfBlocks = 1;
  resultObj->numberOfBlocksXDim = 1;
  resultObj->numberOfBlocksYDim = 1;
  resultObj->blockSize = _blockSize;//in elements
  resultObj->nd = 1;
  resultObj->blockData = (char**)_malloc_align(sizeof(char*)*numBlocksX*numBlocksY,7);
  resultObj->dimensions = (unsigned int*)_malloc_align(sizeof(unsigned int)*resultObj->nd,7);
  resultObj->dimensions[0] = _blockSize*_blockSize;//JMI: This should be controlled differently for SOR!
  unsigned int i = 0;

  for(i = 0;i<1;i++)
    {
      resultObj->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
    }

  //Fill zeros in the block
  unsigned int k;
  for(k=0;k<1;k++)
    {
	((float*)resultObj->blockData[k])[0] = 1;
	((float*)resultObj->blockData[k])[1] = 2;
	((float*)resultObj->blockData[k])[2] = 3;
	((float*)resultObj->blockData[k])[3] = 4;
	((float*)resultObj->blockData[k])[4] = 1;
	((float*)resultObj->blockData[k])[5] = 2;
	((float*)resultObj->blockData[k])[6] = 3;
	((float*)resultObj->blockData[k])[7] = 4;
      	//for(i=0;i<_blockSize*_blockSize;i++)
	//{
	//((float*)resultObj->blockData[k])[i] = k+i;
	//}
    }

return result;
}

/*
 * INTERNAL SLICE MULT - //JMI: This should be merged with the regular mult function in this file. Used in SOR.
 */

void internal_sliceMult(long a, long b, long c)
{
  //printf("sliceMult\n");
  //printf("Inside internal mult\n");
  //Setting the Operation object
  Operation_t op1;
  op1.shaderSize = arrayslicerow_arrayscalar_size;
  op1.EA_shader  = SHADER_arrayslicerow_arrayscalar;
  op1.obj[0]     = a;
  op1.obj[1]     = b;
  op1.obj[2]     = c;

  //printf("arraymultiply: %d\n",arraymultiply);
  //printf("&arraymultiply: %d\n",&arraymultiply);

  //printf("BLOCKDATA[0] IN A: %d\n",((PyArrayObject*)a)->blockData[0]);

  unsigned int speThreadsLocal = 2;

  op1.num_SPES   = speThreadsLocal;
  unsigned int i = 0;

  for(i = 0;i<speThreadsLocal;i++)
    {
      spe_pointer_addr[i][0] = &op1;
    }

  //printf("Adr of op1 sat. It is: %x\n",&op1);

  unsigned int state = 0;
  unsigned int y;

  for ( y = 0 ; y < speThreadsLocal ; y++ )
    {
      //printf("Sending state to disp\n");
      spe_in_mbox_write ( speData[y].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
      //printf("TO THE DISP. THE SUM STATE: %d, has just been sent\n",&state);
      //printf("This is the actual value: %d\n",state);
    }

  //printf("MULT Waiting for the SPE's\n");//ALSO, check that the structure used is correct!
  // Waiting for SPEs!
  //printf( "Waiting for SPEs\n" );
  unsigned int checked = 0;
  unsigned int r;
  while( checked < speThreadsLocal )
    {
      if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
        {
          spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
          checked++;
          //printf("Something read on the inbox\n");
        }
    }
  //printf("Done waiting on threads to finish in SLICEMULT\n");

}

/*
 * SLICEMULT - JMI: This should be merged with the regular mult function in this file. Used in SOR.
 */

PyObject *_sliceMult(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //int a represents the number of elements we need in our random vector.
  long a;
  long b;
  long c;

  //PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  //internal_random(a,result);

  if (PyArg_ParseTuple(args, "iii", &a,&b,&c)) {
    result = Py_BuildValue("i", c);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

//  //printf("Inside mult: adr. of PyArrayObject is: %x,%d\n",a,a);
//  if( SHADER_status[SHADER_arrayslicerow_arrayscalar_id] > 100 )
//  {
	 internal_sliceMult(a,b,c);
//	  SHADER_status[SHADER_arrayslicerow_arrayscalar_id] = 0;
//  }
//  else
//  {
//	  run30nu( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)c, SHADER_status[SHADER_arrayslicerow_arrayscalar_id] );
//  }

  //printf("Before returning value\n");

  return result;

}

PyObject *_Print( PyObject *self, PyObject *args )
{
	long a;


	if (!PyArg_ParseTuple(args, "i", &a))
	{
		return;
	}

	PyArrayObject *o = (PyArrayObject*)a;

	PrintNDArray( o, 1 );

	return Py_None;
}

PyObject *_PrintSci( PyObject *self, PyObject *args )
{
	long a;


	if (!PyArg_ParseTuple(args, "i", &a))
	{
		return;
	}

	PyArrayObject *o = (PyArrayObject*)a;

	PrintNDArray( o, 2 );

	return Py_None;
}

/*
 * TEST dispatcher
 */
PyObject *_testDispatcher1(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  long a = 0,b = 0,c = 0;

  printf( "1\n" );

  //printf("Inside mult: adr. of PyArrayObject is: %x,%d\n",a,a);
  if( SHADER_status[SHADER_dispatcher_test_id] > 100 )
  {
	  run30nr( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)c, strsm_llnu_size, SHADER_strsm_llnu, 0 );
	  //run21( obj1, obj2, ALPHA, strsm_llnu_size, SHADER_strsm_llnu );
	  SHADER_status[SHADER_dispatcher_test_id] = 0;
  }
  else
  {
	  run30nu( SHADER_status[SHADER_dispatcher_test_id] );
  }

  //printf("Before returning value\n");

  return result;

}

PyObject *_testDispatcher2(PyObject *self, PyObject *args)
{
  PyObject *result = Py_BuildValue("");

  long a = 0,b = 0,c = 0;

  run30nr( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)c, arrayadd_size, SHADER_arrayadd, 0 );
  SHADER_status[SHADER_dispatcher_test_id] = 0;

  //printf("Before returning value\n");

  return result;

}



PyObject *getcbe(PyObject *self, PyObject *args)
{
  //PyArrayObject *resultObj = _malloc_align2(sizeof(PyArrayObject),7);
  PyObject *result = NULL;

  //We read a, but so far we don't use it.
  long a;


  if (PyArg_ParseTuple(args, "i", &a)) {
    result = Py_BuildValue("i", ((PyArrayObjectTest*)a)->b);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //Call something interesting
  //testFunction(5);

  return result;
}


PyObject *setcbe(PyObject *self, PyObject *args)
{
  PyArrayObjectTest *resultObj = _malloc_align(sizeof(PyArrayObjectTest),7);
  PyObject *result = NULL;

  //We read a, but so far we don't use it.
  long a;

  resultObj->a = 7;
  resultObj->b = 8;



  if (PyArg_ParseTuple(args, "i", &a)) {
    result = Py_BuildValue("i", resultObj);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //Call something interesting
  //testFunction(5);

  return result;
}

PyObject *_readBlockSize(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //We read a, but so far we don't use it.
  long a;

  if (PyArg_ParseTuple(args, "i", &a)) {
    result = Py_BuildValue("i", ((PyArrayObject*)a)->blockSize);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */
  return result;

}

/***************************
 **
 ** FUNCTIONALITY
 **
 ***************************/



/*
 * Random
 */

PyObject *_random(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if ( PyArg_ParseTuple( args, "i", &a ) )
	{
		result = Py_BuildValue( "i", a );
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//internal_random( (PyArrayObject*)a );
	run10( (PyArrayObject*)a, randomfiller_size, SHADER_randomfiller );

	return result;
}

/*
 * Zeros
 */

PyObject *_Zeros(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if ( PyArg_ParseTuple( args, "i", &a ) )
	{
		result = Py_BuildValue( "i", a );
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//internal_random( (PyArrayObject*)a );
	run10( (PyArrayObject*)a, zerofiller_size, SHADER_zerofiller );

	return result;
}


/*
 * Mult
 */


PyObject *_Multiply(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	long b;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "ii", &a,&b))
	{
		result = Py_BuildValue("i", a);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	PyArrayObject *op1 = (PyArrayObject*)a;
	PyArrayObject *op2 = (PyArrayObject*)b;

	// Dertemine the shader type
	unsigned int t = _DetermineShaderType( op1, op2 );

	//printf( "t=%u\n", t );

	switch( t )
	{
	case 0: // Array array
		//printf( "0\n" );
		run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)a, arraymultiply_size, SHADER_arraymultiply );
		break;
	case 1: // Array scalar
		//printf( "1\n" );
		run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)a, arraymultiply_arrayscalar_size, SHADER_arraymultiply_arrayscalar );
		break;
	case 2: // Scalar array
		//printf( "2\n" );
		run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)a, arraymultiply_scalararray_size, SHADER_arraymultiply_scalararray );
		break;
	}

	//printf("Before returning value\n");

	return result;

}

/*
 * MultSOR
 */


PyObject *_MultiplySOR(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	long b;
	long c;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "iii", &a,&b,&c))
	{
		result = Py_BuildValue("i", c);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	PyArrayObject *op1 = (PyArrayObject*)a;
	PyArrayObject *op2 = (PyArrayObject*)b;

	// Dertemine the shader type
	unsigned int t = _DetermineShaderType( op1, op2 );

	//printf( "t=%u\n", t );

	switch( t )
	{
	case 0: // Array array
		//printf( "0\n" );
		run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)c, arraymultiply_size, SHADER_arraymultiply );
		break;
	case 1: // Array scalar
		//printf( "1\n" );
		run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)c, arraymultiply_arrayscalar_size, SHADER_arraymultiply_arrayscalar );
		break;
	case 2: // Scalar array
		//printf( "2\n" );
		run30( (PyArrayObject*)b, (PyArrayObject*)a, (PyArrayObject*)c, arraymultiply_scalararray_size, SHADER_arraymultiply_scalararray );
		break;
	}

	//printf("Before returning value\n");

	return result;

}

/*
 * Divide
 */


PyObject *_Divide(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	long b;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "ii", &a,&b))
	{
		result = Py_BuildValue("i", a);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//printf("Inside mult: adr. of PyArrayObject is: %x,%d\n",a,a);
	//internal_mult((PyArrayObject*)a,(PyArrayObject*)b);
	run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)a, arraydivide_size, SHADER_arraydivide );

	//printf("Before returning value\n");

	return result;

}

/*
 * Add
 */


PyObject *_add(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	long b;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "ii", &a, &b))
	{
		result = Py_BuildValue("i", a);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",a,a);
	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",b,b);
	//internal_add((PyArrayObject*)a,(PyArrayObject*)b);
	run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)a, arrayadd_size, SHADER_arrayadd );

	//printf("Before returning value\n");

	return result;

}

/*
 * Add
 */


PyObject *_sub(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	long b;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "ii", &a, &b))
	{
		result = Py_BuildValue("i", a);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",a,a);
	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",b,b);
	//internal_add((PyArrayObject*)a,(PyArrayObject*)b);
	run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)a, arraysub_size, SHADER_arraysub );

	//printf("Before returning value\n");

	return result;

}

/*
 * AddSOR
 */


PyObject *_addSOR(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	long b;
	long c;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "iii", &a, &b, &c))
	{
		result = Py_BuildValue("i", c);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",a,a);
	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",b,b);
	//internal_add((PyArrayObject*)a,(PyArrayObject*)b);
	run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)c, arrayadd_size, SHADER_arrayadd );

	//printf("Before returning value\n");

	return result;

}

/*
 * SUBSOR
 */


PyObject *_subSOR(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	long b;
	long c;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "iii", &a, &b, &c))
	{
		result = Py_BuildValue("i", c);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",a,a);
	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",b,b);
	//internal_add((PyArrayObject*)a,(PyArrayObject*)b);
	run30( (PyArrayObject*)a, (PyArrayObject*)b, (PyArrayObject*)c, arraysub_size, SHADER_arraysub );

	//printf("Before returning value\n");

	return result;

}

/*
 * Division
 */

void internal_div(long a, long b)
{
  //printf("Inside internal add\n");
  //Setting the Operation object
  Operation_t op1;
//  op1.shaderSize = arraydivide_size;
//  op1.EA_shader  = arraydivide;
  op1.obj[0]     = a;
  op1.obj[1]     = b;
  op1.obj[2]     = a;

  //printf("arraymultiply: %d\n",arraydivide);
  //printf("&arraymultiply: %d\n",&arraydivide);

  op1.num_SPES   = speThreads;
  unsigned int i = 0;

  for(i = 0;i<speThreads;i++)
    {
      spe_pointer_addr[i][0] = &op1;
    }

  //printf("Adr of op1 sat. It is: %x\n",&op1);

  unsigned int state = 1;
  unsigned int y;

  for ( y = 0 ; y < speThreads ; y++ )
    {
      //printf("Sending state to disp\n");
      spe_in_mbox_write ( speData[y].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
      //printf("TO THE DISP. THE SUM STATE: %d, has just been sent\n",&state);
      //printf("This is the actual value: %d\n",state);
    }

  //printf("ADD Waiting for the SPE's\n");//ALSO, check that the structure used is correct!
  // Waiting for SPEs!
  //printf( "Waiting for SPEs\n" );
  unsigned int checked = 0;
  unsigned int r;
  while( checked < speThreads )
    {
      if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
        {
          spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
          checked++;
          //printf("Something read on the inbox\n");
        }
    }
  //printf("Done waiting on threads to finish in DIV\n");

}


PyObject *_div(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //int a represents the number of elements we need in our random vector.
  long a;
  long b;


  //PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  //internal_random(a,result);

  if (PyArg_ParseTuple(args, "ii", &a, &b)) {
    result = Py_BuildValue("i", a);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //printf("Inside div: adr. of PyArrayObject is: %x,%d\n",a,a);
  internal_div(a,b);

  //printf("Before returning value\n");

  return result;

}

/*
 * Less equal
 */


PyObject *_lessequal(PyObject *self, PyObject *args)
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	float b;


	if (PyArg_ParseTuple(args, "if", &a, &b))
	{
		result = Py_BuildValue("i", a);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	PyArrayObject r;
	sMakeFHB( 1, 1, _blockSize, b, &r );

	//internal_lessequal(a,b);
	run30( (PyArrayObject*)a, &r, (PyArrayObject*)a, arraylessequal_arrayscalar_size, SHADER_arraylessequal_arrayscalar );


	return result;

}


/*
 * Sum
 */


PyObject *_sum(PyObject *self, PyObject *args)
{
  //printf("sum\n");
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "i", &a))
	{
	/* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

		//printf("Inside div: adr. of PyArrayObject is: %x,%d\n",a,a);
		//unsigned int sumResult = internal_sum(a);
		float sumResult = run10r( (PyArrayObject*)a, arraysum_size, SHADER_arraysum );

		result = Py_BuildValue("f", sumResult);

		//printf("Before returning value\n");
	}

	return result;

}

/*
 * ABS - Not completed yet!
 */

PyObject *_abs(PyObject *self, PyObject *args)
{
  //printf("abs\n");
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "i", &a))
	{
	result = Py_BuildValue("i", a);
	/* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//Okay, here we start the abs-function
	//If possible, do this by flipping the sign-bit.
	//For now, do it the old-style way, but try changing it to bit-flipping if the overall performance is bad.
	//Just do it for the whole block for now - it should be okay based on the zero-filling,
	//however consider doing it for less than a block a time, if the performance is bad.

	unsigned int i;
	float zero = 0;
	float* myFloatPointer = ((float*)((PyArrayObject*)a)->blockData[0]);
	for(i=0;i<_blockSize*_blockSize;i++)
	{
		if(myFloatPointer[i] < zero)
		{
			myFloatPointer[i] = myFloatPointer[i] * -1;
		}
	}


	}

	return result;

}

/*
 * MAX - Not completed yet!
 */

PyObject *_max(PyObject *self, PyObject *args)
{
  //printf("max\n");
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	float res;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "i", &a))
	{
	/* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	unsigned int i;
	float zero = 0;
	float* myFloatPointer = ((float*)((PyArrayObject*)a)->blockData[0]);
	float myBiggestFloat = -9999; //This should in fact be the smallest possible float value

	for(i=0;i<_blockSize*_blockSize;i++)
	{
		if(myFloatPointer[i] > myBiggestFloat)
		{
			myBiggestFloat = myFloatPointer[i];
		}
	}

	res = myBiggestFloat;
	result = Py_BuildValue("f", res);

	}

	return result;

}


/*
 * Get index
 */
PyObject *_GetIndexAt( PyObject *self, PyObject *args )
{
  //printf("GetIndexAt\n");
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a, x, y = 0;


	if ( !PyArg_ParseTuple(args, "ii|i", &a, &x, &y ) )
	{
		return Py_None;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	unsigned int col;
	unsigned int row;
	unsigned int block;
	unsigned int index;

	//Vector
	if(obj->nd==1)
	{
		index = x % ( obj->blockSize * obj->blockSize );
		block = x / ( obj->blockSize * obj->blockSize );
	}
	//Matrix
	else
	{
		col   = x % obj->blockSize;
		row   = y % obj->blockSize;
		block = y / obj->blockSize * obj->numberOfBlocksXDim + x / obj->blockSize;
		index = row*obj->blockSize+col;
	}

	// Vector
	//if( obj->dimensions[0] == 1 || obj->dimensions[1] == 1 )
	//{
	//	index = x % ( obj->blockSize * obj->blockSize );
	//	block = x / ( obj->blockSize * obj->blockSize );
	//}
	// Matrix
	//else
	//{
	//	col   = x % obj->blockSize;
	//	row   = y % obj->blockSize;
	//	block = y / obj->blockSize * obj->numberOfBlocksXDim + x / obj->blockSize;
	//	index = row*obj->blockSize+col;
	//}

	//printf( "x=%u, y=%u\n", x, y );
	//printf( "Block=%u, row=%u, col=%u\n", block, row, col );

	float r            = ((float*)obj->blockData[block])[index];

	//printf( "RES=%f\n", r );

	result = Py_BuildValue( "f", r );

	return result;
}

/*
 * Get index, like a matrix row, col
 */
PyObject *_GetIndexAtm( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a, row1, col1 = 0;


	if ( !PyArg_ParseTuple(args, "ii|i", &a, &row1, &col1 ) )
	{
		return Py_None;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	unsigned int col;
	unsigned int row;
	unsigned int block;
	unsigned int index;

	//Vector
	if(obj->nd==1)
	{
		index = col1 % ( obj->blockSize * obj->blockSize );
		block = col1 / ( obj->blockSize * obj->blockSize );
	}
	//Matrix
	else
	{
		col   = col1 % obj->blockSize;
		row   = row1 % obj->blockSize;
		block = row1 / obj->blockSize * obj->numberOfBlocksXDim + col1 / obj->blockSize;
		index = row*obj->blockSize+col;
	}

	// Vector
	//if( obj->dimensions[0] == 1 || obj->dimensions[1] == 1 )
	//{
	//	index = col1 % ( obj->blockSize * obj->blockSize );
	//	block = col1 / ( obj->blockSize * obj->blockSize );
	//}
	// Matrix
	//else
	//{
	//	col   = col1 % obj->blockSize;
	//	row   = row1 % obj->blockSize;
	//	block = row1 / obj->blockSize * obj->numberOfBlocksXDim + col1 / obj->blockSize;
	//	index = row*obj->blockSize+col;
	//}

	//printf( "x=%u, y=%u\n", x, y );
	//printf( "Block=%u, row=%u, col=%u\n", block, row, col );

	float r            = ((float*)obj->blockData[block])[index];

	//printf( "RES=%f\n", r );

	result = Py_BuildValue( "f", r );

	return result;
}

/*
 * Set value at index
 */
PyObject *_SetAtIndex( PyObject *self, PyObject *args )
{
  //printf("SetAtIndex\n");
	//int a represents the number of elements we need in our random vector.
	long a, x, y = 0;
	float f;


	if ( !PyArg_ParseTuple(args, "ifi|i", &a, &f, &x, &y ) )
	{
		return Py_None;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	unsigned int col;
	unsigned int row;
	unsigned int block;
	unsigned int index;

	//Vector
	if(obj->nd==1)
	{
		index = x % ( obj->blockSize * obj->blockSize );
		block = x / ( obj->blockSize * obj->blockSize );
	}
	//Matrix
	else
	{
		col   = x % obj->blockSize;
		row   = y % obj->blockSize;
		printf("2\n");
		block = y / obj->blockSize * obj->numberOfBlocksXDim + x / obj->blockSize;
		printf("3\n");
		index = row*obj->blockSize+col;
	}

	//printf("col: %d\n",col);
	//printf("row: %d\n",row);
	//printf("block: %d\n",block);
	//printf("index: %d\n",index);

	// Vector
	//if( obj->dimensions[0] == 1 || obj->dimensions[1] == 1 )
	//{
	//	index = x % ( obj->blockSize * obj->blockSize );
	//	block = x / ( obj->blockSize * obj->blockSize );
	//}
	// Matrix
	//else
	//{
	//	col   = x % obj->blockSize;
	//	row   = y % obj->blockSize;
	//	block = y / obj->blockSize * obj->numberOfBlocksXDim + x / obj->blockSize;
	//	index = row*obj->blockSize+col;
	//}

	((float*)obj->blockData[block])[index] = f;


	return Py_BuildValue("");
}

/*
 * Set value at index, matrix style row, col
 */
PyObject *_SetAtIndexm( PyObject *self, PyObject *args )
{
	//int a represents the number of elements we need in our random vector.
	long a, col1, row1 = 0;
	float f;


	if ( !PyArg_ParseTuple(args, "ifi|i", &a, &f, &row1, &col1 ) )
	{
		return Py_None;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	unsigned int col;
	unsigned int row;
	unsigned int block;
	unsigned int index;

	//Vector
	if(obj->nd==1)
	{
		index = col1 % ( obj->blockSize * obj->blockSize );
		block = col1 / ( obj->blockSize * obj->blockSize );
	}
	//Matrix
	else
	{
		col   = col1 % obj->blockSize;
		row   = row1 % obj->blockSize;
		block = row1 / obj->blockSize * obj->numberOfBlocksXDim + col1 / obj->blockSize;
		index = row*obj->blockSize+col;
	}

	// Vector
	//if( obj->dimensions[0] == 1 || obj->dimensions[1] == 1 )
	//{
	//	index = col1 % ( obj->blockSize * obj->blockSize );
	//	block = col1 / ( obj->blockSize * obj->blockSize );
	//}
	// Matrix
	//else
	//{
	//	col   = col1 % obj->blockSize;
	//	row   = row1 % obj->blockSize;
	//	block = row1 / obj->blockSize * obj->numberOfBlocksXDim + col1 / obj->blockSize;
	//	index = row*obj->blockSize+col;
	//}

	((float*)obj->blockData[block])[index] = f;


	return Py_None;
}

/*
 * Delete array
 */
PyObject *_Delete( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;

	long a;


	if ( !PyArg_ParseTuple(args, "i", &a ) )
	{
		return Py_None;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	DeleteObject( obj );

	return Py_None;
}

/*
 * Get Length of array
 */
PyObject *_GetLength( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;


	if ( !PyArg_ParseTuple(args, "i", &a ) )
	{
		return Py_None;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	// Vector
	unsigned int length = 1, i;
	for( i = 0 ; i < obj->nd ; i++ )
	{
		length *= obj->dimensions[i];
	}

	result = Py_BuildValue( "i", length );

	return result;
}

/*
 * Solve
 */

PyObject *_solve( PyObject *self, PyObject *args )
{
	PyObject *result = NULL;

	//int a represents the number of elements we need in our random vector.
	long a;
	long b;

	//PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
	//internal_random(a,result);

	if (PyArg_ParseTuple(args, "ii", &a, &b))
	{
		result = Py_BuildValue("i", a);
	} /* otherwise there is an error,
	 * the exception already raised by PyArg_ParseTuple, and NULL is
	 * returned.
	 */

	//printf( "A: bs=%u, dim[0]=%u\n", A->blockSize, A->dimensions[0] );

	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",a,a);
	//printf("Inside add: adr. of PyArrayObject is: %x,%d\n",b,b);
	//internal_add(a,b);
	unsigned int info;
	sgesv( (PyArrayObject*)a, (PyArrayObject*)b, &info );

	//printf("Before returning value\n");

	return result;

}

/*******************
 ** STUFF
 *******************/


PyObject *_readData(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //We read a, but so far we don't use it.
  long a;

  if (PyArg_ParseTuple(args, "i", &a)) {
    result = Py_BuildValue("i", ((PyArrayObject*)a)->blockSize);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  printf("Printint all elements from first block as floats\n");
  printf("((PyArrayObject*)a)->blockData[0][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[0]);
  printf("((PyArrayObject*)a)->blockData[0][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[1]);
  printf("((PyArrayObject*)a)->blockData[0][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[2]);
  printf("((PyArrayObject*)a)->blockData[0][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[3]);
  printf("((PyArrayObject*)a)->blockData[0][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[4]);
  printf("((PyArrayObject*)a)->blockData[0][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[5]);
  printf("((PyArrayObject*)a)->blockData[0][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[6]);
  printf("((PyArrayObject*)a)->blockData[0][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[7]);
  printf("((PyArrayObject*)a)->blockData[0][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[8]);
  printf("((PyArrayObject*)a)->blockData[0][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[9]);
  printf("((PyArrayObject*)a)->blockData[0][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[10]);
  printf("((PyArrayObject*)a)->blockData[0][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[11]);
  printf("((PyArrayObject*)a)->blockData[0][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[12]);
  printf("((PyArrayObject*)a)->blockData[0][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[13]);
  printf("((PyArrayObject*)a)->blockData[0][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[14]);
  printf("((PyArrayObject*)a)->blockData[0][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[15]);
  printf("((PyArrayObject*)a)->blockData[0][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[0])[16]);


  return result;

}

/*
 * For unit test purposes
 */

PyObject *_Sswap(PyObject *self, PyObject *args)
{
	//int a represents the number of elements we need in our random vector.
	long a;
	unsigned int row1, row2;

	if ( !PyArg_ParseTuple(args, "iii", &a, &row1, &row2 ) )
	{
		return NULL;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	FHB_sswap_row_blocked( obj, 0, row1, row2 );

	return Py_None;
}

PyObject *_IsamaxCol(PyObject *self, PyObject *args)
{
	//int a represents the number of elements we need in our random vector.
	long a;
	unsigned int x, y;

	PyObject *result = NULL;

	if ( !PyArg_ParseTuple(args, "iii", &a, &x, &y ) )
	{
		return NULL;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	unsigned int k = FHB_isamax_col2( x, y, a );

	result = Py_BuildValue( "i", k );

	return result;
}

PyObject *_ScaleCol(PyObject *self, PyObject *args)
{
	//int a represents the number of elements we need in our random vector.
	long a;
	unsigned int col, row;
	float scale;

	if ( !PyArg_ParseTuple(args, "ifii", &a, &scale, &col, &row ) )
	{
		return NULL;
	}

	PyArrayObject *obj = (PyArrayObject*)a;

	FHB_sscal_col_blocked( col, row, scale, a );

	return Py_None;
}

PyObject *_SGEMM_fixed(PyObject *self, PyObject *args)
{
	long a, b, c;

	if ( !PyArg_ParseTuple(args, "iii", &a, &b, &c ) )
	{
		return NULL;
	}

	PyArrayObject *obj1 = (PyArrayObject*)a;
	PyArrayObject *obj2 = (PyArrayObject*)b;
	PyArrayObject *obj3 = (PyArrayObject*)c;

	run30( obj1, obj2, obj3, sgemm_fixed_size, SHADER_sgemm_fixed );

	return Py_None;
}

PyObject *_STRSM_LU(PyObject *self, PyObject *args)
{
	long a, b, cb;
	float alpha;

	if ( !PyArg_ParseTuple(args, "iifi", &a, &b, &alpha, &cb ) )
	{
		return NULL;
	}

	PyArrayObject *obj1 = (PyArrayObject*)a;
	PyArrayObject *obj2 = (PyArrayObject*)b;

	PyArrayObject *ALPHA = _malloc_align( sizeof( PyArrayObject ), 7 );
	PyArrayObject *CB    = _malloc_align( sizeof( PyArrayObject ), 7 );

	sMakeFHB( 1, 1, _blockSize, alpha, ALPHA );
	uMakeFHB( 1, 1, _blockSize, cb, CB );

	printf( "ARGS: alpha=%f, cb=%u\n", alpha, cb );
	PrintNDArray( ALPHA, 1 );
	PrintNDArray( CB, 1 );

	run22( obj1, obj2, ALPHA, CB, strsm_llnu_lu_size, SHADER_strsm_llnu_lu );

	DeleteObject( ALPHA );
	DeleteObject( CB );
	printf( "Done diddly done\n" );

	return Py_None;
}


PyObject *_STRSM_LUNN(PyObject *self, PyObject *args)
{
	long a, b;

	if ( !PyArg_ParseTuple(args, "ii", &a, &b ) )
	{
		return NULL;
	}

	PyArrayObject *obj1 = (PyArrayObject*)a;
	PyArrayObject *obj2 = (PyArrayObject*)b;

	PyArrayObject *ALPHA = _malloc_align( sizeof( PyArrayObject ), 7 );
	sMakeFHB( 1, 1, _blockSize, 1, ALPHA );

	run21( obj1, obj2, ALPHA, strsm_lunn_size, SHADER_strsm_lunn );

	printf( "STRSM LUNN! Done diddly done\n" );

	return Py_None;
}

PyObject *_STRSM_LLNU(PyObject *self, PyObject *args)
{
	long a, b;
	float alpha;

	if ( !PyArg_ParseTuple(args, "iif", &a, &b, &alpha ) )
	{
		return NULL;
	}

	PyArrayObject *obj1 = (PyArrayObject*)a;
	PyArrayObject *obj2 = (PyArrayObject*)b;

	PyArrayObject *ALPHA = _malloc_align( sizeof( PyArrayObject ), 7 );
	sMakeFHB( 1, 1, _blockSize, alpha, ALPHA );

	run21( obj1, obj2, ALPHA, strsm_llnu_size, SHADER_strsm_llnu );

	printf( "STRSM LLNU! Done diddly done\n" );

	return Py_None;
}

PyObject *_Sger(PyObject *self, PyObject *args)
{
	long a, b,c;
	float alpha;
	unsigned int j;

	if ( !PyArg_ParseTuple(args, "iiif", &a, &b, &c, &alpha ) )
	{
		return NULL;
	}

	PyArrayObject *obj1 = (PyArrayObject*)a;
	PyArrayObject *obj2 = (PyArrayObject*)b;
	PyArrayObject *obj3 = (PyArrayObject*)c;

	PyArrayObject *ALPHA = _malloc_align( sizeof( PyArrayObject ), 7 );
	sMakeFHB( 1, 1, _blockSize, alpha, ALPHA );

	PyArrayObject *J = _malloc_align( sizeof( PyArrayObject ), 7 );
	uMakeFHB( 1, 1, _blockSize, 1, J );

	run32( obj1, obj2, obj3, ALPHA, J, sger_size, SHADER_sger );

	printf( "STRSM LLNU! Done diddly done\n" );

	return Py_None;
}



PyMethodDef methods[] =
{
	//{ "add", MyCommand, METH_VARARGS },

	/*
	 * NumPy
	 */

	// Create
	{ "shape"    , _Shape, METH_VARARGS },
	{ "arange"   , _arange, METH_VARARGS },
	{ "array"    , _array, METH_VARARGS },

	// Functionality
	{ "add"      , _add, METH_VARARGS },
	{ "multiply" , _Multiply, METH_VARARGS },
	{ "solve"    , _solve, METH_VARARGS },
	{ "subtract" , _sub, METH_VARARGS },
	{ "_div"     , _div, METH_VARARGS },
	{ "divide"   , _Divide, METH_VARARGS },
	{ "less_equal", _lessequal, METH_VARARGS },
	{ "sum"      , _sum, METH_VARARGS },
	{ "abs"      , _abs, METH_VARARGS },
	{ "max"      , _max, METH_VARARGS },

	{ "addSOR"      , _addSOR, METH_VARARGS },
	{ "multiplySOR" , _MultiplySOR, METH_VARARGS },
	{ "subtractSOR" , _subSOR, METH_VARARGS },

	// Getters
	{ "len"      , _GetLength, METH_VARARGS },

	// Fillers
	{ "random"   , _random, METH_VARARGS },
	{ "zeros"    , _Zeros, METH_VARARGS },

	/*
	 * NumPyCBE specifics
	 */

	{"setcbe"        , setcbe, METH_VARARGS },
	{"getcbe"        , getcbe, METH_VARARGS },

	{"_readBlockSize", _readBlockSize, METH_VARARGS },

	{"_readData"     , _readData, METH_VARARGS },

	{"_initVector"   , _initializeVector, METH_NOARGS },

	{"_sliceRow", _sliceRow,METH_VARARGS},//JMI: This function should maybe be replaced with another one in this file. Used in SOR.

	{"_createVector", _createVector,METH_VARARGS},//JMI: This function should maybe be replaced with another one in this file. Used in SOR.

	{"_sliceMult",_sliceMult,METH_VARARGS},//JMI: This function should maybe be merged with the regular mult function. Used in SOR.

	{"_createSOR"    , _createSOR, METH_VARARGS },////JMI: This create is temporarily used while developing SOR. It should be exchanged with something else. Used in SOR.

	{"_equalSOR"    , _equalSOR, METH_VARARGS },////JMI: This create is temporarily used while developing SOR. It should be exchanged with something else. Used in SOR.

	{"create"        , _create, METH_VARARGS },

	{ "SetBlockSize" , _setblocksize, METH_VARARGS },
	{ "Index"        , _GetIndexAt, METH_VARARGS },
	{ "index"        , _GetIndexAtm, METH_VARARGS },
	{ "SetIndex"     , _SetAtIndex, METH_VARARGS },
	{ "setIndex"     , _SetAtIndexm, METH_VARARGS },

	{ "Print"        , _Print, METH_VARARGS },
	{ "PrintS"        , _PrintSci, METH_VARARGS },

	{ "Delete"        , _Delete, METH_VARARGS },

	/*
	 * Unit test thingies
	 */

	{ "SwapRows", _Sswap, METH_VARARGS },
	{ "MaxCol", _IsamaxCol, METH_VARARGS },
	{ "ScaleCol", _ScaleCol, METH_VARARGS },
	{ "Sgemm", _SGEMM_fixed, METH_VARARGS },
	{ "Strsm_lu", _STRSM_LU, METH_VARARGS },
	{ "Strsm_lunn", _STRSM_LUNN, METH_VARARGS },
	{ "Strsm_llnu", _STRSM_LLNU, METH_VARARGS },
	{ "Sger", _Sger, METH_VARARGS },

	// Just testing _testDispatcher1
	{ "testDispatcher1", _testDispatcher1, METH_VARARGS },
	{ "testDispatcher2", _testDispatcher2, METH_VARARGS },


	{ NULL, NULL },
};


int numpycbe_compare( PyObject *self, PyObject *other )
{
	return 1;
}


initDisp( unsigned int numspes )
{
	// Get the number of available SPEs
	speThreads = spe_cpu_info_get(SPE_COUNT_USABLE_SPES, -1);
	// Clamp to the defined number of SPEs used
	if ( speThreads > MAX_SPU_NUM )
	{
		speThreads = MAX_SPU_NUM;
	}
	if( speThreads > numspes )
	{
		speThreads = numspes;
	}
	printf("InitDist. speThreads is: %d\n",speThreads);
	unsigned int i;

	unsigned int temp;

	// Get dispatcher
	//spe_program_handle_t *dispatcher = spe_image_open( "/home/wolfgang/dispatcher" );
	//spe_program_handle_t *dispatcher = spe_image_open( "../../../spudispatcher" );

	char *HOME = getenv("HOME");
	char *DISP = "/numpycbe/spudispatcher";
	char *FILE = strcat( HOME, DISP );

	spe_program_handle_t *dispatcher = spe_image_open( FILE );

	time_t seconds;
	seconds = time( NULL );

  // Initialize threads
	for( i = 0 ; i < speThreads ; i++ )
	{
		//printf( "Initializing SPE #%u\n", i );
		CreateSPEThread( &speData[i], dispatcher, &spe_pointer_addr[i] );

		seconds++;

		// Sending the SPE its id
		spe_in_mbox_write ( speData[i].spe_ctx, &i, 1, SPE_MBOX_ALL_BLOCKING );
		// Sending the SPE its seed. This should be something like time instead of id? Now it is(W)!
		spe_in_mbox_write ( speData[i].spe_ctx, &seconds, 1, SPE_MBOX_ALL_BLOCKING );
   }

  return 0;
}


void setUpDispatcher()
{
  //JMI:Oh well, let's make another function call, why don't we...
  //printf("setUpDispatcher called\n");
  initDisp(speThreads);
}


void initnumpycbe()
{
  (void)Py_InitModule("numpycbe", methods);
  //printf("Inside init\n");
  setUpDispatcher();

  SHADER_index[0] = 999;
  SHADER_index[1] = 999;
  SHADER_index[2] = 999;
  SHADER_index[3] = 999;
  unsigned int i;
  for(i=0;i<100;i++)
  {
	  SHADER_status[i] = 999;
  }

  sliceObject = (SliceObject*)_malloc_align(sizeof(SliceObject),7);
}
