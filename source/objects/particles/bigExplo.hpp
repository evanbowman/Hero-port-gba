#pragma once

#include "object.hpp"



namespace herocore
{



class BigExplo : public Object
{
private:

    int anim_cyc_ = 0;

public:

    BigExplo(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 24;
        origin_ = {8, 8};
    }


    void step() override
    {
        anim_cyc_ += 1;
        if (anim_cyc_ == 3) {
            anim_cyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 6) {
                kill();
            }
        }
    }


private:

    int counter_ = 20;


};



}
