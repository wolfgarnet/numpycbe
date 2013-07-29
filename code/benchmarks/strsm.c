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

struct timeval STARTtv, ENDtv, REStv;
struct timezone tz;
typedef struct timeval timeval_t;

void Tic()
{
	gettimeofday( &STARTtv, &tz );
}

timeval_t Toc()
{
	gettimeofday( &ENDtv, &tz );

	REStv.tv_sec  = ( ENDtv.tv_sec - STARTtv.tv_sec );
	REStv.tv_usec = ( ENDtv.tv_usec - STARTtv.tv_usec );

    if ( REStv.tv_usec < 0 ) {
		REStv.tv_usec += 1000000 ;
		REStv.tv_sec-- ;
    }
    else if ( REStv.tv_usec >= 1000000 )
	{
		REStv.tv_usec -= 1000000 ;
		REStv.tv_sec++ ;
    }

	return REStv;
}

double Toc_d()
{
	gettimeofday( &ENDtv, &tz );

	REStv.tv_sec  = ( ENDtv.tv_sec - STARTtv.tv_sec );
	REStv.tv_usec = ( ENDtv.tv_usec - STARTtv.tv_usec );

    if ( REStv.tv_usec < 0 ) {
		REStv.tv_usec += 1000000 ;
		REStv.tv_sec-- ;
    }
    else if ( REStv.tv_usec >= 1000000 )
	{
		REStv.tv_usec -= 1000000 ;
		REStv.tv_sec++ ;
    }

	return (double)REStv.tv_sec + ( (double)REStv.tv_usec / 1000000 );
}

void PrintTicToc( char *text, timeval_t tt )
{
	unsigned int micro = 0, milli = 0, seconds = 0, minutes = 0;

	if ( tt.tv_sec >= 60 )
	{
		minutes   = floor( ( (double)tt.tv_sec / 60 ) );
		tt.tv_sec = tt.tv_sec - minutes * 60;
	}

	seconds = tt.tv_sec;

	if ( tt.tv_usec >= 1000 )
	{
		milli      = floor( ( (double)tt.tv_usec / 1000 ) );
		tt.tv_usec = tt.tv_usec - milli * 1000;
	}

	micro = tt.tv_usec;

	printf( "%s%dm %ds %dms %dus\n", text, minutes, seconds, milli, micro );
}

void Print( unsigned int m, unsigned int n, float *data )
{
	unsigned int i,j;
	for( i = 0 ; i < m ; i++ )
	{
		printf( "%u: ", i );
		for( j = 0 ; j < n ; j++ )
		{
			printf( "%.2f ", data[i*n+j] );
		}
		printf( "\n" );
	}
}

void RUNSGEMM( unsigned int size )
{
	float *A = (float*)malloc( size*size*4 );
	float *B = (float*)malloc( size*size*4 );

	unsigned int i;

	for(i=0;i<size*size;i++)
	{
		A[i] = 1;
		B[i] = 1;
	}

	Tic();

	cblas_sgemm( 101, 111, 111, size, size, size, -1, A, size, A, size, 1, B, size );

	PrintTicToc( "Time taken: ", Toc() );
}

int main()
{

  RUNSGEMM(1024);

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


  


//	unsigned int jb = 64;
//	cblas_strsm( 101, 141, 122, 111, 132, jb, n-jb, alpha, A, m, &B[jb], m );



//
//	int *ipiv = (int*)malloc( m*4 );
//	int *ipiv2 = (int*)malloc( m*4 );
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
//	/*void cblas_sgemm(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
//	                 const enum CBLAS_TRANSPOSE TransB, const int M, const int N,
//	                 const int K, const float alpha, const float *A,
//	                 const int lda, const float *B, const int ldb,
//	                 const float beta, float *C, const int ldc);*/
//
//	printf( "SGEMM:\n" );
//	cblas_sgemm( 101, 111, 111, m-jb, n-jb, jb, -1, &A[jb*n], m, &B[jb], m, 1, &A[jb*n+jb], m );
//	cblas_sgemm( 101, 111, 111, m, m, m, -1, A, m, B, m, 1, C, m );


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
//	Print( m, n, B );


	return 0;
}
