#pragma once

#include "object.hpp"



namespace herocore
{



class LiquidMetal : public Object
{
private:

public:

    LiquidMetal(const Vec2<Fixnum>& pos, Hitbox::Dimension d)
    {
        position_ = pos;
        hitbox_.dimension_ = d;
    }

    void step() override
    {
    }

    void draw(Platform::Screen& sc) const override
    {
    }
};



}
