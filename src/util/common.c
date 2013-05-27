#include <e_coreid.h>
#include "util/types.h"
#include "util/common.h"
#include "util/common_buffers.h"
#include "util/timers.h"

void connect_network();

inline unsigned core_num()
{
    e_coreid_t coreid = e_get_coreid();
    unsigned row, col;

    row = (coreid >> 6) & (E_ROWS_IN_CHIP - 1);
    col =  coreid       & (E_COLS_IN_CHIP - 1);
    return row * E_COLS_IN_CHIP + col;
}

void print_core_index(int i) {
    Mailbox.debug_index[core_num()] = i;
}

void print_core_at_index(int v, int i) {
    unsigned char num = core_num();
    switch (num) {
        case 0:
            Mailbox.debug_zero[i] = v;
            break;
        case 1:
            Mailbox.debug_one[i] = v;
            break;
        case 2:
            Mailbox.debug_two[i] = v;
            break;
        case 3:
            Mailbox.debug_three[i] = v;
            break;
        case 4:
            Mailbox.debug_four[i] = v;
            break;
        case 5:
            Mailbox.debug_five[i] = v;
            break;
    }
    // Mailbox.debug[num*20 + i] = v;
}

void print_core(int v) {
    unsigned char num = core_num();
    uint index = Mailbox.debug_index[num];
    if (index < 20) {
        print_core_at_index(v, index);
        Mailbox.debug_index[num]++;
    }
    // if (index < 20) {
    //     Mailbox.debug[num*20 + index] = v;
    //     Mailbox.debug_index[num]++;
    // }
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

inline void core_main(void *a, init_t *init)
{
    Mailbox.core.go[core_num()] = 0;
    print_core_index(0);

    stage(1);
    api_t *api = init(a);

    Mailbox.core.go[core_num()] = 2;
    stage_all(2);

    if (core_num() == 0) {
        connect_network();
    }

    stage(3);
    init_clock();
    while(1) {
        api->run(a);
        if (!api->not_finished(a)) {
            break;
        }
    }
    Mailbox.core.go[core_num()] = 4;
    api->end(a);
    Mailbox.core.cycles[core_num()] = get_time();
    Mailbox.core.clocks[core_num()] = get_clock();
    Mailbox.core.go[core_num()] = 5;
    // if (core_num() == 5) {
    //     print_core_at_index(-1, 0);
    //     print_core_at_index(all.instance_double1->in->buffers[0]->total, 1);
    //     print_core_at_index(all.instance_double1->in->buffers[1]->total, 2);
    //     print_core_at_index(all.instance_double2->in->buffers[0]->total, 3);
    //     print_core_at_index(all.instance_double2->in->buffers[1]->total, 4);
    // }
    while(1) ;
}
