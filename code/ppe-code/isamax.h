#ifndef ISAMAX_H_
#define ISAMAX_H_

unsigned int FHB_isamax_row( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A );
unsigned int FHB_isamax_col( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A );
unsigned int FHB_isamax_col2( unsigned int col, unsigned int row, PyArrayObject *A );
unsigned int FHB_isamax_col_blocked( unsigned int offset, unsigned int col, unsigned int row, PyArrayObject *A );

#endif /* ISAMAX_H_ */
