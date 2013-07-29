/*
 * libdispatcher.h
 *
 *  Created on: Mar 23, 2009
 *      Author: wolle
 */

#ifndef LIBDISPATCHER_H_
#define LIBDISPATCHER_H_

#include "../common/common.h"
#include "../common/common_spu.h"

unsigned int mb_getmbox( );
void GetSPEAddr( unsigned int ea, unsigned int *PPE_addr );
void GetOperation( unsigned int ea, Operation_t *data );
void GetShader( unsigned int EA, unsigned int size, unsigned int *shader );

#endif /* LIBDISPATCHER_H_ */
