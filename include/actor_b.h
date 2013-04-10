#ifndef _ACTOR_B_H
#define _ACTOR_B_H

#include "communication.h"
typedef struct actor_b_struct {
    volatile port_in *in;
} actor_b;

void actor_b_init(actor_b *a);

#endif /* end of include guard: _ACTOR_B_H */

