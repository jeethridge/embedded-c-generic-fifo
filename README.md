Simple generic FIFO buffer implementation for embedded C applications
===================================================

This was developed out of a need for a simple
and portable generic fifo to be used across a variety of
small embedded C applications. The calling code is largely
responsible for checking for and handling overflows or other
error conditions.

The software was written using the following sources for reference:

1. https://github.com/pthrasher/c-generic-ring-buffer
2. http://en.wikipedia.org/wiki/Circular_buffer#Read_.2F_Write_Counts
3. http://c.learncodethehardway.org/book/ex44.htm


Example usage:

```c
#include <stdint.h>
#include <stdio.h>
#include <assert.h>

/* typedef some structs that represent different types
 of data to be passed around */

typedef struct type_a_t {
uint32_t x;
uint16_t y;
}type_a_t;

typedef struct type_b_t{
    uint32_t m;
    uint16_t n;
}type_b_t;

/*
*Allocate some memory to use as the FifoBuffer storage
*for each of the FifoBuffer types to be created
*/
#define NUMEL 0xFFFE
static type_a_t bufmem_a[NUMEL];
static type_b_t bufmem_b[NUMEL];

/*
* Call the macro which creates a
* type-specific FifoBuffer definition
*/
FifoBuffer_typedef(type_a_t,TypeAfifo);
FifoBuffer_typedef(type_b_t,TypeBfifo);


/*Demonstrate the use of Fifos for different types in
* the same scope
* */
void FifoBuffer_multiple_types_test(void)
{
    TypeAfifo my_fifo_a;
    TypeAfifo* my_fifo_p_a=&my_fifo_a;

    TypeBfifo my_fifo_b;
    TypeBfifo* my_fifo_p_b=&my_fifo_b;

    type_a_t a;
    type_b_t b;
    uint32_t i;

    FifoBuffer_init(my_fifo_p_a,NUMEL,type_a_t, bufmem_a);
    FifoBuffer_init(my_fifo_p_b,NUMEL,type_b_t, bufmem_b);

    for(i=0;i<NUMEL;i++) {
        a.x = i;
        a.y = i;
        b.m = i;
        b.n = i;
        FifoBuffer_write(my_fifo_p_a, a);
        FifoBuffer_write(my_fifo_p_b, b);
    }

    assert(FifoBuffer_is_full(my_fifo_p_a));
    assert(FifoBuffer_is_full(my_fifo_p_b));

    for(i=0;i<NUMEL;i++) {
        FifoBuffer_read(my_fifo_p_a, a);
        assert((a.x==i)&&(a.y==i));
        FifoBuffer_read(my_fifo_p_b, b);
        assert((b.m==i)&&(b.n==i));
    }
}
