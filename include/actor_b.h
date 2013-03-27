#ifndef _ACTOR_B_H
#define _ACTOR_B_H

#include "communication.h"
typedef struct actor_b_struct {
    port_in *in;
} actor_b;

actor_b *actor_b_new();

#endif /* end of include guard: _ACTOR_B_H */

