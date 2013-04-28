#ifndef _ACTOR_B_H
#define _ACTOR_B_H

#include "communication.h"
typedef struct actor_sink_struct {
    port_in *in;
} actor_sink;

void actor_sink_init(actor_sink *a);

#endif /* end of include guard: _ACTOR_B_H */

