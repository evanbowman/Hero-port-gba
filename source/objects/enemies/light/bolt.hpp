#pragma once

#include "objects/enemies/enemy.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"



namespace herocore
{



class Bolt : public Enemy
{
private:
    int anim_ = 0;
    int timeline_ = 0;

public:

    Bolt(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::bolt, Health{1})
    {
        position_ = pos;

        sprite_index_ = 21;
        sprite_subimage_ = 0;
        hitbox_.dimension_.size_ = {7, 7};
    }


    void step() override
    {
        switch (timeline_++) {
        case 0:
            sprite_subimage_ = 0;
            break;

        case 10:
            sprite_subimage_ = 1;
            break;

        case 20:
            sprite_subimage_ = 0;
            break;

        case 30:
            sprite_subimage_ = 1;
            break;

        case 40:
            timeline_ = 40;
            anim_ += 1;
            if (anim_ == 3) {
                anim_ = 0;
                if (sprite_subimage_ == 2) {
                    sprite_subimage_ = 0;
                } else {
                    sprite_subimage_++;
                }
            }
            break;
        }
    }

};



}
