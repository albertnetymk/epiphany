#include <e_coreid.h>
#include "common.h"
#include "flags.h"

actor_add instance_add;
port_in in1;
port_in in2;
port_out out;
port_in *dests[1];
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo in1_buffer;
fifo in2_buffer;
fifo out_buffer;
dma_cfg dma;
#endif
#ifdef USE_DOUBLE_BUFFER
fifo in1_b0, in1_b1;
fifo in2_b0, in2_b1;
fifo out_b0, out_b1;
dma_cfg dma0, dma1;
#endif

int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    out.dests = address_from_coreid(mycoreid, &dests);
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
    out_buffer.dma = &dma;
    out.buffer = address_from_coreid(mycoreid, &out_buffer);
    in1.buffer = address_from_coreid(mycoreid, &in1_buffer);
    in2.buffer = address_from_coreid(mycoreid, &in2_buffer);
#endif
#ifdef USE_DOUBLE_BUFFER
    out_b0.dma = &dma0;
    out_b1.dma = &dma1;
    out.buffers[0] = address_from_coreid(mycoreid, &out_b0);
    out.buffers[1] = address_from_coreid(mycoreid, &out_b1);
    in1.buffers[0] = address_from_coreid(mycoreid, &in1_b0);
    in1.buffers[1] = address_from_coreid(mycoreid, &in1_b1);
    in2.buffers[0] = address_from_coreid(mycoreid, &in2_b0);
    in2.buffers[1] = address_from_coreid(mycoreid, &in2_b1);
#endif
    instance_add.in1 = address_from_coreid(mycoreid, &in1);
    instance_add.in2 = address_from_coreid(mycoreid, &in2);
    instance_add.out = address_from_coreid(mycoreid, &out);
    core4_main(address_from_coreid(mycoreid, &instance_add));
    return 0;
}
