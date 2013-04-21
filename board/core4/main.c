#include <e_coreid.h>
#include "common.h"
#include "flags.h"

#ifdef USE_DESTINATION_BUFFER
actor_add instance_add;
port_in in1;
port_in in2;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    instance_add.in1 = address_from_coreid(mycoreid, &in1);
    instance_add.in2 = address_from_coreid(mycoreid, &in2);
    core4_main(address_from_coreid(mycoreid, &instance_add));
    return 0;
}
#endif

#ifdef USE_BOTH_BUFFER
actor_add instance_add;
port_in in1;
port_in in2;
fifo buffer;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    in.buffer = address_from_coreid(mycoreid, &buffer);
    // local
    instance_add.in1 = address_from_coreid(mycoreid, &in1);
    instance_add.in2 = address_from_coreid(mycoreid, &in2);
    core4_main(address_from_coreid(mycoreid, &instance_add));
    return 0;
}
#endif

#ifdef USE_DOUBLE_BUFFER
actor_add instance_add;
port_in in1;
port_in in2;
fifo b0, b1;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    in.buffers[0] = address_from_coreid(mycoreid, &b0);
    in.buffers[1] = address_from_coreid(mycoreid, &b1);
    // local
    instance_add.in1 = address_from_coreid(mycoreid, &in1);
    instance_add.in2 = address_from_coreid(mycoreid, &in2);
    core4_main(address_from_coreid(mycoreid, &instance_add));
    return 0;
}
#endif
