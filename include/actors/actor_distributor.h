#ifndef ACTOR_DISTRIBUTOR_H
#define ACTOR_DISTRIBUTOR_H

#include "communication.h"
typedef struct actor_distributor_struct {
    port_in *in;
    port_out *out;
    void (*run)(struct actor_distributor_struct *self);
    void (*end)(struct actor_distributor_struct *self);
    bool (*not_finished)(struct actor_distributor_struct *self);
} actor_distributor;
void actor_distributor_init(actor_distributor *a);

#endif /* end of include guard: ACTOR_DISTRIBUTOR_H */
