#ifndef ACTOR_FINAL
#define ACTOR_FINAL

#include "util/communication.h"
typedef struct actor_Final_struct {
    port_in   *X0, *X1, *X2, *X3;
    port_out  *Y0, *Y1, *Y2, *Y3;
    int isz;
    int osz;
    void (*run) (struct actor_Final_struct *self);
    void (*end) (struct actor_Final_struct *self);
    bool (*not_finished) (struct actor_Final_struct *self);
} actor_Final;
void actor_Final_init(actor_Final *a);
#endif
