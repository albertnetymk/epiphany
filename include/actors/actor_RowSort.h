#ifndef ACTOR_ROWSORT
#define ACTOR_ROWSORT

#include "util/communication.h"
typedef struct actor_RowSort_struct {
    port_in   *ROW;
    port_out  *Y0, *Y1;
    int sz;
    void (*run) (struct actor_RowSort_struct *self);
    void (*end) (struct actor_RowSort_struct *self);
    bool (*not_finished) (struct actor_RowSort_struct *self);
} actor_RowSort;
void actor_RowSort_init(actor_RowSort *a);
#endif
