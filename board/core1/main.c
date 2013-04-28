#include <e_coreid.h>
#include "common.h"
#include "flags.h"

actor_sink instance;
port_in in;
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo buffer;
#endif
#ifdef USE_DOUBLE_BUFFER
fifo b0, b1;
#endif

static inline void fetch_data(void)
{
    // TODO use mailbox
}

static sink_api_t api;
static inline sink_api_t *init(void *a)
{
    all.instance_sink = a;
    actor_sink_init(a);
    api.fetch_data = &fetch_data;
    return &api;
}

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
    instance.in = address_from_coreid(mycoreid, &in);
    core_sink_main(address_from_coreid(mycoreid, &instance), &init);
    return 0;
}
