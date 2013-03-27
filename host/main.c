#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <e_host.h>
#include "common_buffers.h"

unsigned int DRAM_BASE = 0x81000000;

const char *servIP = "127.0.0.1";
const unsigned short eServLoaderPort = 50999;
FILE *fo;

int main(void) {
    // Epiphany_t _epiphany, *epiphany = &_epiphany;
    // int rv;

    // e_set_host_verbosity(2);
    // e_set_loader_verbosity(2);

    // rv = e_load("../epiphany/main.srec", 1, 1, 0);

    int i;
    int start = 0;
    int end = 2;
    puts("Host is running ...");
    fo = stdout;
    for (i=start; i<end; ++i) {
        Mailbox.core.go[i] = 0;
    }

    // for (i=start; i<end; ++i) {
    // printf("go is %d\n", Mailbox.core.go[i]);
    // }

    if (e_open((char *) servIP, eServLoaderPort)) {
        fprintf(fo, "\nERROR: Can't establish connection to E-SERVER!\n\n");
        exit(1);
    }

    unsigned int addr;

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

    for (i=start; i<end; ++i) {
        addr = DRAM_BASE + offsetof(shared_buf_t, core.go[i]);
        // while (1) {
        e_read(addr, (void *) (&Mailbox.core.go[i]), sizeof(int));
        if (Mailbox.core.go[i] == 1){
            addr = DRAM_BASE + offsetof(shared_buf_t, dummy[i]);
            e_read(addr, (void *) (&Mailbox.dummy[i]), sizeof(int));
            printf("read from core %d: %p\n", i, Mailbox.dummy[i]);
        } else {
            printf("Skip for %dth core\n", i);
        }
        // }
    }
    // printf("int is %d\n", sizeof(int));

    if (e_close()) {
        fprintf(fo, "\nERROR: Can't close connection to E-SERVER!\n\n");
        exit(1);
    }

    return 0;
}
