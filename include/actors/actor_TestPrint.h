#ifndef ACTOR_TESTPRINT
#define ACTOR_TESTPRINT

#include "util/communication.h"
typedef struct actor_TestPrint_struct {
    port_in   *S, *A, *B;
    port_out  *Out;
    void (*run) (struct actor_TestPrint_struct *self);
    void (*end) (struct actor_TestPrint_struct *self);
    bool (*not_finished) (struct actor_TestPrint_struct *self);
} actor_TestPrint;
void actor_TestPrint_init(actor_TestPrint *a);
#endif
