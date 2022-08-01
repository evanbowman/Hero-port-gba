#pragma once

#include "object.hpp"



namespace herocore
{



class Flame : public Object
{
private:

public:

    Flame(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 8;
    }


    void step() override
    {
        if (--counter_ == 0) {
            kill();
            return;
        }
    }


private:

    int counter_ = 20;


};



}
