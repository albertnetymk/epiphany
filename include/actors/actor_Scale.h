#ifndef ACTOR_SCALE
#define ACTOR_SCALE

#include "util/communication.h"
typedef struct actor_Scale_struct {
    port_in   *X0, *X1;
    port_out  *Y0, *Y1, *Y2, *Y3;
    int isz;
    int osz;
    int csz;
    void (*run) (struct actor_Scale_struct *self);
    void (*end) (struct actor_Scale_struct *self);
    bool (*not_finished) (struct actor_Scale_struct *self);
} actor_Scale;
void actor_Scale_init(actor_Scale *a);
#endif
