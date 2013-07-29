#ifndef SGETF2_H_
#define SGETF2_H_

#define _NUMBLOCKS 1

unsigned int counter;

#include <timer.h>

//void sgetf2_blocked( PyArrayObject *A, unsigned int jblock, unsigned int mb, unsigned int nb, unsigned int *ipiv, unsigned int *info );
void sgetf2_blocked( PyArrayObject *A, unsigned int jb, unsigned int mb, unsigned int nb, unsigned int *ipiv, unsigned int *info, /* CELL */ Solve_t *solve );

//void sgetf2( PyArrayObject *A, unsigned int jj, unsigned int *ipiv, unsigned int *info );
void sgetf2( PyArrayObject *A, unsigned int jj, unsigned int m, unsigned int n, unsigned int *ipiv, unsigned int *info );


#endif /* SGETF2_H_ */
