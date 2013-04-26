#include <stdlib.h>
#include "actor_add.h"

static void run(actor_add *self)
{
    // int i, l, r;
    // for (i = 0; i < 10; ++i) {
    //     l = epiphany_read(self->in1);
    //     r = epiphany_read(self->in2);
    //     epiphany_write(self->out, l+r);
    // }
    int l, r;
    if (has_input(self->in1, 1)) {
        if (has_input(self->in2, 1)) {
            l = epiphany_read(self->in1);
            r = epiphany_read(self->in2);
            epiphany_write(self->out, l+r);
        }
    }
}

static bool not_finished(actor_add *self)
{
    // iterate through all in ports
    return might_has_input(self->in1)
        || might_has_input(self->in2);
}

static void end(actor_add *self)
{
    // iterate through all out ports
    end_port(self->out);
}

void actor_add_init(actor_add *a)
{
    port_in_init(a->in1);
    port_in_init(a->in2);
    port_out_init(a->out);
    a->run = &run;
    a->not_finished = &not_finished;
    a->end = &end;
}
