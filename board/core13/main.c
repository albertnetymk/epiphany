#include <e_coreid.h> 
#include "util/common.h"
#include "util/flags.h"
actor_Retranspose instance_Retranspose;
port_in X0;
port_in X1;
port_in X2;
port_in X3;
port_out Y;
port_in *Y_dests[1];
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo X0_buffer;
fifo X1_buffer;
fifo X2_buffer;
fifo X3_buffer;
fifo Y_buffer;
dma_cfg dma0;
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
fifo Y_b0;
fifo Y_b1;
dma_cfg dma00, dma01;
#endif
static api_t api;
static inline api_t *init(void *a)
{
    all.instance_Retranspose = a;
    actor_Retranspose_init(a);
    api.run = (run_t *)all.instance_Retranspose->run;
    api.end = (end_t *)all.instance_Retranspose->end;
    api.not_finished = (not_finished_t *)all.instance_Retranspose->not_finished;
    return &api;
}
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    Y.dests = address_from_coreid(mycoreid, &Y_dests);
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
    X0.buffer = address_from_coreid(mycoreid, &X0_buffer);
    X1.buffer = address_from_coreid(mycoreid, &X1_buffer);
    X2.buffer = address_from_coreid(mycoreid, &X2_buffer);
    X3.buffer = address_from_coreid(mycoreid, &X3_buffer);
    Y_buffer.dma = &dma0;
    Y.buffer = address_from_coreid(mycoreid, &Y_buffer);
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
    Y_b0.dma = &dma00;
    Y_b1.dma = &dma01;
    Y.buffers[0] = address_from_coreid(mycoreid, &Y_b0);
    Y.buffers[1] = address_from_coreid(mycoreid, &Y_b1);
#endif
    instance_Retranspose.X0 = address_from_coreid(mycoreid, &X0);
    instance_Retranspose.X1 = address_from_coreid(mycoreid, &X1);
    instance_Retranspose.X2 = address_from_coreid(mycoreid, &X2);
    instance_Retranspose.X3 = address_from_coreid(mycoreid, &X3);
    instance_Retranspose.Y = address_from_coreid(mycoreid, &Y);
   core_main(address_from_coreid(mycoreid, &instance_Retranspose), &init);
   return 0;
}
