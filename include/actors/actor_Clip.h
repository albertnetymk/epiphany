#ifndef ACTOR_CLIP
#define ACTOR_CLIP

#include "util/communication.h"
typedef struct actor_Clip_struct {
    port_in   *I, *SIGNED;
    port_out  *O;
    int isz;
    int osz;
    void (*run) (struct actor_Clip_struct *self);
    void (*end) (struct actor_Clip_struct *self);
    bool (*not_finished) (struct actor_Clip_struct *self);
} actor_Clip;
void actor_Clip_init(actor_Clip *a);
#endif
