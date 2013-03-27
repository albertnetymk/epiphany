#include <stdlib.h>
#include "actor_b.h"
actor_b *actor_b_new()
{
    actor_b *instance_b = malloc(sizeof(actor_b));
    port_in *in = malloc(sizeof(port_in));
    instance_b->in = in;
    return instance_b;
}

