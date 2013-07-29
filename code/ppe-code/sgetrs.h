#ifndef SGETRS_H_
#define SGETRS_H_

void sgetrs( PyArrayObject *A, PyArrayObject *B, unsigned int *ipiv, unsigned int *info, Solve_t *solve );

#endif /* SGETRS_H_ */
