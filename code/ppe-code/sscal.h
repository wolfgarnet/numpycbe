#ifndef SSCAL_H_
#define SSCAL_H_

void FHB_sscal_col( unsigned int offset, unsigned int col, unsigned int row, float scale, PyArrayObject *A );
void FHB_sscal_col_blocked( unsigned int col, unsigned int row, float scale, PyArrayObject *A );

#endif /* SSCAL_H_ */
