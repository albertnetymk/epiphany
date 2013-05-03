#include "communication.h"
#include "matmul.h"
#include "timers.h"

extern unsigned core_num();

#ifdef USE_DESTINATION_BUFFER
void port_out_init(port_out *p)
{
    p->dest_index = 0;
}

void port_in_init(port_in *p)
{
    p->read_index = p->write_index = 0;
    p->carrier = false;
    p->end = false;
}

void internal_epiphany_write(port_out *p, int v)
{
    int i;
    port_in *dest;
    for (i = 0; i < p->dest_index; ++i) {
        dest = (*p->dests)[i];
        if (dest->carrier) {
            timer_resume();
            while (dest->write_index == dest->read_index) ;
            timer_pause();
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

int internal_epiphany_read(port_in *p)
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

int epiphany_peek(port_in *p)
{
    if (!p->carrier) {
        while (p->read_index == p->write_index) ;
    }
    return p->array[p->read_index];
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
    timer_resume();
    while(e_dma_busy(b->dma->id)) ;
    timer_pause();
}

static void do_flush(fifo *b, uint size)
{
    switch (b->dma->status) {
        case DMA_PENDING:
            timer_resume();
            while(! b->total > 0) ;
            while(! b->twin->total == 0) ;
            while(! try_dma(b->dma)) ;
            timer_pause();
            dma_copy(b->dma->id, b->twin->array, b->array,
                    size, E_ALIGN_BYTE);
        case DMA_ING:
            wait_till_dma_over(b);
            b->dma->status = DMA_IDLE;
            b->twin->total = size/sizeof(int);
        case DMA_IDLE:
            ;
    }
}

static void try_flush(fifo *b)
{
    if (b->dma->status == DMA_PENDING) {
        if (b->total > 0 && b->twin->total == 0) {
            if (try_dma(b->dma)) {
                dma_copy(b->dma->id, b->twin->array, b->array,
                        sizeof(b->array), E_ALIGN_BYTE);
            }
        }
    } else if (b->dma->status == DMA_ING && ! e_dma_busy(b->dma->id)) {
        b->dma->status = DMA_IDLE;
        b->twin->total = sizeof(b->array)/sizeof(int);
    }
}

static void wait_till_ready_to_read(volatile fifo *b)
{
    timer_resume();
    while(! b->total > 0) ;
    timer_pause();
}

#ifdef USE_BOTH_BUFFER
void port_out_init(port_out *p)
{
    p->dest_index = 0;

    p->buffer->dma->status = DMA_IDLE;

    p->buffer->total = 0;
    p->buffer->size = sizeof(p->buffer->array)/sizeof(int);

    p->index = 0;
}

void port_in_init(port_in *p)
{
    p->buffer->total = 0;
    p->buffer->size = sizeof(p->buffer->array)/sizeof(int);

    p->end = false;
    p->index = 0;
}

void internal_epiphany_write(port_out *p, int v)
{
    if (p->index == 0 && p->buffer->total == p->buffer->size) {
        do_flush(p->buffer, sizeof(p->buffer->array));
        int i;
        for (i = 1; i < p->dest_index; ++i) {
            p->buffer->twin = (*p->dests)[i]->buffer;
            p->buffer->dma->status = DMA_PENDING;
            do_flush(p->buffer, sizeof(p->buffer->array));
        }
        p->buffer->total = 0;
    }
    p->buffer->array[p->index++] = v;
    p->buffer->total++;

    if (p->index == p->buffer->size) {
        p->index = 0;
        p->buffer->dma->status = DMA_PENDING;

        p->buffer->twin = (*p->dests)[0]->buffer;
        try_flush(p->buffer);
    }
}

int internal_epiphany_read(port_in *p)
{
    if (p->index == 0) {
        // TODO
        // signal sender to start the dma using interrupt
        // or using push, pull combined strategy, which need mutex
        wait_till_ready_to_read(p->buffer);
    }
    int result = p->buffer->array[p->index++];
    if (p->index == p->buffer->total) {
        p->index = p->buffer->total = 0;
    }
    return result;
}

int epiphany_peek(port_in *p)
{
    if (p->index == 0) {
        // TODO
        // signal sender to start the dma using interrupt
        // or using push, pull combined strategy, which need mutex
        wait_till_ready_to_read(p->buffer);
    }
    return p->buffer->array[p->index];
}

void flush(port_out *p)
{
    int size = p->buffer->total * sizeof(int);
    if (size > 0) {
        int i;
        do_flush(p->buffer, size);
        for (i=1; i<p->dest_index; ++i) {
            p->buffer->twin = (*p->dests)[i]->buffer;
            p->buffer->dma->status = DMA_PENDING;
            do_flush(p->buffer, size);
        }
        p->buffer->total = 0;
        p->index = 0;
    }
}

void end_port(port_out *p)
{
    int size = p->buffer->total * sizeof(int);
    if (size > 0) {
        int i;
        if ( p->buffer->total != p->buffer->size ) {
            p->buffer->twin = (*p->dests)[0]->buffer;
            p->buffer->dma->status = DMA_PENDING;
        }
        do_flush(p->buffer, size);
        (*p->dests)[0]->end = true;
        for (i=1; i<p->dest_index; ++i) {
            p->buffer->twin = (*p->dests)[i]->buffer;
            p->buffer->dma->status = DMA_PENDING;
            do_flush(p->buffer, size);
            (*p->dests)[i]->end = true;
        }
        p->index = p->buffer->total = 0;
    }
}

bool has_input(port_in *p, uint n)
{
    return p->buffer->total - p->index  >= n;
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
    p->buffers[current]->total = 0;
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
    p->buffers[current]->total = 0;
    p->current_dest_index[current] = 0;
}

static void try_distribute(port_out *p, uchar current)
{
    if (p->buffers[current]->dma->status != DMA_IDLE) {
        try_flush(p->buffers[current]);
    } else {
        if (++p->current_dest_index[current] < p->dest_index) {
            p->buffers[current]->twin =
                (*p->dests)[p->current_dest_index[current]]->buffers[current];
            p->buffers[current]->dma->status = DMA_PENDING;
            try_flush(p->buffers[current]);
        }
    }
}

void port_out_init(port_out *p)
{
    p->dest_index = 0;
    int i;
    fifo *b;
    for(i=0; i<sizeof(p->buffers)/sizeof(fifo *); ++i) {
        b = p->buffers[i];
        b->dma->status = DMA_IDLE;
        b->total = 0;
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
        b->total = 0;
        b->size = sizeof(b->array)/sizeof(int);
    }
    p->end = false;
    p->index = p->ping_pang = 0;
}

void internal_epiphany_write(port_out *p, int v)
{
    if (p->index == 0 &&
            p->buffers[p->ping_pang]->total
                == p->buffers[p->ping_pang]->size) {
        do_distribute(p, p->ping_pang,
                sizeof(p->buffers[p->ping_pang]->array));
    }

    if (p->buffers[1-p->ping_pang]->total == p->buffers[1-p->ping_pang]->size) {
        try_distribute(p, 1-p->ping_pang);
    }

    p->buffers[p->ping_pang]->array[p->index++] = v;
    p->buffers[p->ping_pang]->total++;

    if (p->index == p->buffers[p->ping_pang]->size) {
        p->index = 0;
        p->buffers[p->ping_pang]->dma->status = DMA_PENDING;
        p->current_dest_index[p->ping_pang] = 0;
        p->buffers[p->ping_pang]->twin = (*p->dests)[0]->buffers[p->ping_pang];
        try_flush(p->buffers[p->ping_pang]);
        p->ping_pang ^= 1;
    }
}

int internal_epiphany_read(port_in *p)
{
    if (p->index == 0) {
        wait_till_ready_to_read(p->buffers[p->ping_pang]);
    }
    int result = p->buffers[p->ping_pang]->array[p->index++];
    if (p->index == p->buffers[p->ping_pang]->total) {
        p->index = p->buffers[p->ping_pang]->total = 0;
        p->ping_pang ^= 1;
    }
    return result;
}

int epiphany_peek(port_in *p)
{
    if (p->index == 0) {
        wait_till_ready_to_read(p->buffers[p->ping_pang]);
    }
    return p->buffers[p->ping_pang]->array[p->index];
}

void flush(port_out *p)
{
    uchar current;
    if (p->buffers[p->ping_pang]->total == p->buffers[p->ping_pang]->size) {
        // double buffer full
        current = p->ping_pang;
        do_distribute(p, current, sizeof(p->buffers[current]->array));
        do_distribute(p, 1-current, sizeof(p->buffers[1-current]->array));
    } else {
        if (p->buffers[1-p->ping_pang]->total
                == p->buffers[1-p->ping_pang]->size){
            // only one buffer full
            current = 1-p->ping_pang;
            do_distribute(p, current, sizeof(p->buffers[current]->array));
        }
        if (p->buffers[p->ping_pang]->total > 0) {
            current = p->ping_pang;
            if ( p->buffers[current]->total != p->buffers[current]->size ) {
                p->buffers[current]->dma->status = DMA_PENDING;
                p->current_dest_index[current] = 0;
                p->buffers[current]->twin = (*p->dests)[0]->buffers[current];
            }
            do_distribute(p, current, p->buffers[current]->total*sizeof(int));
            p->index = p->buffers[current]->total = 0;
        }
    }
}

void end_port(port_out *p)
{
    uchar current;
    if (p->buffers[p->ping_pang]->total == p->buffers[p->ping_pang]->size) {
        // double buffer full
        current = p->ping_pang;
        do_distribute_end(p, current, sizeof(p->buffers[current]->array));
        do_distribute_end(p, 1-current, sizeof(p->buffers[1-current]->array));
    } else {
        if (p->buffers[1-p->ping_pang]->total
                == p->buffers[1-p->ping_pang]->size){
            // only one buffer full
            current = 1-p->ping_pang;
            do_distribute_end(p, current, sizeof(p->buffers[current]->array));
        }
        if (p->buffers[p->ping_pang]->total > 0) {
            current = p->ping_pang;
            if ( p->buffers[current]->total != p->buffers[current]->size ) {
                p->buffers[current]->dma->status = DMA_PENDING;
                p->current_dest_index[current] = 0;
                p->buffers[current]->twin = (*p->dests)[0]->buffers[current];
            }
            do_distribute_end(p, current,
                    p->buffers[current]->total*sizeof(int));
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
    uint total = 0;
    int i;
    for (i = 0; i < sizeof(p->buffers)/sizeof(fifo *); ++i) {
        total += p->buffers[i]->total;
        if (total - p->index >= n) {
            return true;
        }
    }
    return false;
}
#endif // USE_DOUBLE_BUFFER

#endif // USE_DESTINATION_BUFFER

bool might_has_input(port_in *p)
{
    return has_input(p, 1) || !p->end;
}

int read(port_in *p)
{
    timer_pause();
    return internal_epiphany_read(p);
}

void write(port_out *p, int v)
{
    internal_epiphany_write(p, v);
}

int peek(port_in *p)
{
    return epiphany_peek(p);
}

void epiphany_write(port_out *p, int v)
{
    // uint index = Mailbox.core.debug_index[core_num()];
    // Mailbox.core.debug_line[core_num()][index] = v;
    // Mailbox.core.debug_index[core_num()]++;
    internal_epiphany_write(p, v);
}

int epiphany_read(port_in *p)
{
    // int result = internal_epiphany_read(p)
    // uint index = Mailbox.core.debug_index[core_num()];
    // Mailbox.core.debug_line[core_num()][index] = result;
    // Mailbox.core.debug_index[core_num()]++;
    return internal_epiphany_read(p);
}
