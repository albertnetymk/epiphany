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
    if (dest->carrier) {
        while (dest->write_index == dest->read_index) ;
    }
    dest->array[dest->write_index] = v;
    if (dest->write_index ==
            (dest->write_index+1) % (sizeof(dest->array)/sizeof(int)) - 1 ) {
        dest->carrier = true;
        dest->write_index = 0;
    }
}

int epiphany_read(volatile port_in *p)
{
    if (!p->carrier) {
        while (p->read_index == p->write_index) ;
    }
    int result = p->array[p->read_index];
    if (p->read_index ==
            (p->read_index+1) % (sizeof(p->array)/sizeof(int)) - 1) {
        p->carrier = false;
        p->read_index = 0;
    }
    return result;
}

void end(volatile port_out *p)
{
}

void connect(volatile port_out *out, volatile port_in *in)
{
    out->dest = in;
}
