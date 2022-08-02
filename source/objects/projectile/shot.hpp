#pragma once

#include "object.hpp"



namespace herocore
{



class Shot : public Object
{
private:

public:

    Shot(const Vec2<Fixnum>& pos, Fixnum speed)
    {
        position_ = pos;

        sprite_index_ = 7;
        speed_.x = speed;
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y})) {
            kill();
            return;
        }

        Object::step();
    }


private:
    int timeline_ = 0;

};



}
