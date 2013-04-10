#include "common.h"

actor_b instance_b;
port_in in;
int main(void) {
    instance_b.in = &in;
    core1_main(&instance_b);
    return 0;
}
