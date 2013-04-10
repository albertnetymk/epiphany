#include <stdlib.h>
#include "actor_a.h"

void actor_a_init(actor_a *a)
{
    port_out_init(a->out);
}
