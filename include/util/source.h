#ifndef SOURCE_H_G9FRYOZW
#define SOURCE_H_G9FRYOZW

#include <stdbool.h>
#include "util/matmul.h"
bool network_not_finished(volatile network_source *p);
int network_read(volatile network_source *p);
int network_consume(volatile network_source *p);

#endif /* end of include guard: SOURCE_H_G9FRYOZW */

