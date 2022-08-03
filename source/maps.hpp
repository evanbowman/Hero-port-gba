#pragma once

#include "number/int.h"



struct RoomData
{
    u8 tiles_[20][20];
};



const RoomData* load_room(int x, int y);
