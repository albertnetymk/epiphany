#include <stdlib.h>
#include "communication.h"

#ifdef USE_DESTINATION_BUFFER
void port_out_init(port_out *p)
{
    p->index = 0;
    p->dest_index = 0;
}

void port_in_init(port_in *p)
{
    p->read_index = p->write_index = 0;
    p->carrier = false;
    p->end = false;
}

void epiphany_write(port_out *p, int v)
{
    int i;
    port_in *dest;
    for (i = 0; i < p->dest_index; ++i) {
        dest = (*p->dests)[i];
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

void end_port(port_out *p)
{
    int i;
    for (i = 0; i < p->dest_index; ++i) {
        (*p->dests)[i]->end = true;
    }
}

void connect(port_out *out, volatile port_in *in)
{
    (*out->dests)[out->dest_index++] = in;
}

bool has_input(port_in *p, uint n)
{
    uchar write_index;
    if (p->carrier) {
        write_index = p->write_index + sizeof(p->array)/sizeof(int);
    } else {
        write_index = p->write_index;
    }
    return write_index - p->read_index >= n;
}

#else // USE_DESTINATION_BUFFER
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

static void wait_till_dma_over(fifo *b)
{
    while(e_dma_busy(b->dma->id)) ;
}

static void do_flush(fifo *b, uint size)
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
            b->dma->status = DMA_IDLE;
        case DMA_IDLE:
            b->twin->ready_to_dma = false;
            break;
    }
}

static void try_flush(fifo *b)
{
    if (b->dma->status == DMA_PENDING) {
        if (b->ready_to_dma && b->twin->ready_to_dma) {
            if (try_dma(b->dma)) {
                dma_copy(b->dma->id, b->twin->array, b->array,
                        sizeof(b->array), E_ALIGN_BYTE);
            }
        }
    } else if (b->dma->status == DMA_ING && ! e_dma_busy(b->dma->id)) {
        b->dma->status = DMA_IDLE;
    }
}

static void wait_till_ready_to_read(volatile fifo *b)
{
    while(b->ready_to_dma) ;
}

#ifdef USE_BOTH_BUFFER
void port_out_init(port_out *p)
{
    p->dest_index = 0;

    p->buffer->dma->status = DMA_IDLE;

    p->buffer->ready_to_dma = false;
    p->buffer->size = sizeof(p->buffer->array)/sizeof(int);

    p->index = 0;
}

void port_in_init(port_in *p)
{
    p->buffer->ready_to_dma = true;
    p->buffer->size = sizeof(p->buffer->array)/sizeof(int);

    p->end = false;
    p->index = 0;
}

void epiphany_write(port_out *p, int v)
{
    if (p->index == 0 && p->buffer->ready_to_dma) {
        do_flush(p->buffer, sizeof(p->buffer->array));
        int i;
        for (i = 1; i < p->dest_index; ++i) {
            p->buffer->twin = (*p->dests)[i]->buffer;
            p->buffer->dma->status = DMA_PENDING;
            do_flush(p->buffer, sizeof(p->buffer->array));
        }
        p->buffer->ready_to_dma = false;
    }
    p->buffer->array[p->index++] = v;
    if (p->index == p->buffer->size) {
        p->index = 0;
        p->buffer->ready_to_dma = true;
        p->buffer->dma->status = DMA_PENDING;

        p->buffer->twin = (*p->dests)[0]->buffer;
        try_flush(p->buffer);
    }
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
    int size = 0;
    if (p->buffer->ready_to_dma) {
        size = sizeof(p->buffer->array);
    } else if (p->index > 0) {
        size = p->index*sizeof(int);
        p->buffer->ready_to_dma = true;
    }
    if (size > 0) {
        int i;
        do_flush(p->buffer, size);
        for (i=1; i<p->dest_index; ++i) {
            p->buffer->twin = (*p->dests)[i]->buffer;
            p->buffer->dma->status = DMA_PENDING;
            do_flush(p->buffer, size);
        }
        p->buffer->ready_to_dma = false;
        p->index = 0;
    }
}

void end_port(port_out *p)
{
    int size = 0;
    if (p->buffer->ready_to_dma) {
        size = sizeof(p->buffer->array);
    } else if (p->index > 0) {
        size = p->index*sizeof(int);
        p->buffer->ready_to_dma = true;
    }
    if (size > 0) {
        int i;
        do_flush(p->buffer, size);
        (*p->dests)[0]->end = true;
        for (i=1; i<p->dest_index; ++i) {
            p->buffer->twin = (*p->dests)[i]->buffer;
            p->buffer->dma->status = DMA_PENDING;
            do_flush(p->buffer, size);
            (*p->dests)[i]->end = true;
        }
        p->buffer->ready_to_dma = false;
        p->index = 0;
    }
}

bool has_input(port_in *p, uint n)
{
    if (p->buffer->ready_to_dma) {
        return false;
    }
    return p->index + 1 >= n;
}

void connect(port_out *out, port_in *in)
{
    (*out->dests)[out->dest_index++] = in;
}
#endif // USE_BOTH_BUFFER

#ifdef USE_DOUBLE_BUFFER
static void do_distribute_end(port_out *p, uchar current, uint size)
{
    do_flush(p->buffers[current], size);
    uchar current_dest = p->current_dest_index[current];
    (*p->dests)[current_dest]->end = true;
    for (current_dest++; current_dest < p->dest_index; ++current_dest) {
        p->buffers[current]->twin =
            (*p->dests)[current_dest]->buffers[current];
        p->buffers[current]->dma->status = DMA_PENDING;
        do_flush(p->buffers[current], size);
        (*p->dests)[current_dest]->end = true;
    }
    p->buffers[current]->ready_to_dma = false;
    p->current_dest_index[current] = 0;
}
static void do_distribute(port_out *p, uchar current, uint size)
{
    do_flush(p->buffers[current], size);
    uchar current_dest = p->current_dest_index[current];
    for (current_dest++; current_dest < p->dest_index; ++current_dest) {
        p->buffers[current]->twin =
            (*p->dests)[current_dest]->buffers[current];
        p->buffers[current]->dma->status = DMA_PENDING;
        do_flush(p->buffers[current], size);
    }
    p->buffers[current]->ready_to_dma = false;
    p->current_dest_index[current] = 0;
}

void port_out_init(port_out *p)
{
    p->dest_index = 0;
    int i;
    fifo *b;
    for(i=0; i<sizeof(p->buffers)/sizeof(fifo *); ++i) {
        b = p->buffers[i];
        b->dma->status = DMA_IDLE;
        b->ready_to_dma = false;
        b->size = sizeof(b->array)/sizeof(int);
    }
    p->index = p->ping_pang = 0;
}

void port_in_init(port_in *p)
{
    int i;
    fifo *b;
    for(i=0; i<sizeof(p->buffers)/sizeof(fifo *); ++i) {
        b = p->buffers[i];
        b->ready_to_dma = true;
        b->size = sizeof(b->array)/sizeof(int);
    }
    p->end = false;
    p->index = p->ping_pang = 0;
}

void epiphany_write(port_out *p, int v)
{
    if (p->index == 0 && p->buffers[p->ping_pang]->ready_to_dma) {
        do_distribute(p, p->ping_pang,
                sizeof(p->buffers[p->ping_pang]->array));
    }
    {
        uchar other = 1-p->ping_pang;
        if (p->buffers[other]->ready_to_dma) {
            if (p->buffers[other]->dma->status != DMA_IDLE) {
                try_flush(p->buffers[other]);
            } else if (p->current_dest_index[other] < p->dest_index) {
                p->buffers[other]->twin =
                  (*p->dests)[++p->current_dest_index[other]]->buffers[other];
                p->buffers[other]->dma->status = DMA_PENDING;
                try_flush(p->buffers[other]);
            }
        }
    }
    p->buffers[p->ping_pang]->array[p->index++] = v;
    if (p->index == p->buffers[p->ping_pang]->size) {
        p->index = 0;
        p->buffers[p->ping_pang]->ready_to_dma = true;
        p->buffers[p->ping_pang]->dma->status = DMA_PENDING;
        p->current_dest_index[p->ping_pang] = 0;
        p->buffers[p->ping_pang]->twin = (*p->dests)[0]->buffers[p->ping_pang];
        try_flush(p->buffers[p->ping_pang]);
        p->ping_pang ^= 1;
    }
}

int epiphany_read(port_in *p)
{
    if (p->index == 0) {
        wait_till_ready_to_read(p->buffers[p->ping_pang]);
    }
    int result = p->buffers[p->ping_pang]->array[p->index++];
    if (p->index == p->buffers[p->ping_pang]->size) {
        p->index = 0;
        p->buffers[p->ping_pang]->ready_to_dma = true;
        p->ping_pang ^= 1;
    }
    return result;
}

void flush(port_out *p)
{
    uchar current;
    if (p->buffers[p->ping_pang]->ready_to_dma) {
        // double buffer full
        current = p->ping_pang;
        do_distribute(p, current, sizeof(p->buffers[current]->array));
        do_distribute(p, 1-current, sizeof(p->buffers[1-current]->array));
    } else {
        if (p->buffers[1-p->ping_pang]->ready_to_dma){
            // only one buffer full
            current = 1-p->ping_pang;
            do_distribute(p, current, sizeof(p->buffers[current]->array));
        }
        if (p->index > 0) {
            current = p->ping_pang;
            p->buffers[current]->ready_to_dma =true;
            do_distribute(p, current, p->index*sizeof(int));
            p->index = 0;
        }
    }
}

void end_port(port_out *p)
{
    uchar current;
    if (p->buffers[p->ping_pang]->ready_to_dma) {
        // double buffer full
        current = p->ping_pang;
        do_distribute_end(p, current, sizeof(p->buffers[current]->array));
        do_distribute_end(p, 1-current, sizeof(p->buffers[1-current]->array));
    } else {
        if (p->buffers[1-p->ping_pang]->ready_to_dma){
            // only one buffer full
            current = 1-p->ping_pang;
            do_distribute_end(p, current, sizeof(p->buffers[current]->array));
        }
        if (p->index > 0) {
            current = p->ping_pang;
            p->buffers[current]->ready_to_dma =true;
            do_distribute_end(p, current, p->index*sizeof(int));
            p->index = 0;
        }
    }
}

void connect(port_out *out, port_in *in)
{
    (*out->dests)[out->dest_index++] = in;
}

bool has_input(port_in *p, uint n)
{
    if (p->buffers[p->ping_pang]->ready_to_dma) {
        return false;
    }
    // TODO some optimization maybe, for p->index + 1 >= n is true
    if (p->buffers[1-p->ping_pang]->ready_to_dma) {
        return p->index + 1 >= n;
    }
    return p->buffers[1-p->ping_pang]->size+p->index+1 >= n;
}
#endif // USE_DOUBLE_BUFFER

#endif // USE_DESTINATION_BUFFER

bool might_has_input(port_in *p)
{
    return has_input(p, 1) || !p->end;
}
