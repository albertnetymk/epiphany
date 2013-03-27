#ifndef __COMMON_BUFFERS_H__
#define __COMMON_BUFFERS_H__
#include "matmul.h"
#include "actors.h"
#ifdef __HOST__
#define SECTION(x)
#else // __HOST__
#include <e_common.h>
#endif // __HOST__

volatile shared_buf_t Mailbox SECTION(".shared_dram");
volatile actors all SECTION(".shared_dram");

#endif // __COMMON_BUFFERS_H__
