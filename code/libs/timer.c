/*
 * timer.c
 *
 *  Created on: Feb 20, 2009
 *      Author: wolle
 */

#include "timer.h"
#include <stdio.h>

clock_t myTime;

TimerObject *GetTimer()
{
	TimerObject *to = (TimerObject *)malloc( sizeof(TimerObject) );
	to->time = 0;

	return to;
}

/*
 * Start timers
 */

void Tic()
{
	myTime = clock ();
	gettimeofday( &STARTtv, &tz );
}

void StartTimer( TimerObject *to )
{
	gettimeofday( &to->starttv, &to->tz );
}

/*
 * End timers
 */

void StopTimer( TimerObject *to )
{
	gettimeofday( &to->endtv, &to->tz );

	to->restv.tv_sec  = ( to->endtv.tv_sec - to->starttv.tv_sec );
	to->restv.tv_usec = ( to->endtv.tv_usec - to->starttv.tv_usec );

    if ( to->restv.tv_usec < 0 ) {
    	to->restv.tv_usec += 1000000 ;
    	to->restv.tv_sec-- ;
    }
    else if ( to->restv.tv_usec >= 1000000 )
	{
    	to->restv.tv_usec -= 1000000 ;
    	to->restv.tv_sec++ ;
    }

    to->time += (double)to->restv.tv_sec + ( (double)to->restv.tv_usec / 1000000 );
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

/*
 * Prints
 */

void PrintTimer( char *text, TimerObject *to )
{
	unsigned int micro = 0, milli = 0, seconds = 0, minutes = 0;
	double time = to->time;

	if ( to->time >= 60 )
	{
		minutes   = floor( ( time / 60 ) );
		time = time - minutes * 60;
	}

	if ( time >= 1 )
	{
		seconds = floor( time );
		time = time - seconds;
	}

	time *= 1000;

	if ( time >= 1 )
	{
		milli = floor( time );
		time = time - milli;
	}

	micro = (unsigned int)( time * 1000 );

	printf( "%s%dm %ds %dms %dus\n", text, minutes, seconds, milli, micro );
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



/*
 * Stats
 *
 */

void pputimer_GetStats( double seconds, unsigned int bytes, unsigned int numops )
{
	double GB = (double)bytes / (1024*1024*1024);

  	printf( "--------------- STATS ------------------------------\n" );
  	printf( "Seconds: %f\n", seconds );
  	printf( "GigaBytes: %f\n", GB );
  	printf( "MegaBytes: %f\n", GB*1024 );
  	printf( "GB/s: %f\n", GB/seconds );
  	printf( "GFlops: %f\n", ( numops/seconds ) / ( 1024*1024*1024 ) );
  	printf( "----------------------------------------------------\n" );
}

double pputimer_GetGBS( double seconds, unsigned int bytes )
{
	double GB = (double)bytes / (1024*1024*1024);

  	return GB/seconds;
}

/*
 *
 *
 */


void GetTicToc( timeval_t tt )
{
	printf( "%d.%d", (unsigned int)tt.tv_sec, (unsigned int)tt.tv_usec );
}

unsigned int GetTime()
{
	return (unsigned int)clock ();
}

void PrintTime( char *text, unsigned int mtime )
{
	mtime /= CLOCKS_PER_SEC / 1000;
	unsigned int minutes = 0, seconds = 0, mseconds = 0;

	if( mtime > 60000 )
	{
		minutes = mtime / 60000;
		mtime   = mtime % 60000;
	}

	if( mtime > 1000 )
	{
		seconds = mtime / 1000;
		mtime   = mtime % 1000;
	}

	mseconds = mtime;

	printf( "%s%dm %ds %dms\n", text, minutes, seconds, mseconds );
}
