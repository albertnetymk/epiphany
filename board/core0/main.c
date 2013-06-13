#include <e_coreid.h> 
#include "util/common.h"
#include "util/flags.h"
actor_RowSort_source instance_RowSort;
port_in ROW;
port_out Y0;
port_in *Y0_dests[1];
port_out Y1;
port_in *Y1_dests[1];
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo ROW_buffer;
fifo Y0_buffer;
dma_cfg dma0;
fifo Y1_buffer;
dma_cfg dma1;
 #endif
#ifdef USE_DOUBLE_BUFFER
fifo ROW_b0;
fifo ROW_b1;
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
    all.instance_RowSort = a;
    actor_RowSort_source_init(a);
    api.run = (run_t *)all.instance_RowSort->run;
    api.end = (end_t *)all.instance_RowSort->end;
    api.not_finished = (not_finished_t *)all.instance_RowSort->not_finished;
    return &api;
}
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    Y0.dests = address_from_coreid(mycoreid, &Y0_dests);
    Y1.dests = address_from_coreid(mycoreid, &Y1_dests);
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
    ROW.buffer = address_from_coreid(mycoreid, &ROW_buffer);
    Y0_buffer.dma = &dma0;
    Y0.buffer = address_from_coreid(mycoreid, &Y0_buffer);
    Y1_buffer.dma = &dma1;
    Y1.buffer = address_from_coreid(mycoreid, &Y1_buffer);
#endif
#ifdef USE_DOUBLE_BUFFER
    ROW.buffers[0] = address_from_coreid(mycoreid, &ROW_b0);
    ROW.buffers[1] = address_from_coreid(mycoreid, &ROW_b1);
    Y0_b0.dma = &dma00;
    Y0_b1.dma = &dma01;
    Y0.buffers[0] = address_from_coreid(mycoreid, &Y0_b0);
    Y0.buffers[1] = address_from_coreid(mycoreid, &Y0_b1);
    Y1_b0.dma = &dma10;
    Y1_b1.dma = &dma11;
    Y1.buffers[0] = address_from_coreid(mycoreid, &Y1_b0);
    Y1.buffers[1] = address_from_coreid(mycoreid, &Y1_b1);
#endif
    instance_RowSort.ROW = address_from_coreid(mycoreid, &ROW);
    instance_RowSort.Y0 = address_from_coreid(mycoreid, &Y0);
    instance_RowSort.Y1 = address_from_coreid(mycoreid, &Y1);
   core_main(address_from_coreid(mycoreid, &instance_RowSort), &init);
   return 0;
}
