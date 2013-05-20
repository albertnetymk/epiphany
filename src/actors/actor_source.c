#include <stdlib.h>
#include "actors/actor_source.h"

static void end(actor_source *a)
{
    end_port(a->out);
}

void actor_source_init(actor_source *a)
{
    port_out_init(a->out);
    a->end = &end;
}
