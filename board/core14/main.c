#include <e_coreid.h> 
#include "util/common.h"
#include "util/flags.h"
actor_Clip instance_Clip;
port_in I;
port_in SIGNED;
port_out O;
port_in *O_dests[1];
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
fifo I_buffer;
fifo SIGNED_buffer;
fifo O_buffer;
dma_cfg dma0;
 #endif
#ifdef USE_DOUBLE_BUFFER
fifo I_b0;
fifo I_b1;
fifo SIGNED_b0;
fifo SIGNED_b1;
fifo O_b0;
fifo O_b1;
dma_cfg dma00, dma01;
#endif
static api_t api;
static inline api_t *init(void *a)
{
    all.instance_Clip = a;
    actor_Clip_init(a);
    api.run = (run_t *)all.instance_Clip->run;
    api.end = (end_t *)all.instance_Clip->end;
    api.not_finished = (not_finished_t *)all.instance_Clip->not_finished;
    return &api;
}
int main(void) {
    e_coreid_t mycoreid = e_get_coreid();
    O.dests = address_from_coreid(mycoreid, &O_dests);
#ifdef USE_DESTINATION_BUFFER
#endif
#ifdef USE_BOTH_BUFFER
    I.buffer = address_from_coreid(mycoreid, &I_buffer);
    SIGNED.buffer = address_from_coreid(mycoreid, &SIGNED_buffer);
    O_buffer.dma = &dma0;
    O.buffer = address_from_coreid(mycoreid, &O_buffer);
#endif
#ifdef USE_DOUBLE_BUFFER
    I.buffers[0] = address_from_coreid(mycoreid, &I_b0);
    I.buffers[1] = address_from_coreid(mycoreid, &I_b1);
    SIGNED.buffers[0] = address_from_coreid(mycoreid, &SIGNED_b0);
    SIGNED.buffers[1] = address_from_coreid(mycoreid, &SIGNED_b1);
    O_b0.dma = &dma00;
    O_b1.dma = &dma01;
    O.buffers[0] = address_from_coreid(mycoreid, &O_b0);
    O.buffers[1] = address_from_coreid(mycoreid, &O_b1);
#endif
    instance_Clip.I = address_from_coreid(mycoreid, &I);
    instance_Clip.SIGNED = address_from_coreid(mycoreid, &SIGNED);
    instance_Clip.O = address_from_coreid(mycoreid, &O);
   core_main(address_from_coreid(mycoreid, &instance_Clip), &init);
   return 0;
}
