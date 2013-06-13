// ********* Generate Code form flatten NL Test.h *********/

#ifndef _ACTORS_H
#define _ACTORS_H

#include "actors/actor_RowSort.h"
#include "actors/actor_Scale.h"
#include "actors/actor_Combine.h"
#include "actors/actor_ShuffleFly.h"
#include "actors/actor_Shuffle.h"
#include "actors/actor_Final.h"
#include "actors/actor_Transpose.h"
#include "actors/actor_Scale.h"
#include "actors/actor_Combine.h"
#include "actors/actor_ShuffleFly.h"
#include "actors/actor_Shuffle.h"
#include "actors/actor_Final.h"
#include "actors/actor_Shift.h"
#include "actors/actor_Retranspose.h"
#include "actors/actor_Clip.h"

typedef struct actors_struct {
    actor_RowSort *instance_RowSort;
    actor_Scale *idctRow_instance_Scale;
    actor_Combine *idctRow_instance_Combine;
    actor_ShuffleFly *idctRow_instance_ShuffleFly;
    actor_Shuffle *idctRow_instance_Shuffle;
    actor_Final *idctRow_instance_Final;
    actor_Transpose *instance_Transpose;
    actor_Scale *idctCol_instance_Scale;
    actor_Combine *idctCol_instance_Combine;
    actor_ShuffleFly *idctCol_instance_ShuffleFly;
    actor_Shuffle *idctCol_instance_Shuffle;
    actor_Final *idctCol_instance_Final;
    actor_Shift *instance_Shift;
    actor_Retranspose *instance_Retranspose;
    actor_Clip *instance_Clip;
} actors;
#endif
