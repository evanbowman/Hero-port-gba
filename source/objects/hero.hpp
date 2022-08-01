#pragma once

#include "object.hpp"



namespace herocore
{



class Hero : public Object
{
public:

    Hero(const Vec2<Fixnum>& position)
    {
        position_ = position;
        sprite_index_ = 1;
        origin_ = {5, 0};
    }


    void step() override;


    void fireleft();
    void fireright();


private:
    Fixnum flyspeed_ = Fixnum(1);
    int autofire_index_ = 0;
    u8 jetpack_counter_ = 0;
};



}
