#include "util/source.h"
#ifndef ACTOR_CLIP
#define ACTOR_CLIP

#include "util/communication.h"
typedef struct actor_Clip_source_struct {
    port_in   *I, *SIGNED;
    port_out  *O;
    int isz;
    int osz;
    void (*run) (struct actor_Clip_source_struct *self);
    void (*end) (struct actor_Clip_source_struct *self);
    bool (*not_finished) (struct actor_Clip_source_struct *self);
} actor_Clip_source;
void actor_Clip_source_init(actor_Clip_source *a);
#endif
