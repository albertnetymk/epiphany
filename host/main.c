#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <e_host.h>
#include "common_buffers.h"

static char *error_color = "\x1B[0;31m";
static char *success_color = "\x1B[0;32m";
static char *reset = "\x1B[0m";

unsigned int DRAM_BASE = 0x81000000;

const char *servIP = "127.0.0.1";
const unsigned short eServLoaderPort = 50999;
FILE *fo;

void ok(bool assertion, char *msg)
{
    if (!assertion) {
        printf("\t%sError: '%s' is unsatisfied.%s\n", error_color, msg, reset);
    } else {
        printf("\t%sSuccess: %s.%s\n", success_color, msg, reset);
    }
}

int main(void) {
    // Epiphany_t _epiphany, *epiphany = &_epiphany;
    // int rv;

    // e_set_host_verbosity(2);
    // e_set_loader_verbosity(2);

    // rv = e_load("../epiphany/main.srec", 1, 1, 0);

    int i, j;
    int start = 0;
    int end = 6;
    puts("Host is running ...");
    fo = stdout;
    for (i=start; i<end; ++i) {
        Mailbox.core.go[i] = 1;
    }

    Mailbox.players = end;

    int data_size = 10;
    Mailbox.data_size = data_size;
    for (i=0; i<data_size; ++i) {
        Mailbox.source[i] = 2*i;
    }

    // for (i=start; i<end; ++i) {
    // printf("go is %d\n", Mailbox.core.go[i]);
    // }

    if (e_open((char *) servIP, eServLoaderPort)) {
        fprintf(fo, "\nERROR: Can't establish connection to E-SERVER!\n\n");
        exit(1);
    }

    unsigned int addr;
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

    // for (i=start; i<end; ++i) {
    // // sleep(2);
    // addr = DRAM_BASE + offsetof(shared_buf_t, core.go[i]);
    // e_read(addr, (void *) (&Mailbox.core.go[i]), sizeof(int));
    // }

    // for (i=start; i<end; ++i) {
    // printf("go is %d\n", Mailbox.core.go[i]);
    // }

    char msg[50];
    for (i = 0; i < data_size; i += 1) {
        addr = DRAM_BASE + offsetof(shared_buf_t, sink[i]);
        e_read(addr, (void *) (&Mailbox.sink[i]), sizeof(int));
        addr = DRAM_BASE + offsetof(shared_buf_t, debug[i]);
        e_read(addr, (void *) (&Mailbox.debug[i]), sizeof(int));
        // printf("sink[%d]: %d\tdebug[%d]: %d\n", i, Mailbox.sink[i],
        //         i, Mailbox.debug[i]);
        sprintf(msg, "sink[%d] should be %d, but %d is found", i,
                4*Mailbox.source[i], Mailbox.sink[i]);
        ok(Mailbox.sink[i] == 4*Mailbox.source[i], msg);
    }

    // for (i = 0; i < end; ++i) {
    //     addr = DRAM_BASE + offsetof(shared_buf_t, core.clocks[i]);
    //     e_read(addr, (void *) (&Mailbox.core.clocks[i]), sizeof(int));
    //     addr = DRAM_BASE + offsetof(shared_buf_t, core.cycles[i]);
    //     e_read(addr, (void *) (&Mailbox.core.cycles[i]), sizeof(int));

    //     printf("pending/clock for %d is %d/%d\n", i,
    //             Mailbox.core.cycles[i], Mailbox.core.clocks[i]);
    // }
    for (i = 2; i < end; ++i) {
        for (j = 0; j < data_size; j += 1) {
            addr = DRAM_BASE + offsetof(shared_buf_t, core.debug_line[i][j]);
            e_read(addr, (void *) (&Mailbox.core.debug_line[i][j]), sizeof(int));
            printf("core %d, %d\n", i, Mailbox.core.debug_line[i][j]);
        }
    }

    // int should[10] = {};
    // for (i = 0; i < 10; ++i) {
    //     addr = DRAM_BASE + offsetof(shared_buf_t, sink[i]);
    //     e_read(addr, (void *) (&Mailbox.sink[i]), sizeof(int));
    //     sprintf(msg, "sink[i] should be %d, but %d is found", should[i], Mailbox.sink[i]);
    //     ok(Mailbox.sink[i] == should[i], msg);
    // }

    // for (i=start; i<end; ++i) {
    //     addr = DRAM_BASE + offsetof(shared_buf_t, core.go[i]);
    //     // while (1) {
    //     e_read(addr, (void *) (&Mailbox.core.go[i]), sizeof(int));
    //     if (Mailbox.core.go[i] == 1){
    //         addr = DRAM_BASE + offsetof(shared_buf_t, dummy[i]);
    //         e_read(addr, (void *) (&Mailbox.dummy[i]), sizeof(int));
    //         printf("read from core %d: %p\n", i, Mailbox.dummy[i]);
    //     } else {
    //         printf("Skip for %dth core\n", i);
    //     }
    //     // }
    // }

    if (e_close()) {
        fprintf(fo, "\nERROR: Can't close connection to E-SERVER!\n\n");
        exit(1);
    }

    return 0;
}
