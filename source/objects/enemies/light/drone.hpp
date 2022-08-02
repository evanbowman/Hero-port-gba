#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"



namespace herocore
{



class Drone : public Enemy
{
public:

    Drone(const Vec2<Fixnum>& pos) :
        Enemy(Tag::drone, Health{2})
    {
        position_ = pos;

        sprite_index_ = 2;
    }


    void step() override
    {
        Object::step();

        switch (timeline_++) {
        case 0:
            sprite_index_ = 2;
            break;

        case 10:
            sprite_index_ = 3;
            break;

        case 20:
            sprite_index_ = 2;
            break;

        case 30:
            sprite_index_ = 3;
            break;

        case 40:
            if (engine().hp_ > 0) {
                auto dir = direction(fvec(position_),
                                     fvec(engine().hero()->position()));

                if (engine().difficulty_ == Difficulty::hard) {
                    dir = rotate(dir, -20 + rng::choice<40>(rng::utility_state));
                    dir = dir * (1.7f / 2.f);
                } else {
                    dir = dir * 0.5f;
                }

                speed_.x = Fixnum(dir.x);
                speed_.y = Fixnum(dir.y);
            }
            break;

        case 75:
            if (engine().difficulty_ == Difficulty::hard) {
                speed_ = {0, 0};
            }
            break;

        case 99:
            timeline_ = 40;
            break;
        }

        if (not place_free({position_.x + speed_.x,
                            position_.y + speed_.y})) {
            speed_.x = speed_.x * -1;
            speed_.y = speed_.y * -1;
        }
    }


private:

    int timeline_ = 0;

};



}
