#pragma once

#include "object.hpp"



namespace herocore
{



class Explo : public Object
{
private:

    int anim_cyc_ = 0;

public:

    Explo(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 34;
    }


    void step() override
    {
        anim_cyc_ += 1;
        if (anim_cyc_ == 3) {
            anim_cyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 3) {
                kill();
            }
        }
    }


private:

    int counter_ = 20;


};



}
