#include <e_coreid.h>
#include "common.h"
#include "flags.h"

// it's not necessary to use global address for non shared resources.
actor_source instance;
port_out out;
port_in *dests[1];
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo buffer;
dma_cfg dma;
#endif
#ifdef USE_DOUBLE_BUFFER
fifo b0, b1;
dma_cfg dma0, dma1;
#endif

static void connect_network()
{
    connect(all.instance_source->out, all.instance_distributor->in);
    connect(all.instance_distributor->out, all.instance_double1->in);
    connect(all.instance_distributor->out, all.instance_double2->in);
    connect(all.instance_double1->out, all.instance_add->in1);
    connect(all.instance_double2->out, all.instance_add->in2);
    connect(all.instance_add->out, all.instance_sink->in);
}

static void prepare_data()
{
    // TODO access mailbox
}

static source_api_t api;
static inline source_api_t *init(void *a)
{
    all.instance_source = a;
    actor_source_init(a);
    api.connect_network = &connect_network;
    api.prepare_data = &prepare_data;
    api.end = (end_t *)all.instance_source->end;
    return &api;
}
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    out.dests = address_from_coreid(mycoreid, &dests);
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
    buffer.dma = &dma;
    out.buffer = address_from_coreid(mycoreid, &buffer);
#endif
#ifdef USE_DOUBLE_BUFFER
    b0.dma = &dma0;
    b1.dma = &dma1;
    out.buffers[0] = address_from_coreid(mycoreid, &b0);
    out.buffers[1] = address_from_coreid(mycoreid, &b1);
#endif
    // this one to local?
    instance.out = address_from_coreid(mycoreid, &out);
    core_source_main(address_from_coreid(mycoreid, &instance), &init);
    return 0;
}
