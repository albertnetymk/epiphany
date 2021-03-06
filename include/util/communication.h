#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <stdbool.h>
#include "util/types.h"
#include "util/flags.h"

#ifdef USE_DESTINATION_BUFFER
typedef volatile struct port_in_struct {
    uchar read_index;
    uchar write_index;
    int array[10];
    bool carrier;
    bool end;
} port_in;

typedef volatile struct port_out_struct {
    port_in *(*dests)[];
    uchar dest_index;
} port_out;

#else // USE_DESTINATION_BUFFER

#include <e_dma.h>
enum dma_status {
    DMA_PENDING,
    DMA_ING,
    DMA_FINISHED,
    DMA_IDLE
};

typedef volatile struct dma_cfg_struct {
    enum dma_status status;
    e_dma_id_t id;
} dma_cfg;

typedef volatile struct fifo_struct {
    uchar total;
    int array[10];
    uchar size;
    volatile struct fifo_struct *twin;
    dma_cfg *dma;
} fifo;

#ifdef USE_BOTH_BUFFER
typedef struct port_in_struct {
    uchar index;
    fifo *buffer;
    bool end;
} port_in;

typedef struct port_out_struct {
    uchar index;
    fifo *buffer;
    port_in *(*dests)[];
    uchar dest_index;
} port_out;
#endif // USE_BOTH_BUFFER

#ifdef USE_DOUBLE_BUFFER
typedef struct port_in_struct {
    uchar index;
    uchar ping_pang;
    fifo *buffers[2];
    bool end;
} port_in;

typedef struct port_out_struct {
    uchar index;
    uchar ping_pang;
    fifo *buffers[2];
    port_in *(*dests)[];
    uchar dest_index;
    uchar current_dest_index[2];
} port_out;
#endif // USE_DOUBLE_BUFFER

#ifdef USE_MULTIPLE_BUFFER

typedef struct port_in_struct {
    uchar index;
    uchar buffer_index;
    fifo *buffers[BUFFER_NUMBER];
    bool end;
} port_in;

typedef struct port_out_struct {
    uchar index;
    uchar buffer_index;
    fifo *buffers[BUFFER_NUMBER];
    port_in *(*dests)[];
    uchar dest_index;
    uchar current_dest_index[BUFFER_NUMBER];
} port_out;
#endif // USE_MULTIPLE_BUFFER

#endif // USE_DESTINATION_BUFFER

void port_out_init(port_out *p);
void port_in_init(port_in *p);
void epiphany_write(port_out *p, int v);
int epiphany_read(port_in *p);
int epiphany_peek(port_in *p);
void flush(port_out *p);
void end_port(port_out *p);
void connect(port_out *out, port_in *in);
bool has_input(port_in *p, uint n);
bool might_has_input(port_in *p);
int ReadToken(port_in *p, uint n);
int ConsumeToken(port_in *p, uint n);
void SendToken(port_out *p, int v, uint n);
bool TestInputPort(port_in *p, uint n);
void dead();

#endif /* end of include guard: _COMMUNICATION_H */
