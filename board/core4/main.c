#include <e_coreid.h>
#include "util/common.h"
#include "util/flags.h"

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
#ifdef USE_MULTIPLE_BUFFER
fifo in1_buffers[BUFFER_NUMBER];
fifo in2_buffers[BUFFER_NUMBER];
fifo out_buffers[BUFFER_NUMBER];
dma_cfg dma[BUFFER_NUMBER];
#endif

static api_t api;
static inline api_t *init(void *a)
{
    all.instance_add = a;
    actor_add_init(a);
    api.run = (run_t *)all.instance_add->run;
    api.end = (end_t *)all.instance_add->end;
    api.not_finished = (not_finished_t *)all.instance_add->not_finished;
    return &api;
}
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
#ifdef USE_MULTIPLE_BUFFER
    int i;
    for (i = 0; i < BUFFER_NUMBER; ++i) {
        out_buffers[i].dma = &dma[i];
        out.buffers[i] = address_from_coreid(mycoreid, &out_buffers[i]);
        in1.buffers[i] = address_from_coreid(mycoreid, &in1_buffers[i]);
        in2.buffers[i] = address_from_coreid(mycoreid, &in2_buffers[i]);
    }
#endif
    instance_add.in1 = address_from_coreid(mycoreid, &in1);
    instance_add.in2 = address_from_coreid(mycoreid, &in2);
    instance_add.out = address_from_coreid(mycoreid, &out);
    core_main(address_from_coreid(mycoreid, &instance_add), &init);
    return 0;
}
