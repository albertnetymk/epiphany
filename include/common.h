#ifndef _COMMON_H
#define _COMMON_H
#include "actors.h"

inline void *address_from_coreid(e_coreid_t coreid, volatile void *ptr);
void core0_main(actor_a *a);
void core1_main(actor_b *a);
#endif
