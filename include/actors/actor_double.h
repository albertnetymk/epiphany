#ifndef ACTOR_DOUBLE_H_A6EW1BO9
#define ACTOR_DOUBLE_H_A6EW1BO9

#include "util/communication.h"
typedef struct actor_double_struct {
    port_in *in;
    port_out *out;
    void (*run)(struct actor_double_struct *self);
    void (*end)(struct actor_double_struct *self);
    bool (*not_finished)(struct actor_double_struct *self);
} actor_double;
void actor_double_init(actor_double *a);

#endif /* end of include guard: ACTOR_DOUBLE_H_A6EW1BO9 */

