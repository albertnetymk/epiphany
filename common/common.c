#include <e_coreid.h>
#include "types.h"
#include "common.h"
#include "common_buffers.h"
#include "timers.h"

inline static unsigned core_num()
{
    e_coreid_t coreid = e_get_coreid();
    unsigned row, col;

    row = (coreid >> 6) & (E_ROWS_IN_CHIP - 1);
    col =  coreid       & (E_COLS_IN_CHIP - 1);
    return row * E_COLS_IN_CHIP + col;
}

inline void *address_from_coreid(e_coreid_t coreid, volatile void *ptr)
{
    return e_address_from_coreid(coreid, (void *)ptr);
}

void stage(uint s)
{
    while(Mailbox.core.go[core_num()] < s) ;
}

void stage_all(uint s)
{
    int i;
    for (i=0; i<Mailbox.players; ++i) {
        while(Mailbox.core.go[i] < s) ;
    }
}

void core_source_main(actor_source *a, source_init_t *init)
{
    int i;
    Mailbox.core.go[core_num()] = 0;

    for (i = 0; i < 10; ++i) {
        Mailbox.sink[i] = 0;
    }

    stage(1);
    source_api_t *api = init(a);

    Mailbox.core.go[core_num()] = 2;
    stage_all(2);
    // network
    api->connect_network();

    for(i=0; i<Mailbox.players; ++i) {
        Mailbox.core.go[i] = 3;
    }

    stage(3);
    for (i = 0; i < Mailbox.data_size; ++i) {
        epiphany_write(a->out, Mailbox.source[i]);
    }
    api->end(a);
    while(1) ;
}

inline void core_main(void *a, init_t *init)
{
    Mailbox.core.go[core_num()] = 0;

    stage(1);
    api_t *api = init(a);

    Mailbox.core.go[core_num()] = 2;
    stage_all(2);

    stage(3);
    init_clock();
    while(api->not_finished(a)) {
        api->run(a);
    }
    api->end(a);
    Mailbox.core.cycles[core_num()] = get_time();
    Mailbox.core.clocks[core_num()] = get_clock();
    while(1) ;
}

inline void core_sink_main(actor_sink *a, sink_init_t *init)
{
    int i;
    Mailbox.core.go[1] = 0;

    stage(1);
    sink_api_t *api = init(a);

    Mailbox.core.go[1] = 2;
    stage_all(2);

    stage(3);
    int value;
    for (i = 0; i < Mailbox.data_size; ++i) {
        value = epiphany_read(a->in);
        Mailbox.sink[i] = value;
    }
    while(1) ;
}

inline void core2_main(actor_double *a)
{
    Mailbox.core.go[core_num()] = 0;

    stage(1);
    all.instance_double1 = a;
    actor_double_init(a);

    Mailbox.core.go[core_num()] = 2;
    stage_all(2);

    stage(3);
    a->run(a);
    flush(a->out);
    while(1) ;
}

inline void core3_main(actor_double *a)
{
    Mailbox.core.go[core_num()] = 0;

    stage(1);
    all.instance_double2 = a;
    actor_double_init(a);

    Mailbox.core.go[core_num()] = 2;
    stage_all(2);

    stage(3);
    a->run(a);
    flush(a->out);
    while(1) ;
}

inline void core4_main(actor_add *a)
{
    Mailbox.core.go[core_num()] = 0;

    stage(1);
    all.instance_add = a;
    actor_add_init(a);

    Mailbox.core.go[core_num()] = 2;
    stage_all(2);

    stage(3);
    a->run(a);
    flush(a->out);
    while(1) ;
}
