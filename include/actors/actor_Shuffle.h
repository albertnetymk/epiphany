#ifndef ACTOR_SHUFFLE
#define ACTOR_SHUFFLE

#include "util/communication.h"
typedef struct actor_Shuffle_struct {
    port_in   *X0, *X1, *X2, *X3;
    port_out  *Y0, *Y1, *Y2, *Y3;
    int sz;
    void (*run) (struct actor_Shuffle_struct *self);
    void (*end) (struct actor_Shuffle_struct *self);
    bool (*not_finished) (struct actor_Shuffle_struct *self);
} actor_Shuffle;
void actor_Shuffle_init(actor_Shuffle *a);
#endif
