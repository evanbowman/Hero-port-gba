#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"



namespace herocore
{



class Drone : public Enemy
{
public:

    Drone(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::drone, Health{2})
    {
        position_ = pos;

        sprite_index_ = 2;

        hitbox_.dimension_.size_ = {8, 8};
    }


    void step() override
    {
        if (health_ <= 0) {
            kill();
            engine().add_object<Explo>(position_);
            platform().speaker().play_sound("snd_explo1", 1);
            return;
        }

        if (TaggedObject::collision_) {
            if (timeline_ < 90 and timeline_ > 45) {
                timeline_ += -2 + rng::choice<4>(rng::utility_state);
                TaggedObject::collision_ = false;
            }
        }

        move(position_ + speed_);

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
            if (engine().g_.hp_ > 0) {
                auto dir = direction(fvec(position_),
                                     fvec(engine().hero()->position()));

                if (engine().g_.difficulty_ == Difficulty::hard) {
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
            if (engine().g_.difficulty_ == Difficulty::hard) {
                speed_ = {0, 0};
            }
            break;

        case 99:
            timeline_ = 40;
            break;
        }

        if (not place_free({position_.x + speed_.x,
                            position_.y + speed_.y})) {
            auto x = position_.x;
            auto y = position_.y;
            if (not place_free({x + 2, y}) or not place_free({x - 2, y})) {
                speed_.x = speed_.x * -1;
            }
            if (not place_free({x, y + 2}) or not place_free({x, y - 2})) {
                speed_.y = speed_.y * -1;
            }
        }
    }


private:

    int timeline_ = 0;

};



}
