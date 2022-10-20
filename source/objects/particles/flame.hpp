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


    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        spr_.set_texture_index(sprite_index_);
        spr_.set_position(position_);
        spr_.set_priority(priority_);
        screen.draw(spr_);
    }


public:

    int counter_ = 20;
    u8 priority_ = 1;
};



}
