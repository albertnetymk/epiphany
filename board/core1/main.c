#include <e_coreid.h>
#include "common.h"
#include "flags.h"

#ifdef USE_DESTINATION_BUFFER
actor_b instance_b;
port_in in;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    instance_b.in = address_from_coreid(mycoreid, &in);
    core1_main(address_from_coreid(mycoreid, &instance_b));
    return 0;
}
#endif

#ifdef USE_BOTH_BUFFER
actor_b instance_b;
port_in in;
fifo buffer;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    in.buffer = address_from_coreid(mycoreid, &buffer);
    instance_b.in = address_from_coreid(mycoreid, &in);
    core0_main(address_from_coreid(mycoreid, &instance_b));
    return 0;
}
#endif
