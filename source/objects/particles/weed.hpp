#pragma once


#pragma once

#include "number/random.hpp"
#include "object.hpp"


namespace herocore
{


class Weed : public Object
{
private:
    int anim_cyc_ = 0;

public:
    Weed(const Vec2<Fixnum>& pos, bool yflip)
    {
        position_ = pos;
        sprite_index_ = 86;

        sprite_subimage_ = rng::choice<6>(rng::utility_state);

        if (yflip) {
            // vflip_ = true;
            position_.y -= 13;
            position_.x -= 3;
            vflip_ = true;
        } else {
            position_.x -= 4;
            position_.y += 5;
        }
    }


    void step() override
    {
        anim_cyc_ += 1;
        if (anim_cyc_ == 2 + 3) {
            anim_cyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 6) {
                sprite_subimage_ = 1;
            }
        }
    }


private:
    int counter_ = 20;
};


} // namespace herocore
