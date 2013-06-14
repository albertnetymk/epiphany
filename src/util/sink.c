#include "util/matmul.h"

void network_write(network_sink *p, int v)
{
    inc_core_at_index(2);
    p->array[p->index++] = v;
    inc_core_at_index(3);
}
