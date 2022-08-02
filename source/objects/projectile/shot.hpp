#pragma once

#include "object.hpp"
#include "engine.hpp"



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
        engine().shot_count_++;
        speed_.x = speed;
    }


    void step() override
    {
        Object::step();

        if (position_.x < 38) {
            kill();
        }
        if (position_.x > 202) {
            kill();
        }
    }


    ~Shot()
    {
        engine().shot_count_--;
    }


private:
    int timeline_ = 0;

};



}
