#include <e_coreid.h> 
#include "util/common.h"
#include "util/flags.h"
actor_Transpose instance_Transpose;
port_in X0;
port_in X1;
port_in X2;
port_in X3;
port_out Y0;
port_in *Y0_dests[1];
port_out Y1;
port_in *Y1_dests[1];
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
#endif
static api_t api;
static inline api_t *init(void *a)
{
    all.instance_Transpose = a;
    actor_Transpose_init(a);
    api.run = (run_t *)all.instance_Transpose->run;
    api.end = (end_t *)all.instance_Transpose->end;
    api.not_finished = (not_finished_t *)all.instance_Transpose->not_finished;
    return &api;
}
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    Y0.dests = address_from_coreid(mycoreid, &Y0_dests);
    Y1.dests = address_from_coreid(mycoreid, &Y1_dests);
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
#endif
    instance_Transpose.X0 = address_from_coreid(mycoreid, &X0);
    instance_Transpose.X1 = address_from_coreid(mycoreid, &X1);
    instance_Transpose.X2 = address_from_coreid(mycoreid, &X2);
    instance_Transpose.X3 = address_from_coreid(mycoreid, &X3);
    instance_Transpose.Y0 = address_from_coreid(mycoreid, &Y0);
    instance_Transpose.Y1 = address_from_coreid(mycoreid, &Y1);
   core_main(address_from_coreid(mycoreid, &instance_Transpose), &init);
   return 0;
}
