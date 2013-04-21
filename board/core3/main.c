#include <e_coreid.h>
#include "common.h"
#include "flags.h"

// it's not necessary to use global address for non shared resources.
#ifdef USE_DESTINATION_BUFFER
actor_double instance_double;
port_in in;
port_out out;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    // this one to local?
    instance_double.in = address_from_coreid(mycoreid, &in);
    instance_double.out = address_from_coreid(mycoreid, &out);
    core3_main(address_from_coreid(mycoreid, &instance_double));
    return 0;
}
#endif

#ifdef USE_BOTH_BUFFER
actor_double instance_double;
port_in in;
port_out out;
fifo buffer;
dma_cfg dma;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    buffer.dma = &dma;
    out.buffer = address_from_coreid(mycoreid, &buffer);
    // local
    instance_double.in = address_from_coreid(mycoreid, &in);
    instance_double.out = address_from_coreid(mycoreid, &out);
    core3_main(e_doubleddress_from_coreid(mycoreid, &instance_double));
    return 0;
}
#endif

#ifdef USE_DOUBLE_BUFFER
actor_double instance_double;
port_in in;
port_out out;
fifo b0, b1;
dma_cfg dma0, dma1;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    b0.dma = &dma0;
    b1.dma = &dma1;
    out.buffers[0] = address_from_coreid(mycoreid, &b0);
    out.buffers[1] = address_from_coreid(mycoreid, &b1);
    // local
    instance_double.in = address_from_coreid(mycoreid, &in);
    instance_double.out = address_from_coreid(mycoreid, &out);
    core3_main(address_from_coreid(mycoreid, &instance_double));
    return 0;
}
#endif