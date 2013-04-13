#include <stdlib.h>
#include "communication.h"

#ifdef USE_DESTINATION_BUFFER
void port_out_init(port_out *p)
{
}

void port_in_init(port_in *p)
{
    p->read_index = p->write_index = 0;
    p->carrier = false;
}

void epiphany_write(port_out *p, int v)
{
    volatile port_in *dest = p->dest;
    if (dest->carrier) {
        while (dest->write_index == dest->read_index) ;
    }
    dest->array[dest->write_index] = v;
    if (dest->write_index == sizeof(dest->array)/sizeof(int) - 1 ) {
        dest->carrier = true;
        dest->write_index = 0;
    } else {
        dest->write_index++;
    }
}

int epiphany_read(port_in *p)
{
    if (!p->carrier) {
        while (p->read_index == p->write_index) ;
    }
    int result = p->array[p->read_index];
    if (p->read_index == sizeof(p->array)/sizeof(int) - 1) {
        p->carrier = false;
        p->read_index = 0;
    } else {
        p->read_index++;
    }
    return result;
}

void flush(port_out *p)
{
}

void connect(port_out *out, volatile port_in *in)
{
    out->dest = in;
}
#endif

#ifdef USE_BOTH_BUFFER
void port_out_init(port_out *p)
{
    p->buffer->dma->status = DMA_IDLE;

    p->buffer->ready_to_dma = false;
    p->buffer->size = sizeof(p->buffer->array)/sizeof(int);

    p->index = 0;
}

void port_in_init(port_in *p)
{
    p->buffer->ready_to_dma = true;
    p->buffer->size = sizeof(p->buffer->array)/sizeof(int);

    p->index = 0;
}

static dma_cfg *dma_pool[2];
static bool try_dma(dma_cfg *dma)
{
    if (! e_dma_busy(E_DMA_0) ) {
        if (dma_pool[0] != NULL) {
            dma_pool[0]->status = DMA_IDLE;
        }
        dma->status = DMA_ING;
        dma->id = E_DMA_0;
        dma_pool[0] = dma;
        return true;
    }
    if (! e_dma_busy(E_DMA_1) ) {
        if (dma_pool[1] != NULL) {
            dma_pool[1]->status = DMA_IDLE;
        }
        dma->status = DMA_ING;
        dma->id = E_DMA_1;
        dma_pool[1] = dma;
        return true;
    }
    return false;
}

inline static void dma_copy(e_dma_id_t chan, volatile void *dst, volatile void *src,
        size_t bytes, e_dma_align_t align)
{
    e_dma_copy(chan, (void *)dst, (void *)src, bytes, align);
}

static void wait_till_dma_over(volatile fifo *b)
{
    while(e_dma_busy(b->dma->id)) ;
}

static void do_flush(volatile fifo *b, uint size)
{
    while(! b->ready_to_dma) ;
    while(! b->twin->ready_to_dma) ;
    switch (b->dma->status) {
        case DMA_PENDING:
            while(! try_dma(b->dma)) ;
            dma_copy(b->dma->id, b->twin->array, b->array,
                    size, E_ALIGN_BYTE);
        case DMA_ING:
            wait_till_dma_over(b);
            break;
        case DMA_IDLE:
            b->ready_to_dma = b->twin->ready_to_dma = false;
            break;
    }
}

static void try_flush(volatile fifo *b)
{
    if (b->dma->status == DMA_PENDING) {
        if (b->ready_to_dma && b->twin->ready_to_dma) {
            if (try_dma(b->dma)) {
                dma_copy(b->dma->id, b->twin->array, b->array,
                        b->size, E_ALIGN_BYTE);
            }
        }
    }
}

void epiphany_write(port_out *p, int v)
{
    if (p->index == 0 && p->buffer->ready_to_dma) {
        do_flush(p->buffer, p->buffer->size);
    }
    p->buffer->array[p->index++] = v;
    if (p->index == p->buffer->size) {
        p->index = 0;
        p->buffer->ready_to_dma = true;
        p->buffer->dma->status = DMA_PENDING;
        try_flush(p->buffer);
    }
}

static void wait_till_ready_to_read(volatile fifo *b)
{
    while(b->ready_to_dma) ;
}

int epiphany_read(port_in *p)
{
    if (p->index == 0) {
        // TODO
        // signal sender to start the dma using interrupt
        // or using push, pull combined strategy, which need mutex
        wait_till_ready_to_read(p->buffer);
    }
    int result = p->buffer->array[p->index++];
    if (p->index == p->buffer->size) {
        p->index = 0;
        p->buffer->ready_to_dma = true;
    }
    return result;
}

void flush(port_out *p)
{
    if (p->buffer->ready_to_dma) {
        do_flush(p->buffer, p->buffer->size);
    } else if (p->index > 0) {
        p->buffer->ready_to_dma = true;
        do_flush(p->buffer, p->index);
        p->index = 0;
    }
}

void connect(port_out *out, port_in *in)
{
    in->buffer->dma = out->buffer->dma;
    out->buffer->twin = in->buffer;
    in->buffer->twin = out->buffer;
}

#endif
