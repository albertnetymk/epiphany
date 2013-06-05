#ifndef ACTOR_SHUFFLEFLY
#define ACTOR_SHUFFLEFLY

#include "util/communication.h"
typedef struct actor_ShuffleFly_struct {
    port_in   *X0, *X1;
    port_out  *Y0, *Y1, *Y2, *Y3;
    int sz;
    void (*run) (struct actor_ShuffleFly_struct *self);
    void (*end) (struct actor_ShuffleFly_struct *self);
    bool (*not_finished) (struct actor_ShuffleFly_struct *self);
} actor_ShuffleFly;
void actor_ShuffleFly_init(actor_ShuffleFly *a);
#endif
