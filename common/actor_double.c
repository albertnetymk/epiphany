#include <stdlib.h>
#include "actor_double.h"

static void run(actor_double *self)
{
    int i, v;
    for (i = 0; i < 10; ++i) {
        v = epiphany_read(self->in);
        epiphany_write(self->out, v*2);
    }
    // int v = epiphany_read(self->in);
    // epiphany_write(self->out, v*2);
}

static bool not_finished(actor_double *self)
{
    // iterate through all in ports
    return might_has_input(self->in);
}

static void end(actor_double *self)
{
    // iterate through all out ports
    end_port(self->out);
}

void actor_double_init(actor_double *a)
{
    port_in_init(a->in);
    port_out_init(a->out);
    a->run = &run;
    a->not_finished = &not_finished;
    a->end = &end;
}
