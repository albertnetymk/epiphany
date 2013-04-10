#include "common.h"

actor_a instance_a;
port_out out;
int main(void) {
    instance_a.out = &out;
    core0_main(&instance_a);
    return 0;
}
