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

struct timeval STARTtv, ENDtv, REStv;
struct timezone tz;
typedef struct timeval timeval_t;
//struct tm *tm;


clock_t myTime;

void Tic();

timeval_t Toc();

double Toc_d();

void PrintTicToc( char *text, timeval_t tt );

void GetTicToc( timeval_t tt );

unsigned int GetTime();

void PrintTime( char *text, unsigned int mtime );

#endif /*TIMER_H_*/

