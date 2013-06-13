#ifndef ACTOR_TRANSPOSE
#define ACTOR_TRANSPOSE

#include "util/communication.h"
typedef struct actor_Transpose_struct {
    port_in   *X0, *X1, *X2, *X3;
    port_out  *Y0, *Y1;
    int sz;
    void (*run) (struct actor_Transpose_struct *self);
    void (*end) (struct actor_Transpose_struct *self);
    bool (*not_finished) (struct actor_Transpose_struct *self);
} actor_Transpose;
void actor_Transpose_init(actor_Transpose *a);
#endif
