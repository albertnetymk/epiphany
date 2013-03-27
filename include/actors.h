#ifndef _ACTORS_H
#define _ACTORS_H

#include "actor_a.h"
#include "actor_b.h"

typedef struct actors_struct {
    actor_a *instance_a;
    actor_b *instance_b;
} actors;

#endif /* end of include guard: _ACTORS_H */

