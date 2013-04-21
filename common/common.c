#include <e_coreid.h>
#include "types.h"
#include "actors.h"
#include "common_buffers.h"
#include "common.h"

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

static const uint players = 4;

void stage_all(uint s)
{
    int i;
    for (i=0; i<players; ++i) {
        while(Mailbox.core.go[i] < s) ;
    }
}

void core0_main(actor_a *a)
{
    int i;
    Mailbox.core.go[core_num()] = 0;

    for (i = 0; i < 10; ++i) {
        Mailbox.sink[i] = 0;
    }

    stage(1);
    all.instance_a = a;
    actor_a_init(a);

    Mailbox.core.go[core_num()] = 2;
    stage_all(2);
    // network
    // connect(all.instance_a->out, all.instance_b->in);
    connect(all.instance_a->out, all.instance_double1->in);
    connect(all.instance_a->out, all.instance_double2->in);
    connect(all.instance_double1->out, all.instance_add->in1);
    connect(all.instance_double2->out, all.instance_add->in2);
    connect(all.instance_add->out, all.instance_b->in);
    for(i=0; i<players; ++i) {
        Mailbox.core.go[i] = 3;
    }

    stage(3);
    for (i = 0; i < 10; ++i) {
        epiphany_write(a->out, i);
    }
    flush(a->out);
    while(1) ;
}

inline void core1_main(actor_b *a)
{
    int i;
    Mailbox.core.go[1] = 0;

    stage(1);
    all.instance_b = a;
    actor_b_init(a);

    Mailbox.core.go[1] = 2;
    stage_all(2);

    stage(3);
    int value;
    for (i = 0; i < 10; ++i) {
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
    while(1) ;
}
