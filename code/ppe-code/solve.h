#ifndef SOLVE_H_
#define SOLVE_H_

void sgesv( PyArrayObject *A, PyArrayObject *B, unsigned int *info );

unsigned int FHB_isamax_row( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A );
unsigned int FHB_isamax_col( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A );
unsigned int FHB_isamax_col2( unsigned int col, unsigned int row, PyArrayObject *A );
unsigned int FHB_isamax_col_blocked( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A );

void sgetf2_blocked( PyArrayObject *A, unsigned int jb, unsigned int mb, unsigned int nb, unsigned int *ipiv, unsigned int *info, /* CELL */ Solve_t *solve );

void sgetrf( PyArrayObject *A, unsigned int *ipiv, unsigned int *info, Solve_t *solve );

void sgetrs( PyArrayObject *A, PyArrayObject *B, unsigned int *ipiv, unsigned int *info, Solve_t *solve );

void slaswp( unsigned int n, PyArrayObject *A, unsigned int k1, unsigned int k2, unsigned int offset, unsigned int *ipiv );
void slaswp_blocked( unsigned int nb, PyArrayObject *A, unsigned int offsetb, unsigned int yb, unsigned int *ipiv );

void FHB_sscal_col( unsigned int offset, unsigned int col, unsigned int row, float scale, PyArrayObject *A );
void FHB_sscal_col_blocked( unsigned int col, unsigned int row, float scale, PyArrayObject *A );

void FHB_sswap_row( PyArrayObject *A, unsigned int row1, unsigned int row2 );
void FHB_sswap_row_blocked( PyArrayObject *A, unsigned int blockcol, unsigned int row1, unsigned int row2 );

void InitMultVectors( PyArrayObject *multVector1, PyArrayObject *multVector2, PyArrayObject *multVector3, unsigned int numBlocksX, unsigned int numBlocksY, unsigned int _blockSize );
void CreateTempArrays( PyArrayObject *multVector1, PyArrayObject *multVector2, PyArrayObject *multVector3, PyArrayObject *arg1, unsigned int d );

#endif /* SOLVE_H_ */
