#pragma once

#include "object.hpp"
#include "engine.hpp"



namespace herocore
{



class EnemyShot : public Object
{
private:

    int anim_ = 0;
    int count_ = 0;

public:

    EnemyShot(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 17;
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
            if (sprite_subimage_ > 3) {
                sprite_subimage_ = 0;
            }
        }

        if (speed_.x == 0 and speed_.y == 0) {
            count_ += 1;
            if (count_ > 40) {
                kill();
            }
        }
    }

};



}
