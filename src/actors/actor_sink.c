#include <stdlib.h>
#include "actor_sink.h"
void actor_sink_init(actor_sink *a)
{
    port_in_init(a->in);
}
