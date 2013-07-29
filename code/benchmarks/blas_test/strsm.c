/*
 * strsm.c
 *
 *  Created on: May 10, 2009
 *      Author: wolle
 */

#include <cblas.h>
#include <clapack.h>

#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <sys/time.h>
#include <math.h>




/*
 * SGETF2
 */

void SGETF2( unsigned int m, unsigned int n, float *A, int LDA, int *ipiv, int *info )
{
	unsigned int bs = 4, jp;

	unsigned int j, min_mn = m;
	if( n < m ) min_mn = n;

	for( j = 0 ; j < min_mn ; j++ )
	{
		printf( "-------------->\n" );
		Print( m, m, A );

		// Find largest index in column j
		jp = j + cblas_isamax( m-j, &A[j*LDA+j], LDA );
		ipiv[j] = jp;
		if( jp != j )
		{
			cblas_sswap( n, &A[j*LDA], 1, &A[jp*LDA], 1 );
		}

		if( j < m - 1 )
		{
			cblas_sscal( m-j-1, (1/A[j*LDA+j]), &A[(j+1)*LDA+j], LDA );
		}

		if( j < min_mn )
		{
			cblas_sger( 101, m-j-1, n-j-1, -1, &A[(j+1)*LDA+j], LDA, &A[j*LDA+j+1], 1, &A[(j+1)*LDA+j+1], LDA );
		}

		printf( "J=%u:\n", j );
		Print( m, m, A );
	}
}

//void SLASWP( unsigned int n, float *A, unsigned int LDA, unsigned int k1, unsigned int k2, int *ipiv )
//{
//	unsigned int N32 = ( N / 32 ) * 32;
//	unsigned int i,j,k;
//	unsigned int ip, IX;
//	unsigned int IX0 = k1;
//	unsigned int I1 = k1;
//	unsigned int I2 = k2;
//
//	if( N32 != 0 )
//	{
//		for( j = 0 ; j < N32 ; j+=32 )
//		{
//			IX = IX0;
//			for( i = I1 ; i < I2 ; i++ )
//			{
//				ip = (unsigned int)ipiv[IX];
//				if ( ip != i )
//				{
//					for( k = j ;  )
//					{
//
//					}
//				}
//			}
//		}
//	}
//}

typedef int integer;
typedef float real;

/* Subroutine */ int slaswp_(integer *n, real *a, integer *lda, integer *k1,integer *k2, integer *ipiv, integer *incx)
{
/*  -- LAPACK auxiliary routine (version 3.1) --
       Univ. of Tennessee, Univ. of California Berkeley and NAG Ltd..
       November 2006


    Purpose
    =======

    SLASWP performs a series of row interchanges on the matrix A.
    One row interchange is initiated for each of rows K1 through K2 of A.

    Arguments
    =========

    N       (input) INTEGER
            The number of columns of the matrix A.

    A       (input/output) REAL array, dimension (LDA,N)
            On entry, the matrix of column dimension N to which the row
            interchanges will be applied.
            On exit, the permuted matrix.

    LDA     (input) INTEGER
            The leading dimension of the array A.

    K1      (input) INTEGER
            The first element of IPIV for which a row interchange will
            be done.

    K2      (input) INTEGER
            The last element of IPIV for which a row interchange will
            be done.

    IPIV    (input) INTEGER array, dimension (K2*abs(INCX))
            The vector of pivot indices.  Only the elements in positions
            K1 through K2 of IPIV are accessed.
            IPIV(K) = L implies rows K and L are to be interchanged.

    INCX    (input) INTEGER
            The increment between successive values of IPIV.  If IPIV
            is negative, the pivots are applied in reverse order.

    Further Details
    ===============

    Modified by
     R. C. Whaley, Computer Science Dept., Univ. of Tenn., Knoxville, USA

   =====================================================================


       Interchange row I with row IPIV(I) for each of rows K1 through K2.

       Parameter adjustments */
    /* System generated locals */
    integer a_dim1, a_offset, i__1, i__2, i__3, i__4;
    /* Local variables */
    static integer i__, j, k, i1, i2, n32, ip, ix, ix0, inc;
    static real temp;

    a_dim1 = *lda;
    a_offset = 1 + a_dim1;
    a -= a_offset;
    --ipiv;

    /* Function Body */
    if (*incx > 0) {
	ix0 = *k1;
	i1 = *k1;
	i2 = *k2;
	inc = 1;
    } else if (*incx < 0) {
	ix0 = (1 - *k2) * *incx + 1;
	i1 = *k2;
	i2 = *k1;
	inc = -1;
    } else {
	return 0;
    }

    n32 = *n / 32 << 5;
    if (n32 != 0) {
	i__1 = n32;
	for (j = 1; j <= i__1; j += 32) {
	    ix = ix0;
	    i__2 = i2;
	    i__3 = inc;
	    for (i__ = i1; i__3 < 0 ? i__ >= i__2 : i__ <= i__2; i__ += i__3)
		    {
		ip = ipiv[ix];
		if (ip != i__) {
		    i__4 = j + 31;
		    for (k = j; k <= i__4; ++k) {
			temp = a[i__ + k * a_dim1];
			a[i__ + k * a_dim1] = a[ip + k * a_dim1];
			a[ip + k * a_dim1] = temp;
/* L10: */
		    }
		}
		ix += *incx;
/* L20: */
	    }
/* L30: */
	}
    }
    if (n32 != *n) {
	++n32;
	ix = ix0;
	i__1 = i2;
	i__3 = inc;
	for (i__ = i1; i__3 < 0 ? i__ >= i__1 : i__ <= i__1; i__ += i__3) {
	    ip = ipiv[ix];
	    if (ip != i__) {
		i__2 = *n;
		for (k = n32; k <= i__2; ++k) {
		    temp = a[i__ + k * a_dim1];
		    a[i__ + k * a_dim1] = a[ip + k * a_dim1];
		    a[ip + k * a_dim1] = temp;
/* L40: */
		}
	    }
	    ix += *incx;
/* L50: */
	}
    }

    return 0;

/*     End of SLASWP */

} /* slaswp_ */


void SGETRF( unsigned int m, unsigned int n, unsigned int bs, float *A, int *ipiv, int *info )
{
	unsigned int i, j, jb = bs;

	int /*temps*/ nn, jjb, jj, mm, jjbb, incx = 1;

	for( j = 0 ; j < m ; j+=bs )
	{
		SGETF2( m-j, jb, &A[j*m+j], m, ipiv, info );

		// Adjust
		for( i = j ; i < j+jb ; i++ )
		{
			ipiv[i] = j + ipiv[i];
		}

		nn  = (int)j; jjb = (int)(j+jb); jj = (int)j; mm = (int)m; jjbb = (int)(j+jb);
		slaswp_( &nn, A, &mm, &jj, &jjbb, ipiv, &incx);

		if( j+jb+1 < n )
		{
			nn  = (int)(n-j-jb); jjb = (int)(j+jb); jj = (int)j; mm = (int)m; jjbb = (int)(j+jb);
			slaswp_( &nn, &A[j+jb], &mm, &jj, &jjbb, ipiv, &incx);


			cblas_strsm( 101, 141, 122, 111, 132, jb, n-j-jb, 1, &A[j*m+j], m, &A[j*m+j+jb], m );
		}
	}
}

void RUNsegsv( unsigned int size )
{
	float *A = (float*)malloc( size*size*4 );
	float *B = (float*)malloc( size*size*4 );

	unsigned int i, j;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			//A[i] = 1;
			A[i*size+j] = (float)i+((float)j/10);
			B[i*size+j] = 1;
			//C[i] = 1;
		}
	}

	printf( "Solving Ax=B. %u x %u\n", size,size );
	int *ipiv = (int*)malloc( size*4 );

	Tic();

	clapack_sgesv( 101, size, size, A, size, ipiv, B, size );

	PrintTicToc( "Time taken ", Toc() );

	free( A );
	free( B );
	free( ipiv );
}



void RUNSGETF2( unsigned int size, unsigned int bs )
{
	float *A = (float*)malloc( size*size*4 );

	unsigned int i, j;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			//A[i*size+j] = (float)i+((float)j/10);
			// a[j,i] = size*size-i*size+j
			A[i*size+j] = (float)(size*size-i*size+j);
		}
	}

	printf( "A is:\n" );
	Print( size, size, A );

	int *ipiv = (int*)malloc( size*4 );
	int info;
	SGETF2( size,bs,A, size, ipiv,&info );

	printf( "IPIV=" );
	for( i = 0 ; i < size ; i++ )
	{
		printf( "%i, ", ipiv[i] );
	}
	printf( "\n" );

	free( A );
}

void RUNSGETRF( unsigned int size, unsigned int bs )
{
	float *A = (float*)malloc( size*size*4 );

	unsigned int i, j;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			//A[i*size+j] = (float)i+((float)j/10);
			A[i*size+j] = (float)(size*size-i*size+j);
		}
	}

	printf( "A is:\n" );
	Print( size, size, A );

	int *ipiv = (int*)malloc( size*4 );
	int info = 0;
	SGETRF( size, size, 4, A, ipiv, &info );

	printf( "IPIV=" );
	for( i = 0 ; i < size ; i++ )
	{
		printf( "%i, ", ipiv[i] );
	}
	printf( "\n" );

	printf( "A is:\n" );
	Print( size, size, A );

	free( A );
}

void RUNSGETRF_atlas( unsigned int size, unsigned int bs )
{
	float *A = (float*)malloc( size*size*4 );

	unsigned int i, j;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			//A[i*size+j] = (float)i+((float)j/10);
			A[i*size+j] = (float)(size*size-i*size+j);
		}
	}

	printf( "A is:\n" );
	Print( size, size, A );

	int *ipiv = (int*)malloc( size*4 );
	int info = 0;

	/*
	 * int clapack_sgetrf(const enum CBLAS_ORDER Order, const int M, const int N,
                   float *A, const int lda, int *ipiv);
	 */
	clapack_sgetrf( 101, size, size, A, size, ipiv );

	printf( "IPIV=" );
	for( i = 0 ; i < size ; i++ )
	{
		printf( "%i, ", ipiv[i] );
	}
	printf( "\n" );

	printf( "A is:\n" );
	Print( size, size, A );

	free( A );
}


void RUNSGETRF_atlas_1( unsigned int size, unsigned int bs )
{
	float *A = (float*)malloc( size*size*4 );

	unsigned int i, j;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			//A[i*size+j] = (float)i+((float)j/10);
			A[j*size+i] = (float)(size*size-i*size+j);
		}
	}

	printf( "A is:\n" );
	PrintF( size, size, A );

	int *ipiv = (int*)malloc( size*4 );
	int info = 0;

	/*
	 * int clapack_sgetrf(const enum CBLAS_ORDER Order, const int M, const int N,
                   float *A, const int lda, int *ipiv);
	 */
	clapack_sgetrf( 102, size, size, A, size, ipiv );

	printf( "IPIV=" );
	for( i = 0 ; i < size ; i++ )
	{
		printf( "%i, ", ipiv[i] );
	}
	printf( "\n" );

	printf( "A is:\n" );
	PrintF( size, size, A );

	free( A );
}

void RUNSLASWP( unsigned int size, unsigned int bs )
{
	float *A = (float*)malloc( size*size*4 );

	unsigned int i, j;

	for(i=0;i<size;i++)
	{
		for(j=0;j<size;j++)
		{
			//A[i*size+j] = (float)i+((float)j/10);
			A[i*size+j] = (float)(size*size-i*size+j);
		}
	}

	printf( "A is:\n" );
	Print( size, size, A );

	int *ipiv = (int*)malloc( size*4 );
	int info = 0;
	SGETRF( size, size, 4, A, ipiv, &info );

	printf( "IPIV=" );
	for( i = 0 ; i < size ; i++ )
	{
		printf( "%i, ", ipiv[i] );
	}
	printf( "\n" );

	free( A );
}

void SGEMMTEST()
{
	float *A = (float*)malloc( 4*4*4 ); // COL matrix
	float *B = (float*)malloc( 4*4*4 ); // ROW matrix
	float *C = (float*)malloc( 4*4*4 );

	A[0]  = 0.5;  A[1]  = 0.57; A[2]  = 0.38; A[3]  = 0.0;
	A[4]  = 0.38; A[5]  = 0.71; A[6]  = 0.25; A[7]  = 0.0;
	A[8]  = 0.75; A[9]  = 0.29; A[10] = 0.19; A[11] = 0.0;
	A[12] = 0.88; A[13] = 0.14; A[14] = 0.09; A[15] = 0.0;

	B[0]  = 68;  B[1]  = 69;   B[2]  = 70;   B[3]  = 71;
	B[4]  = 3.5; B[5]  = 4.38; B[6]  = 5.25; B[7]  = 6.12;
	B[8]  = -0;  B[9]  = -0;   B[10] = -0;   B[11] = -0;
	B[12] = 0;   B[13] = 0;    B[14] = 0;    B[15] = 0;

	C[0]  = 36; C[1]  = 37; C[2]  = 38; C[3]  = 39;
	C[4]  = 28; C[5]  = 29; C[6]  = 30; C[7]  = 31;
	C[8]  = 52; C[9]  = 53; C[10] = 54; C[11] = 55;
	C[12] = 60; C[13] = 61; C[14] = 62; C[15] = 63;

	printf( "A:\n" );
	Print( 4,4,A );
	printf( "B:\n" );
	Print( 4,4,B );
	printf( "C:\n" );
	Print( 4,4,C );

	cblas_sgemm( 101, 111, 111, 4, 4, 4, -1, A, 4, B, 4, 1, C, 4 );

	printf( "C:\n" );
	Print( 4,4,C );

}

int main()
{
//	RUNsgemm(1024);
//	RUNsgemm(1024);
//	RUNsgemm(2048);
//	RUNsgemm(2048);
//	RUNsgemm(3072);
//	RUNsgemm(3072);
//	RUNsgemm(4096);
//	RUNsgemm(4096);

//	RUNstrsm(1024);
//	RUNstrsm(1024);
//	RUNstrsm(2048);
//	RUNstrsm(2048);
//	RUNstrsm(3072);
//	RUNstrsm(3072);
//	RUNstrsm(4096);
//	RUNstrsm(4096);

	RUNsger(1024);
	RUNsger(1024);
	RUNsger(2048);
	RUNsger(2048);
	RUNsger(3072);
	RUNsger(3072);
	RUNsger(4096);
	RUNsger(4096);

//	RUNsegsv(1024);
//	RUNsegsv(1024);
//	RUNsegsv(2048);
//	RUNsegsv(2048);
//	RUNsegsv(3072);
//	RUNsegsv(3072);
//	RUNsegsv(4096);
//	RUNsegsv(4096);

	//RUNSGETF2( 8,4 );
	//RUNSGETRF_atlas_1( 8,4 );
	//RUNSGETRF( 8,4 );
	//SGEMMTEST();
	/*
	 * void cblas_strsm(const enum CBLAS_ORDER Order, const enum CBLAS_SIDE Side,
                 const enum CBLAS_UPLO Uplo, const enum CBLAS_TRANSPOSE TransA,
                 const enum CBLAS_DIAG Diag, const int M, const int N,
                 const float alpha, const float *A, const int lda,
                 float *B, const int ldb);
	 */

	/*
	 * void cblas_sger(const enum CBLAS_ORDER Order, const int M, const int N,
                const float alpha, const float *X, const int incX,
                const float *Y, const int incY, float *A, const int lda);
	 */


//	int *ipiv = (int*)malloc( m*4 );
//	int info;
//	SGETF2( 8,4,A, 8,ipiv,&info );

//	// SOLVE
//
//	printf( "A:\n" );
//	Print( m, n, A );
//	printf( "B:\n" );
//	Print( m, n, B );
//
//	int *ipiv = (int*)malloc( m*4 );
//	clapack_sgetrf( 101, m, n, A, n, ipiv );
//
//	printf( "IPIV=" );
//	for( i = 0 ; i < m ; i++ )
//	{
//		printf( "%i, ", ipiv[i] );
//	}
//
//	printf( "\nA:\n" );
//	Print( m, n, A );
//
//	clapack_sgetrs( 101, 111, n, m, A, n, ipiv, B, n );
//
//	printf( "B:\n" );
//	Print( m, n, B );

	/*
	 * int clapack_sgesv(const enum CBLAS_ORDER Order, const int N, const int NRHS,
                  float *A, const int lda, int *ipiv,
                  float *B, const int ldb);
	 */



//	printf( "A:\n" );
//	Print( m, n, A );
//	printf( "B:\n" );
//	Print( m, n, B );
//


//	printf( "T=%i\n", t );
//
//	printf( "B:\n" );
//	Print( m, n, B );

//
//	/*
//	 * int clapack_sgetrs
//   (const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE Trans,
//    const int N, const int NRHS, const float *A, const int lda,
//    const int *ipiv, float *B, const int ldb);
//	 */
//	clapack_sgetrs( 101, 111, n, m, A, n, ipiv, B, n );
//
//	Tic();
//
//	printf( "A:\n" );
//	Print( m, n, A );

//	unsigned int jb = 64;
//	cblas_strsm( 101, 141, 122, 111, 132, jb, n-jb, alpha, A, m, &B[jb], m );

	// STRSM TEST

//	A[0] = 1;A[1] = 1;A[2] = 1;A[3] = 1;A[4] = 1;A[5] = 1;A[6] = 1;A[7] = 1;
//	A[8] = 1;A[16] = 1;A[24] = 1;A[32] = 1;A[40] = 1;A[48] = 1;A[56] = 1;
//
//	printf("A:\n");
//	Print( m, n, A );
//
//	cblas_strsm( 101, 141, 122, 111, 132, m, n, alpha, A, m, B, m );
//
//	printf("B1:\n");
//	Print( m, n, B );
//
//	cblas_strsm( 101, 141, 121, 111, 131, m, n, alpha, A, m, B, m );
//
//	printf("B2:\n");
//	Print( m, n, B );

	// First BLOCK

//	int *ipiv = (int*)malloc( m*4 );
//
//	Print( m, n, A );
//
//
//	printf( "SGETRF:\n" );
//	clapack_sgetrf( 101, m, 4, A, n, ipiv );
//
//	Print( m, n, A );
//
//	printf( "IPIV=" );
//	for( i = 0 ; i < m ; i++ )
//	{
//		printf( "%i, ", ipiv[i] );
//	}
//
//	printf("\n");
//
//	//printf( "INFO:%i\n", *info );
//
//	printf( "STRSM:\n" );
//	unsigned int jb = 4;
//	cblas_strsm( 101, 141, 122, 111, 132, jb, n-jb, alpha, A, m, &A[jb], m );
//
//	Print( m, n, A );
//
//	cblas_sgemm( 101, 111, 111, m-jb, n-jb, jb, -1, &A[jb*n], m, &A[jb], m, 1, &A[jb*n+jb], m );
//
//	printf( "After SGEMM:\n" );
//	Print( m, n, A );

//	/*void cblas_sgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
//	                 const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
//	                 const int K, const float alpha, const float *A,
//	                 const int lda, const float *B, const int ldb,
//	                 const float beta, float *C, const int ldc);*/
//
//	printf( "SGEMM:\n" );
//	cblas_sgemm( 101, 111, 111, m-jb, n-jb, jb, -1, &A[jb*n], m, &B[jb], m, 1, &A[jb*n+jb], m );
//	cblas_sgemm( 101, 111, 111, m, m, m, -1, A, m, B, m, 1, C, m );

	//cblas_sgemm( 101, 111, 111, m, m, m, -1, A, m, A, m, 1, C, m );
//
//	Print( m, n, A );
//
//	// SGER
//
//	//cblas_sger( 101, m, n, alpha, x, 1, y, 1, A, m );
//
//	// Calling STRSM
//
//	// LLNU
//	//unsigned int jb = 64;
//	//cblas_strsm( 101, 141, 122, 111, 132, m, n, alpha, A, m, B, m );
//	//cblas_strsm( 101, 141, 122, 111, 132, jb, n-jb, alpha, A, m, &B[jb], m );
//
//	//cblas_strsm( 101, 141, 122, 111, 132, m, n, alpha, A, m, B, m );
//	//cblas_strsm( 101, 141, 121, 111, 131, m, n, alpha, A, m, B, m );
//
//
//

//
//	printf( "BBBBB:\n" );
//	clapack_sgetrf( 101, m, n, B, n, ipiv2 );
//
//
	//Print( m, n, A );


	return 0;
}
