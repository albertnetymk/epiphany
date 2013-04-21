#ifndef _ACTORS_H
#define _ACTORS_H

#include "actor_a.h"
#include "actor_double.h"
#include "actor_add.h"
#include "actor_b.h"

typedef struct actors_struct {
    actor_a *instance_a;
    actor_double *instance_double1;
    actor_double *instance_double2;
    actor_add *instance_add;
    actor_b *instance_b;
} actors;

#endif /* end of include guard: _ACTORS_H */

