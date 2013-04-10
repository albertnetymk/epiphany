#include <stdlib.h>
#include "communication.h"

void port_out_init(volatile port_out *p)
{
}

void port_in_init(volatile port_in *p)
{
    p->read_index = p->write_index = 0;
}

void epiphany_write(volatile port_out *p, int v)
{
    volatile port_in *dest = p->dest;
    uchar new_write_index = (dest->write_index+1) % sizeof(dest->array)/sizeof(int);
    while (new_write_index == dest->read_index);
    dest->array[dest->write_index] = v;
    dest->write_index = new_write_index;
}

int epiphany_read(volatile port_in *p)
{
    uchar new_read_index = (p->read_index+1) % sizeof(p->array)/sizeof(int);
    while (new_read_index == p->write_index);
    int result = p->array[p->read_index];
    p->read_index = new_read_index;
    return result;
}

void end(volatile port_out *p)
{
}

void connect(volatile port_out *out, volatile port_in *in)
{
    out->dest = in;
}
