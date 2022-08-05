#pragma once

#include "object.hpp"
#include "number/random.hpp"



namespace herocore
{



class Star : public Object
{
private:

public:

    Star(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 8;
    }


    void step() override
    {

    }


    void draw(Platform::Screen& s) const override
    {
        if (rng::choice<101>(rng::utility_state) < 98) {
            Object::draw(s);
        }
    }


private:

    int counter_ = 20;


};



}
