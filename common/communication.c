#include <stdlib.h>
#include "types.h"
#include "communication.h"

struct port_in_struct {
};

struct port_out_struct {
};

port_out *port_out_new()
{
    port_out *port = NULL;
    return port;
}

port_in *port_in_new()
{
    port_in *port = NULL;
    return port;
}

void epiphany_write(port_out *p, int v)
{
}

int epiphany_read(port_in *p)
{
    return 0;
}

void connect(port_out *out, port_in *in)
{
}
