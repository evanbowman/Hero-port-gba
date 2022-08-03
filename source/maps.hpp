#pragma once



struct RoomData
{
    bool walls_[20][20];
};



const RoomData* load_room(int x, int y);
