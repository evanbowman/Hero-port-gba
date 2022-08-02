#pragma once

#include "object.hpp"



namespace herocore
{



class Supershot : public Object
{
private:

    int anim_ = 0;
    int count_ = 0;

public:

    Supershot(const Vec2<Fixnum>& pos)
    {
        position_ = pos;
        sprite_index_ = 14;
    }


    void set_speed(const Vec2<Fixnum>& speed)
    {
        speed_ = speed;
    }


    void step() override
    {
        Object::step();

        anim_ += 1;
        if (anim_ == 2) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ > 2) {
                sprite_subimage_ = 0;
            }
        }
        if (speed_.x == 0 and speed_.y == 0) {
            count_ += 1;
            if (count_ > 80) {
                kill();
                // TODO: create obj_explo
            }
        }
    }
};



}
