#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>
#include "FifoBuffer.h"

/**
* Unit tests for a simple, generic Fifo buffer implementation
*/


typedef struct type_a_t {
uint32_t x;
uint16_t y;
}type_a_t;

typedef struct type_b_t{
    uint32_t m;
    uint16_t n;
}type_b_t;

#define NUMEL 0xFFFE
static type_a_t bufmem_a[NUMEL];
static type_b_t bufmem_b[NUMEL];
FifoBuffer_typedef(type_a_t,TypeAfifo);
FifoBuffer_typedef(type_b_t,TypeBfifo);

/* Check that the buffer can be written and read
* appropriately */
void FifoBuffer_writeread_test(void)
{
    TypeAfifo my_fifo;
    TypeAfifo* my_fifo_p=&my_fifo;
    type_a_t s;
    uint32_t i;


    FifoBuffer_init(my_fifo_p,NUMEL,type_a_t, bufmem_a);
    s.x=100;
    s.y=101;
    FifoBuffer_write(my_fifo_p, s);

    s.x=102;
    s.y=103;
    FifoBuffer_write(my_fifo_p, s);

    FifoBuffer_read(my_fifo_p, s);
    assert((s.x==100)&&(s.y==101));

    FifoBuffer_read(my_fifo_p, s);
    assert((s.x==102)&&(s.y==103));

    for(i=0;i<NUMEL;i++) {
        s.x = i;
        s.y = i;
        FifoBuffer_write(my_fifo_p, s);
    }

    assert(FifoBuffer_is_full(my_fifo_p));

    for(i=0;i<NUMEL;i++) {
        FifoBuffer_read(my_fifo_p, s);
        assert((s.x==i)&&(s.y==i));
    }
}

/* Ensure we can detect buffer overflow
* appropriately */
void FifoBuffer_overflow_test(void)
{
    TypeAfifo my_fifo;
    TypeAfifo* my_fifo_p=&my_fifo;
    type_a_t s;
    uint32_t i;
    int overflow_flag=0;
    FifoBuffer_init(my_fifo_p,NUMEL,type_a_t, bufmem_a);

    for(i=0;i<NUMEL+1;i++) {
        s.x = i;
        s.y = i;

        if(!FifoBuffer_is_full(my_fifo_p)){
            FifoBuffer_write(my_fifo_p, s);
        }
        else{
            overflow_flag=1;
            assert(i==NUMEL);
            FifoBuffer_write(my_fifo_p, s);
        }

    }
    assert(overflow_flag==1);
    assert(FifoBuffer_overflow(my_fifo_p)==1);

}

/*Demonstrate the use of Fifos for multiple types in
* the same scope/context
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


/*
* Fill to capacity and drain 5 times
*/
void FifoBuffer_fill_and_drain_test(void)
{
    TypeAfifo my_fifo_a;
    TypeAfifo* my_fifo_p_a=&my_fifo_a;
    type_a_t a;
    uint32_t i;
    uint32_t j;
    FifoBuffer_init(my_fifo_p_a,NUMEL,type_a_t, bufmem_a);

    for(j=0;j<5;j++) {
        for (i = 0; i < NUMEL; i++) {
            a.x = i;
            a.y = i;
            FifoBuffer_write(my_fifo_p_a, a);
        }

        for (i = 0; i < NUMEL; i++) {
            FifoBuffer_read(my_fifo_p_a, a);
            assert((a.x == i) && (a.y == i));

        }

        //ensure the lenght is zero when done
        assert(FifoBuffer_count(my_fifo_p_a)==0);
    }

}

/*General stress/performance test for the fifo
* on bursty data. Feed the fifobuffer a random set of samples between
* 1 and NUMEL times. Then read all the values out using the isempty check. Do this 4095 times.
* */
void FifoBuffer_stress_test(void)
{
    TypeAfifo my_fifo_a;
    TypeAfifo* my_fifo_p_a=&my_fifo_a;
    type_a_t a;
    uint32_t i;
    uint32_t j;
    int r;
    srand(time(NULL));

    FifoBuffer_init(my_fifo_p_a,NUMEL,type_a_t, bufmem_a);

    for(j=0;j<0xFFF;j++) {
        r = rand()%NUMEL+1;//generate random sample count between 1 and NUMEL
        for (i = 0; i < r; i++) {
            a.x = i;
            a.y = i;
            FifoBuffer_write(my_fifo_p_a, a);
        }

        i=0;
        while(!FifoBuffer_is_empty(my_fifo_p_a)) {
            FifoBuffer_read(my_fifo_p_a, a);
            assert((a.x == i) && (a.y == i));
            i++;

        }
    }
}


/*
 *Test the peek functionality of the FifoBuffer
 * */
void FifoBuffer_peek_test(void)
{
    TypeAfifo my_fifo_a;
    TypeAfifo* my_fifo_p_a=&my_fifo_a;
    type_a_t a;
    uint32_t i;

    FifoBuffer_init(my_fifo_p_a,NUMEL,type_a_t, bufmem_a);

    for (i = 0; i < NUMEL; i++) {
        a.x = i;
        a.y = i;
        FifoBuffer_write(my_fifo_p_a, a);
    }

    for (i = 0; i < NUMEL; i++) {
        FifoBuffer_peek(my_fifo_p_a, a,i);
        assert((a.x == i) && (a.y == i));
    }

    assert(FifoBuffer_count(my_fifo_p_a)==NUMEL);

}


/*
 *Test the flush functionality of the FifoBuffer
 * */
void FifoBuffer_flush_test(void)
{
    TypeAfifo my_fifo_a;
    TypeAfifo* my_fifo_p_a=&my_fifo_a;
    type_a_t a;
    uint32_t i;

    FifoBuffer_init(my_fifo_p_a,NUMEL,type_a_t, bufmem_a);

    for (i = 0; i < NUMEL; i++) {
        a.x = i;
        a.y = i;
        FifoBuffer_write(my_fifo_p_a, a);
    }

    FifoBuffer_flush(my_fifo_p_a);

    assert(FifoBuffer_count(my_fifo_p_a)==0);

    a.x = 17;
    a.y = 17;
    FifoBuffer_write(my_fifo_p_a, a);

    assert(FifoBuffer_count(my_fifo_p_a)==1);
    a.x=0;
    a.y=0;
    FifoBuffer_read(my_fifo_p_a, a);
    assert((a.x == 17) && (a.y == 17));
}

int main() {

    FifoBuffer_writeread_test();
    FifoBuffer_overflow_test();
    FifoBuffer_multiple_types_test();
    FifoBuffer_fill_and_drain_test();
    FifoBuffer_stress_test();
    FifoBuffer_peek_test();
    FifoBuffer_flush_test();

    return 0;
}