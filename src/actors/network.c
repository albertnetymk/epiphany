#include "util/common.h"
void connect_network()
{
    connect(all.instance_RowSort->Y0, all.idctRow_instance_Scale->X0);
    connect(all.instance_RowSort->Y1, all.idctRow_instance_Scale->X1);

    connect(all.idctRow_instance_Scale->Y0, all.idctRow_instance_Combine->X0);
    connect(all.idctRow_instance_Scale->Y1, all.idctRow_instance_Combine->X1);
    connect(all.idctRow_instance_Scale->Y2, all.idctRow_instance_Combine->X2);
    connect(all.idctRow_instance_Scale->Y3, all.idctRow_instance_Combine->X3);
    connect(all.idctRow_instance_Combine->Y0, all.idctRow_instance_ShuffleFly->X0);
    connect(all.idctRow_instance_Combine->Y1, all.idctRow_instance_ShuffleFly->X1);
    connect(all.idctRow_instance_ShuffleFly->Y0, all.idctRow_instance_Shuffle->X0);
    connect(all.idctRow_instance_ShuffleFly->Y1, all.idctRow_instance_Shuffle->X1);
    connect(all.idctRow_instance_ShuffleFly->Y2, all.idctRow_instance_Shuffle->X2);
    connect(all.idctRow_instance_ShuffleFly->Y3, all.idctRow_instance_Shuffle->X3);
    connect(all.idctRow_instance_Shuffle->Y0, all.idctRow_instance_Final->X0);
    connect(all.idctRow_instance_Shuffle->Y1, all.idctRow_instance_Final->X1);
    connect(all.idctRow_instance_Shuffle->Y2, all.idctRow_instance_Final->X2);
    connect(all.idctRow_instance_Shuffle->Y3, all.idctRow_instance_Final->X3);

    connect(all.instance_Transpose->Y0, all.idctCol_instance_Scale->X0);
    connect(all.instance_Transpose->Y1, all.idctCol_instance_Scale->X1);

    connect(all.idctCol_instance_Scale->Y0, all.idctCol_instance_Combine->X0);
    connect(all.idctCol_instance_Scale->Y1, all.idctCol_instance_Combine->X1);
    connect(all.idctCol_instance_Scale->Y2, all.idctCol_instance_Combine->X2);
    connect(all.idctCol_instance_Scale->Y3, all.idctCol_instance_Combine->X3);
    connect(all.idctCol_instance_Combine->Y0, all.idctCol_instance_ShuffleFly->X0);
    connect(all.idctCol_instance_Combine->Y1, all.idctCol_instance_ShuffleFly->X1);
    connect(all.idctCol_instance_ShuffleFly->Y0, all.idctCol_instance_Shuffle->X0);
    connect(all.idctCol_instance_ShuffleFly->Y1, all.idctCol_instance_Shuffle->X1);
    connect(all.idctCol_instance_ShuffleFly->Y2, all.idctCol_instance_Shuffle->X2);
    connect(all.idctCol_instance_ShuffleFly->Y3, all.idctCol_instance_Shuffle->X3);
    connect(all.idctCol_instance_Shuffle->Y0, all.idctCol_instance_Final->X0);
    connect(all.idctCol_instance_Shuffle->Y1, all.idctCol_instance_Final->X1);
    connect(all.idctCol_instance_Shuffle->Y2, all.idctCol_instance_Final->X2);
    connect(all.idctCol_instance_Shuffle->Y3, all.idctCol_instance_Final->X3);

    connect(all.idctCol_instance_Final->Y0, all.instance_Shift->X0);
    connect(all.idctCol_instance_Final->Y1, all.instance_Shift->X1);
    connect(all.idctCol_instance_Final->Y2, all.instance_Shift->X2);
    connect(all.idctCol_instance_Final->Y3, all.instance_Shift->X3);

    connect(all.instance_Shift->Y0, all.instance_Retranspose->X0);
    connect(all.instance_Shift->Y1, all.instance_Retranspose->X1);
    connect(all.instance_Shift->Y2, all.instance_Retranspose->X2);
    connect(all.instance_Shift->Y3, all.instance_Retranspose->X3);

    connect(all.instance_Retranspose->Y, all.instance_Clip->I);
}
