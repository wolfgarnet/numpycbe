/* Setup a Givens rotation to two vectors, normally rows or columns of a matrix 
   Prefixes: S, D */
void xROTG ( real *A, real *B, real *C, real *S );
/* Setup a modified Givens rotation to two vectors 
   Prefixes: S, D */
void xROTMG( real *D1, real *D2, real *A, real B, real *PARAM );
/* Applies a Givens rotation 
   Prefixes: S, D */
void xROT  ( unsigned N, real *X, unsigned INCX, real *Y, unsigned INCY, real C, real S );
/* Applies a modified Givens rotation 
   Prefixes: S, D */
void xROTM ( unsigned N, real *X, unsigned INCX, real *Y, unsigned INCY, real PARAM );
/* Swaps two vectors 
   Prefixes: S, D, C, Z */
void xSWAP ( unsigned N, real *X, unsigned INCX, real *Y, unsigned INCY );
/* Scales a vector 
   Prefixes: S, D, C, Z, CS, ZD */
void xSCAL ( unsigned N, real A, real *X, unsigned INCX );
/* Copy one vector into another
   Prefixes: S, D, C, Z  */
void xCOPY ( unsigned N, real *X, unsigned INCX, real *Y, unsigned INCY );
/* Adds a scaled vector to another, y = a * x + y 
   Prefixes: S, D, C, Z */
void xAXPY ( unsigned N, real A, real *X, unsigned INCX, real *Y, unsinged INCY );
/* DOT = inner product, SUM(I=1 to N) of SX(I)*SY(I) 
   Prefixes: S, D, DS */
real xDOT  ( unsigned N, real *X, unsigned INCX, real *Y, unsigned INCY );
/* Unconjugated dot product of two complex vectors 
   Prefixes: C, Z*/
cplx CDOTU ( unsigned N, cplx *X, unsinged INCX, cplx *Y, unsinged INCY );
/* Conjugated dot product of two complex vectors 
   Prefixes: C, Z*/
cplx CDOTC ( unsigned N, cplx *X, unsinged INCX, cplx *Y, unsigned INCY );
/* The Euclidean norm of a real vector(root-mean-square norm) 
   Prefixes: S, D, SC, DZ */
real xNRM2 ( unsinged N, real *X,unsigned INCX );
/* Returns the sum of the elements 
   Prefixes: S, D, SC, DZ */
real xSUM  ( unsinged N, real *X,unsigned INCX );
/* Return the index of the element with the largest absolute value 
   Prefixes: S, D, C, Z */
unsigned IxAMAX( unsigned N, real *X,unsigned INCX );






