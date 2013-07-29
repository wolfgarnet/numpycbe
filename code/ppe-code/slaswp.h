#ifndef SLASWP_H_
#define SLASWP_H_

void slaswp( unsigned int n, PyArrayObject *A, unsigned int k1, unsigned int k2, unsigned int offset, unsigned int *ipiv );
void slaswp_blocked( unsigned int nb, PyArrayObject *A, unsigned int offsetb, unsigned int yb, unsigned int *ipiv );

#endif /* SLASWP_H_ */
