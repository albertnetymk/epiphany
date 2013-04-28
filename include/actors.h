#ifndef _ACTORS_H
#define _ACTORS_H

#include "actor_source.h"
#include "actor_sink.h"
#include "actor_double.h"
#include "actor_add.h"

typedef struct actors_struct {
    actor_source *instance_source;
    actor_double *instance_double1;
    actor_double *instance_double2;
    actor_add *instance_add;
    actor_sink *instance_sink;
} actors;

#endif /* end of include guard: _ACTORS_H */

