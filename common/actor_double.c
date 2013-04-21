#include <stdlib.h>
#include "actor_double.h"

static void run(actor_double *self)
{
    int i, v;
    for (i = 0; i < 10; ++i) {
        v = epiphany_read(self->in);
        epiphany_write(self->out, v*2);
    }
}

void actor_double_init(actor_double *a)
{
    port_in_init(a->in);
    port_out_init(a->out);
    a->run = &run;
}
