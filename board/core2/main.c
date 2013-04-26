#include <e_coreid.h>
#include "common.h"
#include "flags.h"

// it's not necessary to use global address for non shared resources.
actor_double instance_double;
port_in in;
port_out out;
port_in *dests[1];
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo in_buffer;
fifo out_buffer;
dma_cfg dma;
#endif
#ifdef USE_DOUBLE_BUFFER
fifo in_b0, in_b1;
fifo out_b0, out_b1;
dma_cfg dma0, dma1;
#endif

static api_t api;
static inline api_t *init(void *a)
{
    all.instance_double1 = a;
    actor_double_init(a);
    api.run = (run_t *)all.instance_double1->run;
    api.end = (end_t *)all.instance_double1->end;
    api.not_finished = (not_finished_t *)all.instance_double1->not_finished;
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
    in.buffer = address_from_coreid(mycoreid, &in_buffer);
#endif
#ifdef USE_DOUBLE_BUFFER
    out_b0.dma = &dma0;
    out_b1.dma = &dma1;
    out.buffers[0] = address_from_coreid(mycoreid, &out_b0);
    out.buffers[1] = address_from_coreid(mycoreid, &out_b1);
    in.buffers[0] = address_from_coreid(mycoreid, &in_b0);
    in.buffers[1] = address_from_coreid(mycoreid, &in_b1);
#endif
    instance_double.in = address_from_coreid(mycoreid, &in);
    instance_double.out = address_from_coreid(mycoreid, &out);
    core_main(address_from_coreid(mycoreid, &instance_double), &init);
    return 0;
}
