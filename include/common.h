#ifndef _COMMON_H
#define _COMMON_H
#include "actors.h"

extern volatile actors all;

typedef void run_t (void *);
typedef bool not_finished_t (void *);
typedef void end_t (void *);
typedef struct api_t_struct {
    run_t *run;
    end_t *end;
    not_finished_t *not_finished;
} api_t;

typedef api_t *init_t (void *);
inline void core_main(void *a, init_t *init);

inline void *address_from_coreid(e_coreid_t coreid, volatile void *ptr);
inline void core_source_main(actor_a *a);
inline void core_sink_main(actor_b *a);
inline void core2_main(actor_double *a);
inline void core3_main(actor_double *a);
inline void core4_main(actor_add *a);
// void core1_main(actor_b *a);
// void core1_main(actor_b *a);
#endif
