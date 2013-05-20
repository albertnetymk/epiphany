#ifndef ACTOR_SOURCE_H
#define ACTOR_SOURCE_H

#include "util/communication.h"
typedef struct actor_source_struct {
    port_out *out;
    void (*end)(struct actor_source_struct *self);
} actor_source;
void actor_source_init(actor_source *a);

#endif /* end of include guard: ACTOR_SOURCE_H */
