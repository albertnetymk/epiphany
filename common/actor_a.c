#include <stdlib.h>
#include "actor_a.h"

actor_a *actor_a_new()
{
    actor_a *instance_a = malloc(sizeof(actor_a));
    port_out *out = malloc(sizeof(port_out));
    instance_a->out = out;
    return instance_a;
}

