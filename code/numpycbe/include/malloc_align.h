/* --------------------------------------------------------------  */
/* (C)Copyright 2001,2007,                                         */
/* International Business Machines Corporation,                    */
/* Sony Computer Entertainment, Incorporated,                      */
/* Toshiba Corporation,                                            */
/*                                                                 */
/* All Rights Reserved.                                            */
/*                                                                 */
/* Redistribution and use in source and binary forms, with or      */
/* without modification, are permitted provided that the           */
/* following conditions are met:                                   */
/*                                                                 */
/* - Redistributions of source code must retain the above copyright*/
/*   notice, this list of conditions and the following disclaimer. */
/*                                                                 */
/* - Redistributions in binary form must reproduce the above       */
/*   copyright notice, this list of conditions and the following   */
/*   disclaimer in the documentation and/or other materials        */
/*   provided with the distribution.                               */
/*                                                                 */
/* - Neither the name of IBM Corporation nor the names of its      */
/*   contributors may be used to endorse or promote products       */
/*   derived from this software without specific prior written     */
/*   permission.                                                   */
/*                                                                 */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND          */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,     */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF        */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE        */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR            */
/* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT    */
/* NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;    */
/* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)        */
/* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN       */
/* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR    */
/* OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,  */
/* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              */
/* --------------------------------------------------------------  */
/* PROLOG END TAG zYx                                              */

#ifndef _MALLOC_ALIGN_H_
#define _MALLOC_ALIGN_H_	1

#include <stdlib.h>

/* Function
 *
 *	void * malloc_align(size_t size, unsigned int log2_align)
 *
 * Description
 * 	The malloc_align routine allocates a memory buffer of <size>
 * 	bytes aligned to the power of 2 alignment specified by <log2_align>.
 *	For example, malloc_align(4096, 7) will allocate a memory heap 
 *	buffer of 4096 bytes aligned on a 128 byte boundary.
 *
 * 	The aligned malloc routine allocates an enlarged buffer
 * 	from the standard memory heap. Space for the real allocated memory
 * 	pointer is reserved on the front of the memory buffer.
 *
 *	      ----------------- <--- start of allocated memory
 *	     |    pad 0 to     |
 *	     |(1<<log2_align)-1|
 *	     |     bytes       |
 *	     |-----------------|
 *	     | allocation size |
 *	     |-----------------|
 *	     | real buffer ptr |
 *	     |-----------------|<---- returned aligned memory pointer
 *	     |                 |
 *	     |    requested    |
 *	     |     memory      |
 *	     |     buffer      |
 *	     |      size       |
 *	     |      bytes      |
 *	     |_________________|
 *
 *      Memory allocated by this routine must be freed using the free_align
 *	routine.
 *
 *	The size of the allocation is saved for special cases where the 
 *	data must be mored during a realloc_align.
 */

static __inline void * _malloc_align(size_t size, unsigned int log2_align)
{
  void *ret;
  char *real;
  unsigned long offset;
  unsigned long align;
  
  align = 1 << log2_align;
  real = (char *)malloc(size + 2*sizeof(void *) + (align-1));
  if (real) {
    offset = (align - (unsigned long)(real + 2*sizeof(void *))) & (align-1);
    ret = (void *)((real + 2*sizeof(void *)) + offset);
    *((size_t *)(ret)-2) = size;
    *((void **)(ret)-1) = (void *)(real);
  } else {
    ret = (void *)(real);
  }
  return (ret);
}

#endif /* _MALLOC_ALIGN_H_ */
