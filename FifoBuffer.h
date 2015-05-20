/**
*	@file 	FifoBuffer.h
*	@brief
*	@date 2015-03-27
*	@author	James Ethridge <jeethridge@gmail.com>
*
*
* Copyright (c) 2014, James Ethridge
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
* The software was written using the following sources for reference:
* [1] Philip Thrasher's Crazy Awesome Ring Buffer Macros : https://github.com/pthrasher/c-generic-ring-buffer
* [2] http://en.wikipedia.org/wiki/Circular_buffer#Read_.2F_Write_Counts
* [3] "Excercise 44: Ring Buffer", Learn C The Hard Way,  http://c.learncodethehardway.org/book/ex44.htm
*
*/


/** <Package>
*	@addtogroup <Package>
*	@{
*/

/** FifoBuffer
*	@addtogroup FifoBuffer
*	@{
*/
/* ***** Inclusion Gaurd ****** */
#ifndef __FifoBuffer_H__
#define __FifoBuffer_H__

/* ***** Dependencies ****** */
//#include "<file2>.h"

/** FifoBuffer_Public_Macros
*	@addtogroup FifoBuffer_Public_Macros
*	@{
*/
#define FifoBuffer_typedef(T, NAME) \
  typedef struct { \
    int size; \
    int start; \
    int end; \
    int write_count;\
    int read_count;\
    T* elems; \
  } NAME

#define FifoBuffer_init(BUF, S, T, BUFMEM) \
  BUF->size = S; \
  BUF->start = 0; \
  BUF->end = 0; \
  BUF->read_count=0;\
  BUF->write_count=0;\
  BUF->elems = (T*)BUFMEM

#define FifoBuffer_write(BUF, ELEM)\
    BUF->elems[BUF->end]=ELEM;\
    BUF->write_count++;\
    BUF->end=(BUF->end+1)%BUF->size;

#define FifoBuffer_read(BUF, ELEM)\
    ELEM=BUF->elems[BUF->start];\
    BUF->read_count++;\
    BUF->start=(BUF->start+1)%BUF->size;

#define FifoBuffer_peek(BUF,ELEM,INDEX)\
    ELEM=BUF->elems[BUF->start+INDEX];

#define FifoBuffer_flush(BUF)\
    BUF->start = 0; \
    BUF->end = 0; \
    BUF->read_count=0;\
    BUF->write_count=0;

#define FifoBuffer_count(BUF) (BUF->write_count-BUF->read_count)
#define FifoBuffer_is_full(BUF) (FifoBuffer_count(BUF)==BUF->size)
#define FifoBuffer_is_empty(BUF) (FifoBuffer_count(BUF)==0)
#define FifoBuffer_overflow(BUF) (FifoBuffer_count(BUF)>=BUF->size)


/** FifoBuffer_Public_Macros
*	@}
*/

/** FifoBuffer_Public_Types
*	@addtogroup FifoBuffer_Public_Types
*	@{
*/


/** FifoBuffer_Public_Types
*	@}
*/


/* A pointer to an incomplete FifoBuffer type */


/** FifoBuffer_Public_Variables
*	@addtogroup FifoBuffer_Public_Variables
*	@{
*/

/** FifoBuffer_Public_Variables
*	@}
*/

/** FifoBuffer_Public_Functions
*	@addtogroup FifoBuffer_Public_Functions
*	@{
*/

/** FifoBuffer_Public_Functions
*	@}
*/

#endif

/** FifoBuffer
*	@}
*/

/** <Package>
*	@}
*/


/* ~~~~~~~~~~~~~~~~~~~~~~~~~ EOF ~~~~~~~~~~~~~~~~~~~~~~~~~ */
