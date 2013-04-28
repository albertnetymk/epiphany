#include <stdlib.h>
#include "actor_distributor.h"

static void run(actor_distributor *self)
{
    int v;
    if (has_input(self->in, 1)) {
        v = epiphany_read(self->in);
        epiphany_write(self->out, v);
    }
}

static bool not_finished(actor_distributor *self)
{
    // iterate through all in ports
    return might_has_input(self->in);
}

static void end(actor_distributor *a)
{
    end_port(a->out);
}

void actor_distributor_init(actor_distributor *a)
{
    port_in_init(a->in);
    port_out_init(a->out);
    a->run = &run;
    a->not_finished = &not_finished;
    a->end = &end;
}
