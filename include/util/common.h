#ifndef _COMMON_H
#define _COMMON_H
#include "actors/actors.h"

extern volatile actors all;

typedef void run_t (void *);
typedef bool not_finished_t (void *);
typedef void end_t (void *);
typedef struct api_t_struct {
    run_t *run;
    end_t *end;
    not_finished_t *not_finished;
} api_t;

typedef void connect_network_t (void);
typedef void prepare_data_t (void);
typedef struct source_api_t_struct {
    connect_network_t *connect_network;
    prepare_data_t *prepare_data;
    end_t *end;
} source_api_t;
typedef source_api_t *source_init_t (void *);

typedef void fetch_data_t (void);
typedef struct sink_api_t_struct {
    fetch_data_t *fetch_data;
} sink_api_t;
typedef sink_api_t *sink_init_t (void *);

typedef api_t *init_t (void *);
inline void core_main(void *a, init_t *init);

inline void *address_from_coreid(e_coreid_t coreid, volatile void *ptr);
inline void core_source_main(actor_source *a, source_init_t *init);
inline void core_sink_main(actor_sink *a, sink_init_t *init);
inline void core2_main(actor_double *a);
inline void core3_main(actor_double *a);
inline void core4_main(actor_add *a);
// void core1_main(actor_b *a);
// void core1_main(actor_b *a);
#endif
