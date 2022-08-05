#pragma once

#include "number/int.h"



struct RoomData
{
    u8 zone_;
    u8 tiles_[20][20];

    struct Object {
        u8 type_ = 0;
        u8 x_ = 0;
        u8 y_ = 0;
    };

    Object objects_[60];
};



const RoomData* load_room_normal(int x, int y);
const RoomData* load_room_hard(int x, int y);
