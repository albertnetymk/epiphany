#include <stdlib.h>
#include "actor_b.h"
void actor_b_init(actor_b *a)
{
    port_in_init(a->in);
}
