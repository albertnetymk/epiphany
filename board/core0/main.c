#include <e_coreid.h>
#include "common.h"
#include "flags.h"

// it's not necessary to use global address for non shared resources.
actor_a instance_a;
port_out out;
port_in *dests[2];
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
    instance_a.out = address_from_coreid(mycoreid, &out);
    core_source_main(address_from_coreid(mycoreid, &instance_a));
    return 0;
}
