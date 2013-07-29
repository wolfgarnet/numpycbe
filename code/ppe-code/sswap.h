#ifndef SSWAP_H_
#define SSWAP_H_

void FHB_sswap_row( PyArrayObject *A, unsigned int row1, unsigned int row2 );
void FHB_sswap_row_blocked( PyArrayObject *A, unsigned int blockcol, unsigned int row1, unsigned int row2 );

#endif /* SSWAP_H_ */
