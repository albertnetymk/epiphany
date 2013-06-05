#include <stdbool.h>
#include "util/matmul.h"

bool network_not_finished(network_source *p)
{
    return p->index < p->size;
}

int network_read(network_source *p)
{
    return p->array[p->index];
}

int network_consume(network_source *p)
{
    return p->array[p->index++];
}
