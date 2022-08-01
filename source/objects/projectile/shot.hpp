#pragma once

#include "object.hpp"



namespace herocore
{



class Shot : public Object
{
private:

public:

    Shot(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 7;
    }


    void step() override
    {

    }


private:

    int timeline_ = 0;

};



}
