/*
 * tests.h
 *
 *  Created on: Jun 8, 2009
 *      Author: wolle
 */

#ifndef TESTS_H_
#define TESTS_H_

#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <sys/time.h>
#include <math.h>

struct timeval STARTtv, ENDtv, REStv;
struct timezone tz;
typedef struct timeval timeval_t;

#define Malloc(x) malloc(x)

#endif /* TESTS_H_ */
