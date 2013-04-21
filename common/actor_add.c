#include <stdlib.h>
#include "actor_add.h"

static void run(actor_add *self)
{
    int i, l, r;
    for (i = 0; i < 10; ++i) {
        l = epiphany_read(self->in1);
        r = epiphany_read(self->in2);
        epiphany_write(self->out, l+r);
    }
}

void actor_add_init(actor_add *a)
{
    port_in_init(a->in1);
    port_in_init(a->in2);
    port_out_init(a->out);
    a->run = &run;
}
