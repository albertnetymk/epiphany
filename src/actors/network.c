#include "util/common.h"
void connect_network()
{
    connect(all.instance_Scale->Y0, all.instance_Combine->X0);
    connect(all.instance_Scale->Y1, all.instance_Combine->X1);
    connect(all.instance_Scale->Y2, all.instance_Combine->X2);
    connect(all.instance_Scale->Y3, all.instance_Combine->X3);
    connect(all.instance_Combine->Y0, all.instance_ShuffleFly->X0);
    connect(all.instance_Combine->Y1, all.instance_ShuffleFly->X1);
    connect(all.instance_ShuffleFly->Y0, all.instance_Shuffle->X0);
    connect(all.instance_ShuffleFly->Y1, all.instance_Shuffle->X1);
    connect(all.instance_ShuffleFly->Y2, all.instance_Shuffle->X2);
    connect(all.instance_ShuffleFly->Y3, all.instance_Shuffle->X3);
    connect(all.instance_Shuffle->Y0, all.instance_Final->X0);
    connect(all.instance_Shuffle->Y1, all.instance_Final->X1);
    connect(all.instance_Shuffle->Y2, all.instance_Final->X2);
    connect(all.instance_Shuffle->Y3, all.instance_Final->X3);
}
