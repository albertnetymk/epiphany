#ifndef __COMMON_BUFFERS_H__
#define __COMMON_BUFFERS_H__
#include "matmul.h"
#ifdef __HOST__
#define SECTION(x)
#else // __HOST__
#include "actors.h"
#include <e_common.h>
#endif // __HOST__

volatile shared_buf_t Mailbox SECTION(".shared_dram");

#ifndef __HOST__
volatile actors all SECTION(".shared_dram");
#endif

#endif // __COMMON_BUFFERS_H__
