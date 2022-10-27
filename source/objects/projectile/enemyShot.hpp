#pragma once

#include "enemyProjectile.hpp"
#include "engine.hpp"
#include "taggedObject.hpp"


namespace herocore
{


class EnemyShot : public EnemyProjectile
{
private:
    int anim_ = 0;
    int count_ = 0;

public:
    EnemyShot(const Vec2<Fixnum>& pos) : EnemyProjectile(pos, 1, 0)
    {
        sprite_index_ = 17;

        hitbox_.dimension_.size_ = {5, 5};
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
            if (sprite_subimage_ > 3) {
                sprite_subimage_ = 0;
            }
        }

        if (speed_.x == 0 and speed_.y == 0) {
            count_ += 1;
            if (count_ > 40) {
                kill();
            }
        }
    }
};


} // namespace herocore
