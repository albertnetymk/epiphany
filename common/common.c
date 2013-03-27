#include <e_coreid.h>
#include "common_buffers.h"
#include "common.h"

// volatile int list[5] SECTION(".data_bank1");
// volatile int flag[5] SECTION(".data_bank1");

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
    Mailbox.dummy[i] = v;
    Mailbox.core.go[i] = 1;
    while(1);
}

void core0_main()
{
}
void core1_main()
{
}
