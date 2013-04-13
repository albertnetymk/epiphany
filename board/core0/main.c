#include <e_coreid.h>
#include "common.h"
#include "flags.h"

void *address_from_coreid(e_coreid_t coreid, volatile void *ptr)
{
    return e_address_from_coreid(coreid, (void *)ptr);
}

#ifdef USE_DESTINATION_BUFFER
actor_a instance_a;
port_out out;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    instance_a.out = address_from_coreid(mycoreid, &out);
    core0_main(address_from_coreid(mycoreid, &instance_a));
    return 0;
}
