#pragma once

#include "engine.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/enemyProjectile.hpp"
#include "objects/solid.hpp"


namespace herocore
{



class Ship : public Solid
{
public:
    Ship(const Vec2<Fixnum>& pos)
    {
        position_ = pos;
        position_.y -= 2;

        hitbox_.dimension_.size_ = {38, 16};
    }


    void draw(Platform::Screen& screen) const
    {
        auto pos = position_;

        Sprite spr_;
        spr_.set_texture_index(238);
        spr_.set_position(position_);
        screen.draw(spr_);

        pos.x += 16;
        spr_.set_texture_index(239);
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.x += 16;
        spr_.set_texture_index(240);
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.x -= 32;
        pos.y += 16;
        spr_.set_texture_index(241);
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.x += 16;
        spr_.set_texture_index(242);
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.x += 16;
        spr_.set_texture_index(243);
        spr_.set_position(pos);
        screen.draw(spr_);
    }
};


} // namespace herocore
