#ifndef __MATMUL_H__
#define __MATMUL_H__

#define _Nchips 2                  // # of chips in operand matrix side
#define _Nside  4                  // # of cores in chip side
#define _Ncores (_Nside * _Nside)  // Num of cores = 16
#define _Score  16                  // side size of per-core sub-submatrix (max 32)
#define _Schip  (_Score * _Nside)  // side size of per-chip submatrix
#define _Smtx   (_Schip * _Nchips) // side size of operand matrix

#define _Nbanks 4                  // Num of SRAM banks on core

#define _BankP  0
#define _BankA  1
#define _BankB  2
#define _BankC  3
#define _PING   0
#define _PONG   1

typedef struct {
    int    coreID;
    int    corenum;
    int    row;
    int    col;

    void  *bank[_Nbanks][2];   // Ping Pong Bank local space pointer
    void  *tgt_bk[_Nbanks][2]; // Target Bank for matrix rotate in global space

    int    synch;     // Sync with horizontal peer core
    int    syncv;     // Sync with vertical peer core
    int   *tgt_synch; // ptr to synch of target neighbor
    int   *tgt_syncv; // ptr to syncv of target neighbor

    int    pingpong; // Ping-Pong bank select indicator

    int    count;
} core_t;


#include "util/types.h"

typedef struct {
    int    ready[_Ncores]; // Core is ready after reset
    int    go[_Ncores];    // Call for matmul function
    uint    cycles[_Ncores];
    uint    clocks[_Ncores];
} mbox_t;

typedef struct {
    uint index;
    uint size;
    int array[100];
} network_source;

typedef struct {
    uint index;
    int array[100];
} network_sink;

typedef struct {
    // Note: Since host is 64bit, alignment has to be addressed.
    int data_size;
    int players;
    int source[20];
    int sink[20];
    network_source n_source[2];
    network_sink n_sink[4];
    int debug_zero[20];
    int debug_one[20];
    int debug_two[20];
    int debug_three[20];
    int debug_four[20];
    int debug_five[20];
    // int debug[200];
    int debug_index[10];
    mbox_t core;
} shared_buf_t;

extern volatile shared_buf_t Mailbox;

#endif // __MATMUL_H__
