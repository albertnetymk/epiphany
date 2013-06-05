#include <e_coreid.h> 
#include "util/common.h"
#include "util/flags.h"
actor_Shuffle instance_Shuffle;
port_in X0;
port_in X1;
port_in X2;
port_in X3;
port_out Y0;
port_in *Y0_dests[1];
port_out Y1;
port_in *Y1_dests[1];
port_out Y2;
port_in *Y2_dests[1];
port_out Y3;
port_in *Y3_dests[1];
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo X0_buffer;
fifo X1_buffer;
fifo X2_buffer;
fifo X3_buffer;
fifo Y0_buffer;
dma_cfg dma0;
fifo Y1_buffer;
dma_cfg dma1;
fifo Y2_buffer;
dma_cfg dma2;
fifo Y3_buffer;
dma_cfg dma3;
 #endif
#ifdef USE_DOUBLE_BUFFER
fifo X0_b0;
fifo X0_b1;
fifo X1_b0;
fifo X1_b1;
fifo X2_b0;
fifo X2_b1;
fifo X3_b0;
fifo X3_b1;
fifo Y0_b0;
fifo Y0_b1;
dma_cfg dma00, dma01;
fifo Y1_b0;
fifo Y1_b1;
dma_cfg dma10, dma11;
fifo Y2_b0;
fifo Y2_b1;
dma_cfg dma20, dma21;
fifo Y3_b0;
fifo Y3_b1;
dma_cfg dma30, dma31;
#endif
static api_t api;
static inline api_t *init(void *a)
{
    all.instance_Shuffle = a;
    actor_Shuffle_init(a);
    api.run = (run_t *)all.instance_Shuffle->run;
    api.end = (end_t *)all.instance_Shuffle->end;
    api.not_finished = (not_finished_t *)all.instance_Shuffle->not_finished;
    return &api;
}
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    Y0.dests = address_from_coreid(mycoreid, &Y0_dests);
    Y1.dests = address_from_coreid(mycoreid, &Y1_dests);
    Y2.dests = address_from_coreid(mycoreid, &Y2_dests);
    Y3.dests = address_from_coreid(mycoreid, &Y3_dests);
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
    X0.buffer = address_from_coreid(mycoreid, &X0_buffer);
    X1.buffer = address_from_coreid(mycoreid, &X1_buffer);
    X2.buffer = address_from_coreid(mycoreid, &X2_buffer);
    X3.buffer = address_from_coreid(mycoreid, &X3_buffer);
    Y0_buffer.dma = &dma0;
    Y0.buffer = address_from_coreid(mycoreid, &Y0_buffer);
    Y1_buffer.dma = &dma1;
    Y1.buffer = address_from_coreid(mycoreid, &Y1_buffer);
    Y2_buffer.dma = &dma2;
    Y2.buffer = address_from_coreid(mycoreid, &Y2_buffer);
    Y3_buffer.dma = &dma3;
    Y3.buffer = address_from_coreid(mycoreid, &Y3_buffer);
#endif
#ifdef USE_DOUBLE_BUFFER
    X0.buffers[0] = address_from_coreid(mycoreid, &X0_b0);
    X0.buffers[1] = address_from_coreid(mycoreid, &X0_b1);
    X1.buffers[0] = address_from_coreid(mycoreid, &X1_b0);
    X1.buffers[1] = address_from_coreid(mycoreid, &X1_b1);
    X2.buffers[0] = address_from_coreid(mycoreid, &X2_b0);
    X2.buffers[1] = address_from_coreid(mycoreid, &X2_b1);
    X3.buffers[0] = address_from_coreid(mycoreid, &X3_b0);
    X3.buffers[1] = address_from_coreid(mycoreid, &X3_b1);
    Y0_b0.dma = &dma00;
    Y0_b1.dma = &dma01;
    Y0.buffers[0] = address_from_coreid(mycoreid, &Y0_b0);
    Y0.buffers[1] = address_from_coreid(mycoreid, &Y0_b1);
    Y1_b0.dma = &dma10;
    Y1_b1.dma = &dma11;
    Y1.buffers[0] = address_from_coreid(mycoreid, &Y1_b0);
    Y1.buffers[1] = address_from_coreid(mycoreid, &Y1_b1);
    Y2_b0.dma = &dma20;
    Y2_b1.dma = &dma21;
    Y2.buffers[0] = address_from_coreid(mycoreid, &Y2_b0);
    Y2.buffers[1] = address_from_coreid(mycoreid, &Y2_b1);
    Y3_b0.dma = &dma30;
    Y3_b1.dma = &dma31;
    Y3.buffers[0] = address_from_coreid(mycoreid, &Y3_b0);
    Y3.buffers[1] = address_from_coreid(mycoreid, &Y3_b1);
#endif
    instance_Shuffle.X0 = address_from_coreid(mycoreid, &X0);
    instance_Shuffle.X1 = address_from_coreid(mycoreid, &X1);
    instance_Shuffle.X2 = address_from_coreid(mycoreid, &X2);
    instance_Shuffle.X3 = address_from_coreid(mycoreid, &X3);
    instance_Shuffle.Y0 = address_from_coreid(mycoreid, &Y0);
    instance_Shuffle.Y1 = address_from_coreid(mycoreid, &Y1);
    instance_Shuffle.Y2 = address_from_coreid(mycoreid, &Y2);
    instance_Shuffle.Y3 = address_from_coreid(mycoreid, &Y3);
   core_main(address_from_coreid(mycoreid, &instance_Shuffle), &init);
   return 0;
}
