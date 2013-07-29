/*
 * tests.c
 *
 *  Created on: Jun 8, 2009
 *      Author: wolle
 */

#include "tests.h"

#include <cblas.h>


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
			//printf( "%e ", data[i*n+j] );
		}
		printf( "\n" );
	}
}

void PrintF( unsigned int m, unsigned int n, float *data )
{
	unsigned int i,j;
	for( i = 0 ; i < m ; i++ )
	{
		printf( "%u: ", i );
		for( j = 0 ; j < n ; j++ )
		{
			printf( "%.2f ", data[j*n+i] );
			//printf( "%e ", data[i*n+j] );
		}
		printf( "\n" );
	}
}

void RUNsgemm( unsigned int size )
{
	float *A = (float*)Malloc( size*size*4 );
	float *B = (float*)Malloc( size*size*4 );

	unsigned int i;

	for(i=0;i<size*size;i++)
	{
		A[i] = 1;
		B[i] = 1;
	}

	Tic();

	cblas_sgemm( 101, 111, 111, size, size, size, -1, A, size, A, size, 1, B, size );

	PrintTicToc( "Time taken: ", Toc() );

	free(A);
	free(B);
}

void RUNstrsm( unsigned int size )
{
	float *A = (float*)Malloc( size*size*4 );
	float *B = (float*)Malloc( size*size*4 );

	unsigned int i;

	for(i=0;i<size*size;i++)
	{
		A[i] = 1;
		B[i] = 1;
	}

	Tic();

	cblas_strsm( 101, 141, 121, 111, 131, size, size, 1, A, size, B, size );

	PrintTicToc( "Time taken: ", Toc() );

	free(A);
	free(B);
}

void RUNsger( unsigned int size )
{
	float *A = (float*)Malloc( size*size*4 );

	float *x = (float*)Malloc( size*4 );
	float *y = (float*)Malloc( size*4 );

	unsigned int i;

	for(i=0;i<size*size;i++)
	{
		A[i] = 1;
	}

	for(i=0;i<size;i++)
	{
		x[i] = 1;
		y[i] = 1;
	}

	Tic();

	/*
	 * void cblas_sger(const enum CBLAS_ORDER Order, const int M, const int N,
                const float alpha, const float *X, const int incX,
                const float *Y, const int incY, float *A, const int lda);
	 *
	 */

	cblas_sger( 101, size, size, 1, x, 1, y, 1, A, size );

	PrintTicToc( "Time taken: ", Toc() );

	free(A);
	free(x);
	free(y);
}

