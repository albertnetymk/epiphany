// ********* Generate Code form flatten NL idct1d.h *********/

#ifndef _ACTORS_H
#define _ACTORS_H

#include "actors/actor_Scale.h"
#include "actors/actor_Combine.h"
#include "actors/actor_ShuffleFly.h"
#include "actors/actor_Shuffle.h"
#include "actors/actor_Final.h"

typedef struct actors_struct {
    actor_Scale *instance_Scale;
    actor_Combine *instance_Combine;
    actor_ShuffleFly *instance_ShuffleFly;
    actor_Shuffle *instance_Shuffle;
    actor_Final *instance_Final;
} actors;
#endif
