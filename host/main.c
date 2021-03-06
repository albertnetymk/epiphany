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
static int expect[1][64000];

void ok(bool assertion, char *msg)
{
    if (!assertion) {
        printf("%sX%s", error_color, reset);
        // printf("\t%sError: '%s' is unsatisfied.%s\n", error_color, msg, reset);
        // exit(-1);
    } else {
        // printf("\t%sSuccess: %s.%s\n", success_color, msg, reset);
        printf("%s.%s", success_color, reset);
    }
}
void show_core_go()
{
    addr = DRAM_BASE + offsetof(shared_buf_t, core.go);
    e_read(addr, (void *) Mailbox.core.go, sizeof(Mailbox.core.go));
    int i;
    printf("       ");
    for (i = 0; i < Mailbox.players; ++i) {
        printf("c%d: %d\t", i, Mailbox.core.go[i]);
    }
    printf("\n");
}

void show_debug_info()
{
    addr = DRAM_BASE + offsetof(shared_buf_t, debug);
    e_read(addr, (void *) Mailbox.debug, sizeof(Mailbox.debug));
    int i, j;
    // for (i = 0; i < Mailbox.players; ++i) {
    for (i = 0; i < 4; ++i) {
        printf("line %d ", i);
        for (j = 0; j < Mailbox.players; ++j) {
            printf("c%d: %d\t", j, Mailbox.debug[j][i]);
        }
        printf("\n");
    }
    // j = 12;
    // for (i = 4; i < 30; ++i) {
    //     printf("line %d ", i-3);
    //     printf("c%d: %d\n", j, Mailbox.debug[j][i]);
    // }
}

void compare_n_sink(int size)
{
    int i, j;
    char msg[50];
    addr = DRAM_BASE + offsetof(shared_buf_t, n_sink);
    e_read(addr, (void *) Mailbox.n_sink, sizeof(Mailbox.n_sink));
    for (i = 0; i < sizeof(expect)/sizeof(expect[0]); ++i) {
        for (j = 0; j < size; ++j) {
            sprintf(msg, "n_sink[%d][%d] should be %d, but %d is found", i, j,
                    expect[i][j], Mailbox.n_sink[i].array[j]);
            ok(expect[i][j] == Mailbox.n_sink[i].array[j], msg);
        }
    }
    printf("\n");
}

int main(int argc, char **argv) {
    // Epiphany_t _epiphany, *epiphany = &_epiphany;
    // int rv;

    // e_set_host_verbosity(2);
    // e_set_loader_verbosity(2);

    // rv = e_load("../epiphany/main.srec", 1, 1, 0);

    int i, j, n=0;
    int start = 0;
    int end = 15;
    puts("Host is running ...");
    fo = stdout;
    for (i=start; i<end; ++i) {
        Mailbox.core.go[i] = 1;
    }

    Mailbox.players = end;

    int input_data_size, output_data_size;
    if ( argc > 1 ) {
        input_data_size = atoi(argv[1]);
    } else {
        input_data_size = 64;
    }

    output_data_size = input_data_size;
    {
        char buffer[10];
        int n;
        FILE *f;
        i = 0;
        f = fopen("../in.txt", "r");
        while(NULL != fgets(buffer, sizeof(buffer), f)) {
            if (i<input_data_size) {
                n = atoi(buffer);
                Mailbox.n_source[0].array[i++] = n;
            } else {
                break;
            }
        }
        fclose(f);
        i = 0;
        f = fopen("../signed.txt", "r");
        while(NULL != fgets(buffer, sizeof(buffer), f)) {
            if (i<input_data_size) {
                n = atoi(buffer);
                Mailbox.n_source[1].array[i++] = n;
            } else {
                break;
            }
        }
        fclose(f);
        i = 0;
        f = fopen("../expect.txt", "r");
        while(NULL != fgets(buffer, sizeof(buffer), f)) {
            if (i<output_data_size) {
                n = atoi(buffer);
                expect[0][i++] = n;
            } else {
                break;
            }
        }
        fclose(f);
    }
    for (i = 0; i < sizeof(Mailbox.n_source)/sizeof(Mailbox.n_source[0]);
            ++i) {
        Mailbox.n_source[i].size = input_data_size;
        Mailbox.n_source[i].index = 0;
    }
    Mailbox.n_source[1].size = input_data_size/64;

    printf("size: %d\n", input_data_size);

    if (e_open((char *) servIP, eServLoaderPort)) {
        fprintf(fo, "\nERROR: Can't establish connection to E-SERVER!\n\n");
        exit(1);
    }

    // Dump everything to the board.
    addr = DRAM_BASE;
    e_write(addr, (void *) &Mailbox, sizeof(Mailbox));
    puts("Waiting for the board to finish...");
    sleep(1);

    while(1) {
        show_core_go();
        if (Mailbox.core.go[Mailbox.players-1] == 5) {
            break;
        }
        sleep(2);
        show_debug_info();
        printf("core is in %d, loop times: %d\n",
                Mailbox.core.go[Mailbox.players-1], n++);
    }

    puts("Read data from board");

    compare_n_sink(output_data_size);

    for (i = 0; i < end; ++i) {
        addr = DRAM_BASE + offsetof(shared_buf_t, core.clocks);
        e_read(addr, (void *) Mailbox.core.clocks, sizeof(Mailbox.core.clocks));
        addr = DRAM_BASE + offsetof(shared_buf_t, core.cycles);
        e_read(addr, (void *) Mailbox.core.cycles, sizeof(Mailbox.core.cycles));

        printf("%u, %u\n", Mailbox.core.cycles[i], Mailbox.core.clocks[i]);
        // printf("pending/clock for %d is %u/%u\n", i,
        // Mailbox.core.cycles[i], Mailbox.core.clocks[i]);
    }

    if (e_close()) {
        fprintf(fo, "\nERROR: Can't close connection to E-SERVER!\n\n");
        exit(1);
    }

    return 0;
}
