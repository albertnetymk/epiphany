#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <e_host.h>
#include "util/common_buffers.h"

static char *error_color = "\x1B[0;31m";
static char *success_color = "\x1B[0;32m";
static char *reset = "\x1B[0m";

unsigned int DRAM_BASE = 0x81000000;
unsigned int addr;

const char *servIP = "127.0.0.1";
const unsigned short eServLoaderPort = 50999;
FILE *fo;

void ok(bool assertion, char *msg)
{
    if (!assertion) {
        printf("\t%sError: '%s' is unsatisfied.%s\n", error_color, msg, reset);
        exit(-1);
    } else {
        // printf("\t%sSuccess: %s.%s\n", success_color, msg, reset);
        printf("%s.%s", success_color, reset);
    }
}
void show_core_go()
{
    int i;
    // for (i = 0; i < 1; ++i) {
    //     addr = DRAM_BASE + offsetof(shared_buf_t, core.go[i]);
    //     e_read(addr, (void *) (&Mailbox.core.go[i]), sizeof(int));
    //     printf("core %d is in stage %d\n", i, Mailbox.core.go[i]);
    // }
    for (i = 0; i < Mailbox.players; ++i) {
        addr = DRAM_BASE + offsetof(shared_buf_t, core.go[i]);
        e_read(addr, (void *) (&Mailbox.core.go[i]), sizeof(int));
        printf("core %d is in stage %d\n", i, Mailbox.core.go[i]);
    }
    addr = DRAM_BASE + offsetof(shared_buf_t, sink);
    e_read(addr, (void *) Mailbox.sink, sizeof(int)*20);
    for (i = 0; i < 5; ++i) {
        printf("sink[%d] = %d\n", i, Mailbox.sink[i]);
    }
}

void show_debug_info()
{
    int i;
    addr = DRAM_BASE + offsetof(shared_buf_t, debug_one);
    e_read(addr, (void *) Mailbox.debug_one, sizeof(int)*20);
    addr = DRAM_BASE + offsetof(shared_buf_t, debug_two);
    e_read(addr, (void *) Mailbox.debug_two, sizeof(int)*20);
    addr = DRAM_BASE + offsetof(shared_buf_t, debug_three);
    e_read(addr, (void *) Mailbox.debug_three, sizeof(int)*20);
    addr = DRAM_BASE + offsetof(shared_buf_t, debug_four);
    e_read(addr, (void *) Mailbox.debug_four, sizeof(int)*20);
    addr = DRAM_BASE + offsetof(shared_buf_t, debug_five);
    e_read(addr, (void *) Mailbox.debug_five, sizeof(int)*20);
    for (i = 0; i < 10; i += 1) {
        printf("line %d, core %d: %d,\tcore %d: %d,\tcore %d: %d,\t core %d, %d,\t core %d, %d,\t core %d, %d\n",
                i,
                0, Mailbox.debug_zero[i],
                1, Mailbox.debug_one[i],
                2, Mailbox.debug_two[i],
                3, Mailbox.debug_three[i],
                4, Mailbox.debug_four[i],
                5, Mailbox.debug_five[i]
              );
    }
}

int main(int argc, char **argv) {
    // Epiphany_t _epiphany, *epiphany = &_epiphany;
    // int rv;

    // e_set_host_verbosity(2);
    // e_set_loader_verbosity(2);

    // rv = e_load("../epiphany/main.srec", 1, 1, 0);

    int i, j, n;
    int start = 0;
    int end = 5;
    puts("Host is running ...");
    fo = stdout;
    for (i=start; i<end; ++i) {
        Mailbox.core.go[i] = 1;
    }

    Mailbox.players = end;

    int data_size;
    if ( argc > 1 ) {
        data_size = atoi(argv[1]);
    } else {
        data_size = 9;
    }

    // Mailbox.data_size = data_size;
    // for (i=0; i<data_size; ++i) {
    //     Mailbox.source[i] = 2*i;
    // }
    int input[2][24] = {
        { 1458 , -249 , -289 , -89  , 49  , 0   , 69 , -29 , 89 , -29 , -69 , 0   , 69 , 0 , -69 , 0 , 69  , 0 , 0 , 0   , 0 , 0 , 0 , 0 },
        { -169 , -49  , 0    , -149 , -29 , -49 , 0  , 0   , 49 , 0   , 0   , -29 , 0  , 0 , 0   , 0 , -29 , 0 , 0 , -29 , 0 , 0 , 0 , -29 }
    };
    for (i = 0; i < 2; ++i) {
        Mailbox.n_source[i].size = 23;
        Mailbox.n_source[i].index = 0;
        for (j = 0; j < sizeof(input[i])/sizeof(int); ++j) {
            Mailbox.n_source[i].array[j] = input[i][j];
        }
    }
    int expect[4][12] = {
        { 2329  , 11048 , 531  , 2107 , -238 , -391 , -214 , -97  , 47  , 848  , -273 , 64 },
        { 12665 , 13853 , -636 , 165  , 1839 , 780  , 1318 , 1201 , 593 , 720  , 273  , -64 },
        { 12588 , 13222 , 252  , 482  , 1437 , 334  , 400  , 118  , 502 , 1106 , 182  , 322 },
        { 13665 , 13941 , 493  , -258 , 1377 , 558  , 704  , 986  , 138 , 462  , -182 , -322 }
    };

    // for (i=start; i<end; ++i) {
    // printf("go is %d\n", Mailbox.core.go[i]);
    // }

    if (e_open((char *) servIP, eServLoaderPort)) {
        fprintf(fo, "\nERROR: Can't establish connection to E-SERVER!\n\n");
        exit(1);
    }

    // Decide how many cores are used.
    addr = DRAM_BASE + offsetof(shared_buf_t, players);
    e_write(addr, (void *) &(Mailbox.players), sizeof(int));

    // Prepare input data.
    addr = DRAM_BASE + offsetof(shared_buf_t, source);
    e_write(addr, (void *) Mailbox.source, sizeof(int)*data_size);

    addr = DRAM_BASE + offsetof(shared_buf_t, data_size);
    e_write(addr, (void *) &(Mailbox.data_size), sizeof(int));

    addr = DRAM_BASE + offsetof(shared_buf_t, core.go);
    e_write(addr, (void *) (&Mailbox.core.go[0]), sizeof(int)*end);
    puts("Waiting for the board to finish...");
    sleep(1);

    char msg[50];
    show_core_go();
    // while (n<2) {
    //     addr = DRAM_BASE + offsetof(shared_buf_t, core.go[1]);
    //     e_read(addr, (void *) (&Mailbox.core.go[1]), sizeof(int));
    //     if (Mailbox.core.go[4] == 4) {
    //         addr = DRAM_BASE + offsetof(shared_buf_t, sink);
    //         for (i = 0; i < sizeof(expect)/sizeof(expect[0]); ++i) {
    //             e_read(addr, (void *) (Mailbox.n_sink[i].array),
    //                     sizeof(expect[i]));
    //         }
    //         for (i = 0; i < sizeof(expect)/sizeof(expect[0]); ++i) {
    //             for (j = 0; j < sizeof(expect[i])/sizeof(int); ++j)
    //             {
    //                 sprintf(msg, "n_sink[%d] should be %d, but %d is found", i,
    //                         expect[i][j], Mailbox.n_sink[i].array[j]);
    //             }
    //         }
    //         printf("\n");
    //         break;
    //         // exit(-1);
    //         // show_debug_info();
    //     } else {
    //         show_core_go();
    //         show_debug_info();
    //     }
    //     printf("Board hasn't finished yet... %d\n", n++);
    //     sleep(2);
    // }

    // for (i = 0; i < end; ++i) {
    //     addr = DRAM_BASE + offsetof(shared_buf_t, core.clocks[i]);
    //     e_read(addr, (void *) (&Mailbox.core.clocks[i]), sizeof(int));
    //     addr = DRAM_BASE + offsetof(shared_buf_t, core.cycles[i]);
    //     e_read(addr, (void *) (&Mailbox.core.cycles[i]), sizeof(int));

    //     printf("pending/clock for %d is %u/%u\n", i,
    //             Mailbox.core.cycles[i], Mailbox.core.clocks[i]);
    // }

    if (e_close()) {
        fprintf(fo, "\nERROR: Can't close connection to E-SERVER!\n\n");
        exit(1);
    }

    return 0;
}
