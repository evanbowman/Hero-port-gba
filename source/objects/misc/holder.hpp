#pragma once

#include "engine.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/enemyProjectile.hpp"
#include "objects/solid.hpp"


namespace herocore
{


// The gamemaker source code calls this object simply obj_holder. For reference,
// this object is the superstructure surrounding a generator.
class Holder : public Solid
{
public:
    Holder(const Vec2<Fixnum>& pos)
    {
        position_ = pos;
        position_.x -= 8;

        hitbox_.dimension_.size_ = {20, 16 + 11};
    }


    bool overlapping(Object& other) override
    {
        auto pos2 = position_;
        pos2.y += (32 + 5);

        Hitbox hb1 = hitbox_;
        Hitbox hb2 = hitbox_;
        hb2.position_ = &pos2;

        if (hb1.overlapping(other.hitbox()) or hb2.overlapping(other.hitbox())) {
            if (dynamic_cast<EnemyProjectile*>(&other)) {
                return false;
            }
            return true;
        }
        return false;
    }


    void draw(Platform::Screen& screen) const
    {
        auto pos = position_;

        Sprite spr_;
        spr_.set_texture_index(196);
        spr_.set_position(position_);
        screen.draw(spr_);

        pos.x += 16;
        spr_.set_texture_index(197);
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.x -= 16;
        pos.y += 16;
        spr_.set_texture_index(198);
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.x += 16;
        spr_.set_texture_index(199);
        spr_.set_position(pos);
        screen.draw(spr_);

        spr_.set_flip({false, true});
        pos.y += 16;
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.x -= 16;
        spr_.set_texture_index(198);
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.y += 16;
        spr_.set_texture_index(196);
        spr_.set_position(pos);
        screen.draw(spr_);

        pos.x += 16;
        spr_.set_texture_index(197);
        spr_.set_position(pos);
        screen.draw(spr_);
    }
};


} // namespace herocore
