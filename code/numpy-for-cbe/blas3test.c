#include <Python.h>
#include <stdio.h>
#include <malloc.h>

//#include <libspe2.h>
//#include <pthread.h>

#include "common.h"
#include "common_ppu.h"
#include "libppu.c"

typedef unsigned int uint32_t;

#include "/home/jens/randomfiller.h"
#include "/home/jens/arraymultiply.h"
#include "/home/jens/arrayadd.h"
#include "/home/jens/arraydivide.h"
#include "/home/jens/arraysum.h"
#include "/home/jens/arraylessequal_arrayscalar.h"
#include "/home/jens/spu_mult_shader.h"

unsigned int speThreads = 6;
//#define MAX_SPE_THREADS 6
#define MAX_SPU_NUM 6


//unsigned int spe_pointer_addr[MAX_SPE_THREADS][4] __attribute__((aligned(16)));
//PpuPthreadData_t speData[MAX_SPE_THREADS];

unsigned int _numberOfBlocksX = 1;
unsigned int _numberOfBlocksY = 1;
unsigned int _blockSize = 64;

float resultFloats[MAX_SPU_NUM][4] __attribute__((aligned(16)));

unsigned int spe_pointer_addr[MAX_SPU_NUM][4] __attribute__((aligned(16)));
PpuPthreadData_t speData[MAX_SPU_NUM];


//These arrays are used for rearranging metadata (blockPointer) for blas3Mult
PyArrayObject *multVector1, *multVector2, *multVector3;

typedef struct
{
  int a;
  int b;
}PyArrayObjectTest;

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
  resultObj->numberOfBlocks = _numberOfBlocksX*_numberOfBlocksY;
  resultObj->numberOfBlocksXDim = _numberOfBlocksX;
  resultObj->numberOfBlocksYDim = _numberOfBlocksY;
  resultObj->blockSize = _blockSize;//in elements
  resultObj->nd = 1;
  resultObj->blockData = (char**)_malloc_align(sizeof(char*)*_numberOfBlocksX*_numberOfBlocksY,7);
  unsigned int i = 0;
  //printf("start blockData[0] : %d,%x\n",resultObj->blockData[0],resultObj->blockData[0]);
  for(i = 0;i<_numberOfBlocksX*_numberOfBlocksY;i++)
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
  unsigned int k;
  for(k=0;k<_numberOfBlocksX*_numberOfBlocksY;k++)
    {
      for(i=0;i<_blockSize*_blockSize;i++)
	{
	  ((float*)resultObj->blockData[k])[i] = 1;
	}
    }
  
  
  return result;
}



void internal_random(PyArrayObject *a)
{
  //printf("adr of PyArrayObject: %x,%d\n",result,result);

  //Setting the PyArrayObject
  //result->numberOfBlocks = _numberOfBlocks*_numberOfBlocks;
  //result->numberOfBlocksXDim = _numberOfBlocks;
  //result->numberOfBlocksYDim = _numberOfBlocks;
  //result->blockSize = _blockSize;//in elements
  //result->nd = 1;
  //result->blockData = (char**)_malloc_align(sizeof(char*)*_numberOfBlocks*_numberOfBlocks,7);
  //unsigned int i = 0;
  //for(i = 0;i<_numberOfBlocks*_numberOfBlocks;i++)
  //  {
  //    result->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
  //  }

  //printf("Adr of blockData: %x\n",result->blockData);
  //printf("Adr of blockData: %d\n",result->blockData);

  //Setting the Operation object
  Operation_t op1;
  op1.shaderSize = randomfiller_size;
  op1.EA_shader  = randomfiller;
  op1.obj[0]     = a;

  //printf("randomfiller: %d\n",randomfiller);
  //printf("&randomfiller: %d\n",&randomfiller);

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
      spe_in_mbox_write ( speData[y].spe_ctx, &state, 1, SPE_MBOX_ALL_BLOCKING );
      //printf("TO THE DISP. THE SUM STATE: %d, has just been sent\n",&state);
      //printf("This is the actual value: %d\n",state);
    }

  //printf("SUM Waiting for the SPE's\n");//ALSO, check that the structure used is correct!
  // Waiting for SPEs!
  //printf( "Waiting for SPEs\n" );
  unsigned int checked = 0;
  unsigned int r;
  //printf("checked is: %d\n",checked);
  //printf("speThreads is: %d\n",speThreads);
  while( checked < speThreads )
    {
      //printf("checked is: %d\n",checked);
      if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
        {
	  spe_out_mbox_read( speData[checked].spe_ctx, &r, 1 );
          checked++;
          //printf("Something read on the inbox\n");
        }
    }
  //printf("Done waiting on threads to finish\n");

}

PyObject *_random(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //int a represents the number of elements we need in our random vector.
  long a;

  //PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  //internal_random(a,result);

  if (PyArg_ParseTuple(args, "i", &a)) {
    result = Py_BuildValue("i", a);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //Move result PyObject if it is not aligned
  //  if(!((((unsigned int)result)%16)==0))
    //    {
      //      printf("unaligned result is: %x\n",result);
      //      PyObject *alignedResult = _malloc_align(32,7);//This should be enough
      //      unsigned int u = 0;
      //for(u=0;u<32;u++)
      //	{
      //	  ((char*)alignedResult)[u] = ((char*)result);
      //	}
      //      result = alignedResult;
      //    }
  
  //printf("Before calling internal_random: adr. is\n");
  //printf("blockData[0]: %x\n",((PyArrayObject*)a)->blockData[0]);
  //printf("blockData[1]: %x\n",((PyArrayObject*)a)->blockData[1]);
  //printf("a: %d\n",a);

  internal_random(a);

  //printf("After calling internal_random: adr. is\n");
  //printf("blockData[0]: %x\n",((PyArrayObject*)a)->blockData[0]);
  //printf("blockData[1]: %x\n",((PyArrayObject*)a)->blockData[1]);
  //printf("a: %d\n",a);

  //printf("Before returning value\n");

  return result;

}

PyObject *getcbe(PyObject *self, PyObject *args)
{
  //PyArrayObject *resultObj = _malloc_align2(sizeof(PyArrayObject),7);
  PyObject *result = NULL;

  //We read a, but so far we don't use it.
  long a;

  //resultObj->a = 7;
  //resultObj->b = 8;



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

void internal_mult(long a, long b)
{
  //printf("Inside internal mult\n");
  //Setting the Operation object
  Operation_t op1;
  op1.shaderSize = arraymultiply_size;
  op1.EA_shader  = arraymultiply;
  op1.obj[0]     = a;
  op1.obj[1]     = b;
  op1.obj[2]     = a;

  //printf("arraymultiply: %d\n",arraymultiply);
  //printf("&arraymultiply: %d\n",&arraymultiply);

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

  //printf("MULT Waiting for the SPE's\n");//ALSO, check that the structure used is correct!
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
  //printf("Done waiting on threads to finish in MULT\n");

}

PyObject *_mult(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //int a represents the number of elements we need in our random vector.
  long a;
  long b;

  //PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  //internal_random(a,result);

  if (PyArg_ParseTuple(args, "ii", &a,&b)) {
    result = Py_BuildValue("i", a);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //printf("Inside mult: adr. of PyArrayObject is: %x,%d\n",a,a);
  internal_mult(a,b);

  //printf("Before returning value\n");

  return result;

}

void internal_add(long a, long b)
{
  //printf("Inside internal add\n");
  //Setting the Operation object
  Operation_t op1;
  op1.shaderSize = arrayadd_size;
  op1.EA_shader  = arrayadd;
  op1.obj[0]     = a;
  op1.obj[1]     = b;
  op1.obj[2]     = a;

  //printf("arraymultiply: %d\n",arrayadd);
  //printf("&arraymultiply: %d\n",&arrayadd);

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
  //printf("Done waiting on threads to finish in ADD\n");

}


PyObject *_add(PyObject *self, PyObject *args)
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

  //printf("Inside add: adr. of PyArrayObject is: %x,%d\n",a,a);
  //printf("Inside add: adr. of PyArrayObject is: %x,%d\n",b,b);
  internal_add(a,b);

  //printf("Before returning value\n");

  return result;

}

void internal_div(long a, long b)
{
  //printf("Inside internal add\n");
  //Setting the Operation object
  Operation_t op1;
  op1.shaderSize = arraydivide_size;
  op1.EA_shader  = arraydivide;
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

void internal_lessequal(long a, float b)
{
  //printf("Inside internal add\n");

  //b must be wrapped in a PyArrayObject
  PyArrayObject *scalar = (PyArrayObject*)_malloc_align(sizeof(PyArrayObject),7);
  scalar->blockData = (char**)_malloc_align(sizeof(char*),7);
  scalar->blockData[0] = (char*)_malloc_align(sizeof(float),7);
  ((float*)(scalar->blockData[0]))[0] = b;
  scalar->numberOfBlocks = 1;

  //Setting the Operation object
  Operation_t op1;
  op1.shaderSize = arraylessequal_arrayscalar_size;
  op1.EA_shader  = arraylessequal_arrayscalar;
  op1.obj[0]     = a;
  op1.obj[1]     = scalar;
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
  //printf("Done waiting on threads to finish in LESSEQUAL\n");

}


PyObject *_lessequal(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //int a represents the number of elements we need in our random vector.
  long a;
  float b;

  //PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  //internal_random(a,result);

  if (PyArg_ParseTuple(args, "if", &a, &b)) {
    result = Py_BuildValue("i", a);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //printf("a Inside div: adr. of PyArrayObject is: %x,%d\n",a,a);
  //printf("b Inside div: adr. of PyArrayObject is: %x,%f\n",b,b);
  //printf("Before calling internal_lessequal: adr. is\n");
  //printf("blockData[0]: %x\n",((PyArrayObject*)a)->blockData[0]);
  //printf("blockData[1]: %x\n",((PyArrayObject*)a)->blockData[1]);

  internal_lessequal(a,b);

  //printf("Before returning value\n");
  //printf("Before returning in _lessequal\n");
  //printf("result is: %d\n",a);

  //printf("After calling internal_lessequal: adr. is\n");
  //printf("blockData[0]: %x\n",((PyArrayObject*)a)->blockData[0]);
  //printf("blockData[1]: %x\n",((PyArrayObject*)a)->blockData[1]);


  return result;

}

unsigned int internal_sum(long a)
{
  //printf("Inside internal add\n");
  //Setting the Operation object
  Operation_t op1;
  op1.shaderSize = arraysum_size;
  op1.EA_shader  = arraysum;
  op1.obj[0]     = a;

  //printf("arraymultiply: %d\n",arraydivide);
  //printf("&arraymultiply: %d\n",&arraydivide);

  op1.num_SPES   = speThreads;
  unsigned int i = 0;

  for(i = 0;i<speThreads;i++)
    {
      spe_pointer_addr[i][0] = &op1;
    }

  //printf("Adr of op1 sat. It is: %x\n",&op1);

  unsigned int state = 2;
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
  unsigned int signal;
  //float r;
  //printf("1. r[0]: %f\n",resultFloats[0][0]);
  //printf("1. adr is: %x\n",&(resultFloats[0][0]));
  unsigned int pointersToResultFloats[speThreads];
  unsigned int e;
  for(e=0;e<speThreads;e++)
    {
      pointersToResultFloats[e] = &(resultFloats[e][0]);
    }

  for ( y = 0 ; y < speThreads ; y++ )
    {
      //printf("Sending res pointers to disp\n");
      spe_in_mbox_write ( speData[y].spe_ctx, &pointersToResultFloats[y], 1, SPE_MBOX_ALL_BLOCKING );
      //printf("TO THE DISP. THE SUM STATE: %d, has just been sent\n",&state);
      //printf("This is the actual value: %d\n",state);
    }

  float finalRes = 0;
  while( checked < speThreads )
    {
      if ( spe_out_mbox_status( speData[checked].spe_ctx ) )
        {
          spe_out_mbox_read( speData[checked].spe_ctx, &signal, 1 );
          checked++;
          //printf("Something read on the inbox\n");
        }
    }
  //printf("Done waiting on threads to finish in DIV\n");
  //printf("2. r[0]: %f\n",resultFloats[y][0]);

  //Add all the results together
  unsigned int u = 0;
  for(u=0;u<speThreads;u++)
    {
      finalRes = finalRes+resultFloats[u][0];
    }
  
  return finalRes;
}

PyObject *_sum(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //int a represents the number of elements we need in our random vector.
  long a;

  //PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  //internal_random(a,result);

  if (PyArg_ParseTuple(args, "i", &a)) {
    /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

    //printf("Inside div: adr. of PyArrayObject is: %x,%d\n",a,a);
  unsigned int sumResult = internal_sum(a);

  result = Py_BuildValue("i", sumResult);

  //printf("Before returning value\n");
  }
  return result;

}

void internal_blas3Mult(long a, long b, long c)
{
  //printf("Inside internal add\n");
  //Setting the Operation object
  Operation_t op1;
  op1.shaderSize = MAIN_SIZE;
  op1.EA_shader  = main2;
  op1.obj[0]     = a;
  op1.obj[1]     = b;
  op1.obj[2]     = c;

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

  //return result;
}



PyObject *_blas3Mult(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //int a represents the number of elements we need in our random vector.
  long a;
  long b;
  long c;

  //PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  //internal_random(a,result);

  if (PyArg_ParseTuple(args, "iii", &a, &b,&c)) {
    result = Py_BuildValue("i", c);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //printf("Inside div: adr. of PyArrayObject is: %x,%d\n",a,a);
  internal_blas3Mult(a,b,c);

  //printf("Before returning value\n");

  return result;

}

internal_createTempArrays(PyArrayObject *arg1, long d)
{
  //Set multVector1/argument 1 til blas3Mult
  multVector1->numberOfBlocks = arg1->numberOfBlocksXDim-d;
  multVector1->numberOfBlocksXDim = arg1->numberOfBlocksXDim-d;
  multVector1->numberOfBlocksYDim = 1;
  multVector1->blockSize = arg1->blockSize;
  multVector1->nd = arg1->nd;

  unsigned int i;
  unsigned int startIndex;
  for(i=0;i<(arg1->numberOfBlocksXDim-d);i++)
    {
      multVector1->blockData[i] = arg1->blockData[(arg1->numberOfBlocksXDim*(d-1)+d)+i];
    }
  
  //Set multVector2/argument 2 til blas3Mult
  multVector2->numberOfBlocks = arg1->numberOfBlocksYDim-d;
  multVector2->numberOfBlocksXDim = 1;
  multVector2->numberOfBlocksYDim = arg1->numberOfBlocksYDim-d;
  multVector2->blockSize = arg1->blockSize;
  multVector2->nd = arg1->nd;

  for(i=0;i<(arg1->numberOfBlocksYDim-d);i++)
    {
      multVector2->blockData[i] = arg1->blockData[(arg1->numberOfBlocksXDim*d+d-1)+(i*(arg1->numberOfBlocksYDim-d))];
    }  
  
  //Set multVector3/argument 3 til blas3Mult
  multVector3->numberOfBlocks = (arg1->numberOfBlocksYDim-d) * (arg1->numberOfBlocksXDim-d);
  multVector3->numberOfBlocksXDim = arg1->numberOfBlocksXDim-d;
  multVector3->numberOfBlocksYDim = arg1->numberOfBlocksYDim-d;
  multVector3->blockSize = arg1->blockSize;
  multVector3->nd = arg1->nd;

  unsigned int u;
  
  unsigned int resultIndex = 0;

  startIndex = arg1->numberOfBlocksXDim*d+d;

  for(i=0;i<(arg1->numberOfBlocksYDim-d);i++)
    {
      for(u=0;u<(arg1->numberOfBlocksXDim-d);u++)
	{
	  multVector3->blockData[resultIndex] = arg1->blockData[startIndex+u];
	  resultIndex++;
	}
      startIndex=startIndex+arg1->numberOfBlocksXDim;
    }

}

PyObject *_blas3Mult_lu(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;

  //int a represents the number of elements we need in our random vector.
  long arg1;
  long d;

  //PyArrayObject *resultObj = _malloc_align(sizeof(PyArrayObject),7);
  //internal_random(a,result);

  if (PyArg_ParseTuple(args, "ii", &arg1, &d)) {
    result = Py_BuildValue("i", arg1);
  } /* otherwise there is an error,
     * the exception already raised by PyArg_ParseTuple, and NULL is
     * returned.
     */

  //printf("Inside div: adr. of PyArrayObject is: %x,%d\n",a,a);
  internal_createTempArrays(arg1,d);
  internal_blas3Mult(multVector1,multVector2,multVector3);

  printf("Before returning value in _blas3Mult_lu\n");

  return result;

}


PyObject *_create(PyObject *self, PyObject *args)
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
  resultObj->nd = 1;
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
  //printf("Based on sizes: %d and %d: \n",sizeof(char*)*_numberOfBlocks*_numberOfBlocks,sizeof(char)*_blockSize*_blockSize*\4);

//Fill zeros in the block
unsigned int k;
for(k=0;k<numBlocksX*numBlocksY;k++)
  {
    for(i=0;i<_blockSize*_blockSize;i++)
      {
	((float*)resultObj->blockData[k])[i] = 1;
      }
  }


return result;
}

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

  printf("Printint first 4 elements from second block as floats\n");
  printf("((PyArrayObject*)a)->blockData[1][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[0]);
  printf("((PyArrayObject*)a)->blockData[1][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[1]);
  printf("((PyArrayObject*)a)->blockData[1][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[2]);
  printf("((PyArrayObject*)a)->blockData[1][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[3]);
  printf("((PyArrayObject*)a)->blockData[1][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[0]);
  printf("((PyArrayObject*)a)->blockData[1][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[1]);
  printf("((PyArrayObject*)a)->blockData[1][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[2]);
  printf("((PyArrayObject*)a)->blockData[1][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[3]);
  printf("((PyArrayObject*)a)->blockData[1][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[0]);
  printf("((PyArrayObject*)a)->blockData[1][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[1]);
  printf("((PyArrayObject*)a)->blockData[1][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[2]);
  printf("((PyArrayObject*)a)->blockData[1][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[3]);
  printf("((PyArrayObject*)a)->blockData[1][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[0]);
  printf("((PyArrayObject*)a)->blockData[1][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[1]);
  printf("((PyArrayObject*)a)->blockData[1][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[2]);
  printf("((PyArrayObject*)a)->blockData[1][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[1])[3]);


  printf("Printint div 4 elements from third block as floats\n");
  printf("((PyArrayObject*)a)->blockData[0][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[22])[34]);
  printf("((PyArrayObject*)a)->blockData[0][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[30])[62]);
  printf("((PyArrayObject*)a)->blockData[0][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[4])[63]);
  printf("((PyArrayObject*)a)->blockData[0][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[5])[63]);


  printf("Printint first 4 elements from fourth block as floats\n");
  printf("((PyArrayObject*)a)->blockData[0][0]: %f\n",((float*)((PyArrayObject*)a)->blockData[3])[0]);
  printf("((PyArrayObject*)a)->blockData[0][1]: %f\n",((float*)((PyArrayObject*)a)->blockData[3])[1]);
  printf("((PyArrayObject*)a)->blockData[0][2]: %f\n",((float*)((PyArrayObject*)a)->blockData[3])[2]);
  printf("((PyArrayObject*)a)->blockData[0][3]: %f\n",((float*)((PyArrayObject*)a)->blockData[3])[3]);
  

  //printf("hex ((PyArrayObject*)a)->blockData[0][0]: %x\n",((float*)((PyArrayObject*)a)->blockData[0])[0]);

  
  //printf("((PyArrayObject*)a)->blockData[0][0]: %d\n",((unsigned int*)((PyArrayObject*)a)->blockData[0])[0]);
  //printf("((PyArrayObject*)a)->blockData[0][1]: %d\n",((unsigned int*)((PyArrayObject*)a)->blockData[0])[1]);
  //printf("((PyArrayObject*)a)->blockData[0][2]: %d\n",((unsigned int*)((PyArrayObject*)a)->blockData[0])[2]);
  //printf("((PyArrayObject*)a)->blockData[0][3]: %d\n",((unsigned int*)((PyArrayObject*)a)->blockData[0])[3]);

  //printf("((PyArrayObject*)a)->blockData[0][0]: %x\n",((char*)((PyArrayObject*)a)->blockData[0])[0]);
  //printf("((PyArrayObject*)a)->blockData[0][1]: %x\n",((char*)((PyArrayObject*)a)->blockData[0])[1]);
  //printf("((PyArrayObject*)a)->blockData[0][2]: %x\n",((char*)((PyArrayObject*)a)->blockData[0])[2]);
  //printf("((PyArrayObject*)a)->blockData[0][3]: %x\n",((char*)((PyArrayObject*)a)->blockData[0])[3]);

  return result;

}

PyObject *_initMultVectors(PyObject *self, PyObject *args)
{
  PyObject *result = NULL;
  long numBlocksX;
  long numBlocksY;

  if (PyArg_ParseTuple(args, "ii", &numBlocksX, &numBlocksY)) {
  result = Py_BuildValue("i", 1);
  }

  multVector1 = _malloc_align(sizeof(PyArrayObject),7);
  multVector2 = _malloc_align(sizeof(PyArrayObject),7);
  multVector3 = _malloc_align(sizeof(PyArrayObject),7);

  //Setting the PyArrayObject
  multVector1->numberOfBlocks = numBlocksX*numBlocksY;
  multVector1->numberOfBlocksXDim = numBlocksX;
  multVector1->numberOfBlocksYDim = numBlocksY;
  multVector1->blockSize = _blockSize;//in elements
  multVector1->nd = 1;
  multVector1->blockData = (char**)_malloc_align(sizeof(char*)*numBlocksX*numBlocksY,7);
  unsigned int i = 0;
  for(i = 0;i<numBlocksX*numBlocksY;i++)
    {
      multVector1->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
    }

  multVector2->numberOfBlocks = numBlocksX*numBlocksY;
  multVector2->numberOfBlocksXDim = numBlocksX;
  multVector2->numberOfBlocksYDim = numBlocksY;
  multVector2->blockSize = _blockSize;//in elements
  multVector2->nd = 1;
  multVector2->blockData = (char**)_malloc_align(sizeof(char*)*numBlocksX*numBlocksY,7);
  for(i = 0;i<numBlocksX*numBlocksY;i++)
    {
      multVector2->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
    }

  multVector3->numberOfBlocks = numBlocksX*numBlocksY;
  multVector3->numberOfBlocksXDim = numBlocksX;
  multVector3->numberOfBlocksYDim = numBlocksY;
  multVector3->blockSize = _blockSize;//in elements
  multVector3->nd = 1;
  multVector3->blockData = (char**)_malloc_align(sizeof(char*)*numBlocksX*numBlocksY,7);
  for(i = 0;i<numBlocksX*numBlocksY;i++)
    {
      multVector3->blockData[i] = (char*)_malloc_align(sizeof(char)*_blockSize*_blockSize*4,7);
    }


  return result;

}


PyMethodDef methods[] = {
  {"add", MyCommand, METH_VARARGS},
  {"setcbe", setcbe, METH_VARARGS},
  {"getcbe", getcbe, METH_VARARGS},
  {"_random", _random, METH_VARARGS},
  {"_readBlockSize", _readBlockSize, METH_VARARGS},
  {"_mult",_mult,METH_VARARGS},
  {"_readData",_readData,METH_VARARGS},
  {"_add",_add,METH_VARARGS},
  {"_div",_div,METH_VARARGS},
  {"_lessequal",_lessequal,METH_VARARGS},
  {"_sum",_sum,METH_VARARGS},
  {"_initVector",_initializeVector,METH_NOARGS},
  {"_blas3Mult",_blas3Mult,METH_VARARGS},
  {"_create",_create,METH_VARARGS},
  {"_initMultVectors",_initMultVectors,METH_VARARGS},
  {"_blas3Mult_lu",_blas3Mult_lu,METH_VARARGS},
  {NULL, NULL},
};

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
  //printf("InitDist. speThreads is: %d\n",speThreads);
  unsigned int i;

  unsigned int temp;

  // Get dispatcher
  //printf("Getting the dispatcher\n");
  spe_program_handle_t *dispatcher = spe_image_open( "/home/jens/numpycbe_dispatcher" );
  //printf("After getting the dispatcher\n");

  // Initialize threads
  for( i = 0 ; i < speThreads ; i++ )
    {
      CreateSPEThread( &speData[i], dispatcher, &spe_pointer_addr[i] );

      // Sending the SPE its id
      //printf("spe_write MULTIARRAYMODULE Sending id to SPE %d.\n",i);
      spe_in_mbox_write ( speData[i].spe_ctx, &i, 1, SPE_MBOX_ALL_BLOCKING );
      // Sending the SPE its seed. This should be something like time instead of id?
      //printf("spe_write MULTIARRAYMODULE Sending seed to SPE %d.\n",i);
      spe_in_mbox_write ( speData[i].spe_ctx, &i, 1, SPE_MBOX_ALL_BLOCKING );
    }
  //printf("speData[i].spe_ctx is : %d\n",speData[i].spe_ctx);

  //spe_in_mbox_write ( (void*)temp, &i, 1, SPE_MBOX_ALL_BLOCKING );

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
}
