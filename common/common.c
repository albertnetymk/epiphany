#include <e_coreid.h>
#include "types.h"
#include "common_buffers.h"
#include "common.h"

unsigned core_num(e_coreid_t coreid)
{
    unsigned row, col;

    row = (coreid >> 6) & (E_ROWS_IN_CHIP - 1);
    col =  coreid       & (E_COLS_IN_CHIP - 1);
    return row * E_COLS_IN_CHIP + col;
}

void dummy(int i, int v)
{
    Mailbox.core.go[i] = 1;
    while(Mailbox.core.go[i] == 1) ;
    // Mailbox.dummy[i] = v;
    Mailbox.core.go[i] = 1;
    while(1);
}

void stage(uint s)
{
    uint index = core_num(e_get_coreid());
    while(Mailbox.core.go[index] < s) ;
}

void core0_main()
{
    int i;
    Mailbox.core.go[0] = 0;

    stage(1);
    for (i = 0; i < 10; ++i) {
        Mailbox.sink[i] = 0;
    }
    all.instance_a = actor_a_new();

    stage(2);
    // network
    connect(all.instance_a->out, all.instance_b->in);
    for(i=0; i<3; ++i) {
        Mailbox.core.go[i] = 3;
    }

    stage(3);
    while(Mailbox.core.go[0] < 3) ;
    // for (i = 0; i < 10; ++i) {
    //     epiphany_write(instance_a->out, i);
    // }
    while(1) ;
}

void core1_main()
{
    int i;
    Mailbox.core.go[1] = 0;

    stage(1);
    all.instance_b = actor_b_new();

    stage(2);

    stage(3);
    int value;
    for (i = 0; i < 10; ++i) {
        value = epiphany_read(all.instance_b->in);
        Mailbox.sink[i] = value;
    }
    while(1) ;
}
