#include <e_coreid.h>
#include "common.h"


actor_a instance_a;
volatile port_out out;
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    instance_a.out = e_address_from_coreid(mycoreid, (void *)&out);
    core0_main(e_address_from_coreid(mycoreid, (void *)&instance_a));
    return 0;
}
