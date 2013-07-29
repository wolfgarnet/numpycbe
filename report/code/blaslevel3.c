/* performs one of the matrix vector operations
   y = ALPHA * A * x + BETA * y   or   y = ALPHA * A' * x + BETA * y,
   depending on TRANS.
   Prefixes: S, D, C, Z */
void xGEMV ( enum TRANS, unsigned M, unsigned N, real ALPHA, float *A, int lda, 
             real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
/* performs one of the matrix vector operations
   y = ALPHA * A * x + BETA * y   or   y = ALPHA * A' * x + BETA * y,
   depending on TRANS, with kl sub-diagonals and ku super-diagonals.
   Prefixes: S, D, C, Z */
void xGBMV ( enum TRANS, unsigned M, unsigned N, real ALPHA, float *A, int lda, 
             int kl, int ku, real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
