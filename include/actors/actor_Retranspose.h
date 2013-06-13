#ifndef ACTOR_RETRANSPOSE
#define ACTOR_RETRANSPOSE

#include "util/communication.h"
typedef struct actor_Retranspose_struct {
    port_in   *X0, *X1, *X2, *X3;
    port_out  *Y;
    int isz;
    int osz;
    void (*run) (struct actor_Retranspose_struct *self);
    void (*end) (struct actor_Retranspose_struct *self);
    bool (*not_finished) (struct actor_Retranspose_struct *self);
} actor_Retranspose;
void actor_Retranspose_init(actor_Retranspose *a);
#endif
