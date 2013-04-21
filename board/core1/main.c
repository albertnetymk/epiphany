#include <e_coreid.h>
#include "common.h"
#include "flags.h"

actor_b instance_b;
port_in in;
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo buffer;
#endif
#ifdef USE_DOUBLE_BUFFER
fifo b0, b1;
#endif

int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
    in.buffer = address_from_coreid(mycoreid, &buffer);
#endif
#ifdef USE_DOUBLE_BUFFER
    in.buffers[0] = address_from_coreid(mycoreid, &b0);
    in.buffers[1] = address_from_coreid(mycoreid, &b1);
#endif
    // this one to local?
    instance_b.in = address_from_coreid(mycoreid, &in);
    core1_main(address_from_coreid(mycoreid, &instance_b));
    return 0;
}
