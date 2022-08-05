#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"



namespace herocore
{



class Barrier : public Enemy
{
private:
    int anim_ = 0;
    bool visible_ = true;
    bool wide_;

public:

    Barrier(const Vec2<Fixnum>& pos, bool wide) :
        Enemy(TaggedObject::Tag::barrier, Health{32}),
        wide_(wide)
    {
        position_ = pos;

        hitbox_.dimension_.origin_ = {0, 0};

        if (not wide) {
            sprite_index_ = 96;
            hitbox_.dimension_.size_ = {16, 32};
        } else {
            sprite_index_ = 97;
            hitbox_.dimension_.size_ = {32, 16};
        }

    }


    void step() override
    {
        anim_ += 1;
        if (anim_ == 4) {
            visible_ = false;
        } else if (anim_ == 8) {
            anim_ = 0;
            visible_ = true;
        }
    }


    void draw(Platform::Screen& screen) const override
    {
        if (not visible_) {
            return;
        }

        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(sprite_index_ + sprite_subimage_);
        spr_.set_position(position_);
        spr_.set_flip({hflip_, vflip_});
        screen.draw(spr_);

        if (not wide_) {
            spr_.set_position({position_.x, position_.y + 16});
        } else {
            spr_.set_position({position_.x + 16, position_.y});
        }
        screen.draw(spr_);
    }


    int collision_damage_extra_invulnerable_time() const override
    {
        return -27;
    }


    int collision_damage() const override
    {
        return 4;
    }


};



}
