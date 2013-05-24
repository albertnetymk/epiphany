#ifndef ACTORS_H
#define ACTORS_H

#include "actors/actor_source.h"
#include "actors/actor_sink.h"
#include "actors/actor_distributor.h"
#include "actors/actor_double.h"
#include "actors/actor_add.h"

typedef struct actors_struct {
    actor_source *instance_source;
    actor_distributor *instance_distributor;
    actor_double *instance_double1;
    actor_double *instance_double2;
    actor_add *instance_add;
    actor_sink *instance_sink;
} actors;

#endif /* end of include guard: _ACTORS_H */

