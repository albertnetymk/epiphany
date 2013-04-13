#ifndef _ACTOR_A_H
#define _ACTOR_A_H

#include "communication.h"
typedef struct actor_a_struct {
    port_out *out;
} actor_a;
void actor_a_init(actor_a *a);

#endif /* end of include guard: _ACTOR_A_H */
