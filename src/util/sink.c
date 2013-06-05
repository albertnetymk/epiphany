#include "util/matmul.h"

void network_write(network_sink *p, int v)
{
    p->array[p->index++] = v;
}
