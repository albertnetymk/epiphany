#ifndef ACTOR_SHIFT
#define ACTOR_SHIFT

#include "util/communication.h"
typedef struct actor_Shift_struct {
    port_in   *X0, *X1, *X2, *X3;
    port_out  *Y0, *Y1, *Y2, *Y3;
    int isz;
    int csz;
    void (*run) (struct actor_Shift_struct *self);
    void (*end) (struct actor_Shift_struct *self);
    bool (*not_finished) (struct actor_Shift_struct *self);
} actor_Shift;
void actor_Shift_init(actor_Shift *a);
#endif
