#ifndef _ACTOR_A_H
#define _ACTOR_A_H

#include "communication.h"
typedef struct actor_a_struct {
    port_out *out;
} actor_a;
actor_a *actor_a_new();

#endif /* end of include guard: _ACTOR_A_H */
