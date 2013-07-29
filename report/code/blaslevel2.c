/* performs one of the matrix vector operations
   y = ALPHA * A * x + BETA * y   or   y = ALPHA * A' * x + BETA * y,
   depending on TRANS.
   Prefixes: S, D, C, Z */
void xGEMV ( enum TRANS, unsigned M, unsigned N, real ALPHA, float *A, int lda, 
             real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
/* performs one of the matrix vector operations
   y = ALPHA * A * x + BETA * y   or   y = ALPHA * A' * x + BETA * y,
   depending on TRANS, with KL sub-diagonals and KU super-diagonals.
   Prefixes: S, D, C, Z */
void xGBMV ( enum TRANS, unsigned M, unsigned N, real ALPHA, float *A, int lda, 
             int KL, int KU, real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
/* Performs a hermitian matrix vector multiplication of the form,
   y = ALPHA * A * x + BETA * y,
   where A is an n by n hermitian matrix
*/
void xHEMV ( enum UPLO, unsigned N, real ALPHA, float *A, int lda, 
             real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
/* Performs a hermitian matrix vector multiplication of the form,
   y = ALPHA * A * x + BETA * y,
   where A is an n by n hermitian matrix with K super diagonals
*/
void xHBMV ( enum UPLO, unsigned N, int K, real ALPHA, float *A, int lda, 
             real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
/* Performs a hermitian matrix vector multiplication of the form,
   y = ALPHA * A * x + BETA * y,
   where A is an n by n packed hermitian matrix
*/
void xHPMV ( enum UPLO, unsigned N, real ALPHA, float *AP, 
             real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
/* Performs a matrix vector multiplication of the form,
   y = ALPHA * A * x + BETA * y,
   where A is an n by n symmetric matrix
*/
void xSYMV ( enum UPLO, unsigned N, real ALPHA, float *A, int lda,
             real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
/* Performs a matrix vector multiplication of the form,
   y = ALPHA * A * x + BETA * y,
   where A is an n by n symmetric matrix with K super diagonals
*/
void xSBMV ( enum UPLO, unsigned N, int K, real ALPHA, float *A, int lda,
             real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
/* Performs a matrix vector multiplication of the form,
   y = ALPHA * A * x + BETA * y,
   where A is an n by n symmetric packed matrix
*/
void xSPMV ( enum UPLO, unsigned N, real ALPHA, float *A,
             real *X, unsigned INCX, real BETA, real *y, unsigned INCY );
