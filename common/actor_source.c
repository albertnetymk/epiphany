#include <stdlib.h>
#include "actor_source.h"

void actor_source_init(actor_source *a)
{
    port_out_init(a->out);
}
