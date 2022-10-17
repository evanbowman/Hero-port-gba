#pragma once

#include "object.hpp"



namespace herocore
{



class Shot : public Object
{
private:

public:

    Shot(const Vec2<Fixnum>& pos,
         Fixnum speed,
         int power) :
        power_(power)
    {
        position_ = pos;

        sprite_index_ = 7;
        speed_.x = speed;
        hitbox_.dimension_ = {3, 3, 0, 0};
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y})) {
            kill();
            return;
        }

        Object::step();
    }


    int damage() const
    {
        return power_;
    }


private:
    int timeline_ = 0;
    int power_;
};



}
