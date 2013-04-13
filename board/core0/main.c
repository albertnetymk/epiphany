#include <e_coreid.h>
#include "common.h"
#include "flags.h"

#ifdef USE_DESTINATION_BUFFER
actor_a instance_a;
port_out out;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    instance_a.out = address_from_coreid(mycoreid, &out);
    core0_main(address_from_coreid(mycoreid, &instance_a));
    return 0;
}
#endif

#ifdef USE_BOTH_BUFFER
actor_a instance_a;
port_out out;
fifo buffer;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    out.buffer = address_from_coreid(mycoreid, &buffer);
    instance_a.out = address_from_coreid(mycoreid, &out);
    core0_main(e_address_from_coreid(mycoreid, &instance_a));
    return 0;
}
#endif
