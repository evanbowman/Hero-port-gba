#pragma once

#include "enemyProjectile.hpp"



namespace herocore
{



class Supershot : public EnemyProjectile
{
private:

    int anim_ = 0;
    int count_ = 0;

public:

    Supershot(const Vec2<Fixnum>& pos) : EnemyProjectile(pos)
    {
        sprite_index_ = 14;
        hitbox_.dimension_.size_ = {7, 7};
    }


    void set_speed(const Vec2<Fixnum>& speed)
    {
        speed_ = speed;
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            kill();
            return;
        }

        Object::step();

        anim_ += 1;
        if (anim_ == 2) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ > 2) {
                sprite_subimage_ = 0;
            }
        }
        if (speed_.x == 0 and speed_.y == 0) {
            count_ += 1;
            if (count_ > 80) {
                kill();
                // TODO: create obj_explo
            }
        }
    }
};



}
