#ifndef ACTOR_COMBINE
#define ACTOR_COMBINE

#include "util/communication.h"
typedef struct actor_Combine_struct {
    port_in   *X0, *X1, *X2, *X3;
    port_out  *Y0, *Y1;
    int isz;
    int osz;
    bool row;
    void (*run) (struct actor_Combine_struct *self);
    void (*end) (struct actor_Combine_struct *self);
    bool (*not_finished) (struct actor_Combine_struct *self);
} actor_Combine;
void actor_Combine_init(actor_Combine *a);
#endif
