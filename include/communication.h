#ifndef _COMMUNICATION_H
#define _COMMUNICATION_H

#include "types.h"

typedef struct port_in_struct {
} port_in;

typedef struct port_out_struct {
} port_out;

void port_out_init(volatile port_out *p);
void port_in_init(volatile port_in *p);
void epiphany_write(volatile port_out *p, int v);
int epiphany_read(volatile port_in *p);
void end(volatile port_out *p);
void connect(volatile port_out *out, volatile port_in *in);

#endif /* end of include guard: _COMMUNICATION_H */
