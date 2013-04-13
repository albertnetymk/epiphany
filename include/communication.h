#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include <stdbool.h>
#include "types.h"

typedef struct port_in_struct {
    uchar read_index;
    uchar write_index;
    int array[10];
    bool carrier;
} port_in;

typedef struct port_out_struct {
    volatile port_in *dest;
} port_out;

void port_out_init(volatile port_out *p);
void port_in_init(volatile port_in *p);
void epiphany_write(volatile port_out *p, int v);
int epiphany_read(volatile port_in *p);
void flush(volatile port_out *p);
void connect(volatile port_out *out, volatile port_in *in);

#endif /* end of include guard: _COMMUNICATION_H */
