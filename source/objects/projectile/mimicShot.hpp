#pragma once

#include "enemyProjectile.hpp"
#include "object.hpp"


namespace herocore
{


class MimicShot : public EnemyProjectile
{
private:
public:
    MimicShot(const Vec2<Fixnum>& pos, Fixnum speed) : EnemyProjectile(pos, 1, 0)
    {
        sprite_index_ = 7;
        speed_.x = speed;
        hitbox_.dimension_ = {3, 3, 0, 0};
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y})) {
            kill();
            return;
        }

        Object::step();
    }


private:
    int timeline_ = 0;
};


} // namespace herocore
