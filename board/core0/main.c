#include <e_coreid.h>
#include "common.h"
#include "flags.h"

// it's not necessary to use global address for non shared resources.
#ifdef USE_DESTINATION_BUFFER
actor_a instance_a;
port_out out;
port_in *dests[2];
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    out.dests = &dests;
    // this one to local?
    instance_a.out = address_from_coreid(mycoreid, &out);
    core0_main(address_from_coreid(mycoreid, &instance_a));
    return 0;
}
#endif

#ifdef USE_BOTH_BUFFER
actor_a instance_a;
port_out out;
port_in *dests[1];
fifo buffer;
dma_cfg dma;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    buffer.dma = &dma;
    out.buffer = address_from_coreid(mycoreid, &buffer);
    out.dests = &dests;
    // local
    instance_a.out = address_from_coreid(mycoreid, &out);
    core0_main(e_address_from_coreid(mycoreid, &instance_a));
    return 0;
}
#endif

#ifdef USE_DOUBLE_BUFFER
actor_a instance_a;
port_out out;
port_in *dests[1];
fifo b0, b1;
dma_cfg dma0, dma1;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    b0.dma = &dma0;
    b1.dma = &dma1;
    out.buffers[0] = address_from_coreid(mycoreid, &b0);
    out.buffers[1] = address_from_coreid(mycoreid, &b1);
    out.dests = &dests;
    // local
    instance_a.out = address_from_coreid(mycoreid, &out);
    core0_main(address_from_coreid(mycoreid, &instance_a));
    return 0;
}
#endif
