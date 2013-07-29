/*
 * timer.h
 *
 *  Created on: Feb 20, 2009
 *      Author: wolle
 */

#ifndef TIMER_H_
#define TIMER_H_

#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include "timer.h"

struct timeval STARTtv, ENDtv, REStv;
struct timezone tz;
typedef struct timeval timeval_t;
//struct tm *tm;

typedef struct
{
	timeval_t       starttv;
	timeval_t       endtv;
	timeval_t       restv;
	struct timezone tz;
	double          time;
} TimerObject;



TimerObject *GetTimer();
void StartTimer( TimerObject *to );
void StopTimer( TimerObject *to );
void PrintTimer( char *text, TimerObject *to );

clock_t myTime;

void Tic();

timeval_t Toc();

double Toc_d();

void PrintTicToc( char *text, timeval_t tt );

void GetTicToc( timeval_t tt );

unsigned int GetTime();

void pputimer_GetStats( double seconds, unsigned int bytes, unsigned int numops );
double pputimer_GetGBS( double seconds, unsigned int bytes );

void PrintTime( char *text, unsigned int mtime );

#endif /*TIMER_H_*/

