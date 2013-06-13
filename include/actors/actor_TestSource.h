#ifndef ACTOR_TESTSOURCE
#define ACTOR_TESTSOURCE

#include "util/communication.h"
typedef struct actor_TestSource_struct {
    port_in   *Next;
    port_out  *V, *Signed;
    void (*run) (struct actor_TestSource_struct *self);
    void (*end) (struct actor_TestSource_struct *self);
    bool (*not_finished) (struct actor_TestSource_struct *self);
} actor_TestSource;
void actor_TestSource_init(actor_TestSource *a);
#endif
