#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include <stdbool.h>
#include "types.h"
#include "flags.h"

#ifdef USE_DESTINATION_BUFFER
typedef volatile struct port_in_struct {
    uchar read_index;
    uchar write_index;
    int array[10];
    bool carrier;
} port_in;

typedef struct port_out_struct {
    port_in *dest;
} port_out;

void port_out_init(port_out *p);
void port_in_init(port_in *p);
void epiphany_write(port_out *p, int v);
int epiphany_read(port_in *p);
void flush(port_out *p);
void connect(port_out *out, port_in *in);
#endif

#ifdef USE_BOTH_BUFFER
#include <e_dma.h>
enum dma_status {
    DMA_PENDING,
    DMA_ING,
    DMA_IDLE,
};

typedef struct dma_cfg_struct {
    enum dma_status status;
    e_dma_id_t id;
} dma_cfg;

typedef volatile struct fifo_struct {
    bool ready_to_dma;
    int array[10];
    uchar size;
    volatile struct fifo_struct *twin;
    dma_cfg *dma;
} fifo;

typedef struct port_in_struct {
    uchar index;
    fifo *buffer;
} port_in;

typedef struct port_out_struct {
    uchar index;
    fifo *buffer;
} port_out;
void port_out_init(port_out *p);
void port_in_init(port_in *p);
void epiphany_write(port_out *p, int v);
int epiphany_read(port_in *p);
void flush(port_out *p);
void connect(port_out *out, port_in *in);
#endif

#endif /* end of include guard: _COMMUNICATION_H */
