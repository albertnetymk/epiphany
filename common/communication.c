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
