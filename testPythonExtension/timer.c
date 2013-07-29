/*
 * timer.c
 *
 *  Created on: Feb 20, 2009
 *      Author: wolle
 */

#include "timer.h"
#include <stdio.h>

clock_t myTime;

void Tic()
{
	myTime = clock ();
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
