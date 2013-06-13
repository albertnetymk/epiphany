#include "util/sink.h"
#include "util/source.h"
#ifndef ACTOR_CLIP
#define ACTOR_CLIP

#include "util/communication.h"
typedef struct actor_Clip_source_sink_struct {
    port_in   *I, *SIGNED;
    port_out  *O;
    int isz;
    int osz;
    void (*run) (struct actor_Clip_source_sink_struct *self);
    void (*end) (struct actor_Clip_source_sink_struct *self);
    bool (*not_finished) (struct actor_Clip_source_sink_struct *self);
} actor_Clip_source_sink;
void actor_Clip_source_sink_init(actor_Clip_source_sink *a);
#endif
