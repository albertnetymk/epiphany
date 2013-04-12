#include <e_coreid.h>
#include "common.h"

actor_b instance_b;
volatile port_in in;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    instance_b.in = e_address_from_coreid(mycoreid, (void *)&in);
    core1_main(e_address_from_coreid(mycoreid, (void *)&instance_b));
    return 0;
}
