#include "/opt/cell/sysroot/usr/include/cblas.h"
#include <malloc.h>
#define BUF_SIZE 32
#include <stdio.h>
#include "pputimer.h"

static __inline void * _malloc_align2(unsigned int size, unsigned int log2_align)
{
  void *ret;
  char *real;
  unsigned long offset;
  unsigned long align;

  align = 1 << log2_align;
  real = (char *)malloc(size + 2*sizeof(void *) + (align-1));
  if (real) {
    offset = (align - (unsigned long)(real + 2*sizeof(void *))) & (align-1);
    ret = (void *)((real + 2*sizeof(void *)) + offset);
    *((unsigned int *)(ret)-2) = size;
    *((void **)(ret)-1) = (void *)(real);
  } else {
    ret = (void *)(real);
  }
  return (ret);
}

void testSGEMM()
{
	//Testing IBM Blas sgemm routine
	/*
	void cblas_sgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
                 const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
                 const int K, const float alpha, const float *A,
                 const int lda, const float *B, const int ldb,
                 const float beta, float *C, const int ldc);
    */
    //Allocate data pointers:
    float *pointerA = _malloc_align2(64,7);
    float *pointerB = _malloc_align2(64,7);
    float *pointerC = _malloc_align2(64,7);
    
    //Fill data in matrices:
    unsigned int i;
    for(i = 0;i<4;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    for(i = 4;i<8;i++)
    {
    	pointerA[i] = 2;
    	pointerB[i] = 1;
    }
    for(i = 8;i<12;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    for(i = 12;i<16;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    
    unsigned int j;
    for(j = 0;j<16*4;j++)
    {
    	pointerC[j] = 0;
    }
    
    Tic();
    cblas_sgemm (CblasRowMajor,     // Matrix is in column major order
                CblasNoTrans,     // Don't transpose A
                CblasNoTrans,     // Don't transpose B
                4,        // Number of rows in A (and C)
                4,    // Number of columns in B (and C)
                4,    // Number of columns in A (and rows in B)
                1,        // Scalar factor multiplying A
                pointerA,    // Pointer to A
                4,     // Length of leading dim of A (number of 
						     //rows for col major)
                pointerB,    // Pointer to B
                4,    // Length of leading dim of B (number of 
							 //rows for col major)
                1,        // Scalar factor multiplying C
                pointerC,        // Pointer to C
                4        // Length of leading dim of C (number of rows 
							   //for col major)
                );
    PrintTicToc("First time is:",Toc());
    
    //Fill data in matrices:
    for(i = 0;i<4;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    for(i = 4;i<8;i++)
    {
    	pointerA[i] = 2;
    	pointerB[i] = 1;
    }
    for(i = 8;i<12;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    for(i = 12;i<16;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    
    for(j = 0;j<16*4;j++)
    {
    	pointerC[j] = 0;
    }
    
    Tic();
    cblas_sgemm (CblasRowMajor,     // Matrix is in column major order
                CblasNoTrans,     // Don't transpose A
                CblasNoTrans,     // Don't transpose B
                4,        // Number of rows in A (and C)
                4,    // Number of columns in B (and C)
                4,    // Number of columns in A (and rows in B)
                1,        // Scalar factor multiplying A
                pointerA,    // Pointer to A
                1,     // Length of leading dim of A (number of
						     //rows for col major)
                pointerB,    // Pointer to B
                4,    // Length of leading dim of B (number of 
							 //rows for col major)
                1,        // Scalar factor multiplying C
                pointerC,        // Pointer to C
                4        // Length of leading dim of C (number of rows
							   //for col major)
                );
     PrintTicToc("Second time is:",Toc());
     
    
	
	//Read data from resultMatrix
	printf("Result are:\n");
    for(j = 0;j<16;j++)
    {
    	printf("%f\n",pointerC[j]);
    }
	
/*

int i,j ;

int entries_x, entries_y ;

float sa=0.1;

float *sx, *sy ;

int incx=1, incy=2;

int n = BUF_SIZE;

double result;

entries_x = n * incx ;

entries_y = n * incy ;

sx = (float *) _malloc_align2( entries_x * sizeof( float ), 7 ) ;

sy = (float *) _malloc_align2( entries_y * sizeof( float ), 7 ) ;

for( i = 0 ; i < entries_x ; i++ )

sx[i] = (float) (i) ;

j = entries_y - 1 ;

for( i = 0 ; i < entries_y ; i++,j-- )

sy[i] = (float) (j) ;

cblas_scopy( &n, sx, &incx, sy, &incy ) ;

result = cblas_sdot( &n, sx, &incx, sy, &incy ) ;

*/
}

void testSGEMV()
{
	/*
	void cblas_sgemv(const enum CBLAS_ORDER order,
                 const enum CBLAS_TRANSPOSE TransA, const int M, const int N,
                 const float alpha, const float *A, const int lda,
                 const float *X, const int incX, const float beta,
                 float *Y, const int incY);
    */
    
    //Allocate data pointers:
    float *pointerA = _malloc_align2(64,7);
    float *pointerB = _malloc_align2(64,7);
    float *pointerC = _malloc_align2(64,7);
    
    //Fill data in matrices:
    unsigned int i;
    for(i = 0;i<4;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    for(i = 4;i<8;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    for(i = 8;i<12;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    for(i = 12;i<16;i++)
    {
    	pointerA[i] = 1;
    	pointerB[i] = 1;
    }
    
    unsigned int j;
    for(j = 0;j<16;j++)
    {
    	pointerC[j] = 0;
    }
    
    Tic();
    cblas_sgemv(CblasRowMajor,
    			CblasNoTrans,
    			4,//M
    			4,//N
    			1,//alpha
    			pointerA,//pointerA    			1,
    			4,//lda
    			pointerB,//pointerX
    			1,//incX
    			1,//beta
    			pointerC,//pointerY
    			1//incY
    			);
    
    PrintTicToc("1st Time: ",Toc());
    //printf("Printing result\n");		
    //for(j = 0;j<4;j++)
    //{
    //	printf("%f\n",pointerC[j]);
    //}
    
    Tic();
    cblas_sgemv(CblasRowMajor,
    			CblasNoTrans,
    			4,//M
    			4,//N
    			1,//alpha
    			pointerA,//pointerA
    			4,//lda
    			pointerB,//pointerX
    			1,//incX
    			1,//beta
    			pointerC,//pointerY
    			1//incY
    			);
    PrintTicToc("2nd Time: ",Toc());
}

int main()
{
	testSGEMV();
	//testSGEMM();
	
return 0;
}
