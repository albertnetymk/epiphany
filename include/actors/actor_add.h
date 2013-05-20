#ifndef ACTOR_ADD_H_U3BIYYBH
#define ACTOR_ADD_H_U3BIYYBH

#include "communication.h"
typedef struct actor_add_struct {
    port_in *in1;
    port_in *in2;
    port_out *out;
    void (*run)(struct actor_add_struct *self);
    void (*end)(struct actor_add_struct *self);
    bool (*not_finished)(struct actor_add_struct *self);
} actor_add;
void actor_add_init(actor_add *a);

#endif /* end of include guard: ACTOR_ADD_H_U3BIYYBH */

