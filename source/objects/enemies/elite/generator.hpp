#pragma once

#include "objects/enemies/enemy.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/particles/bigExplo.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"



namespace herocore
{



class Generator : public Enemy
{
private:
    int anim_ = 0;
    int timeline_ = 0;

public:

    Generator(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::generator, Health{32})
    {
        position_ = pos;

        sprite_index_ = 92;
        sprite_subimage_ = 0;
        hitbox_.dimension_.size_ = {14, 14};
    }


    void step() override
    {
        anim_ += 1;
        if (anim_ == 4) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 4) {
                sprite_subimage_ = 0;
            }
        }

        switch (timeline_++) {
        case 40:
            break;

        case 109:
            timeline_ = 40;
            break;
        }
    }

};



}
