#pragma once

#include "object.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"



namespace herocore
{



class Reaver : public Object
{
private:
    Vec2<Fixnum> direction_;
    Fixnum speed_;
    Fixnum max_speed_;

public:

    Reaver(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 4;
        sprite_subimage_ = 0;
    }


    void step() override
    {
        position_ = position_ + direction_ * speed_;

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
            if (engine().hp_ > 0) {
                auto dir = direction(fvec(position_),
                                     fvec(engine().hero()->position()));

                sprite_subimage_ = 2;
                origin_.x = 4;

                if (position_.x > engine().hero()->position().x) {
                    hflip_ = false;
                } else {
                    hflip_ = true;
                }

                if (engine().difficulty_ == Difficulty::hard) {
                    dir = rotate(dir, -20 + rng::choice<40>(rng::utility_state));
                    if (max_speed_ == Fixnum(1.2f)) {
                        max_speed_ = Fixnum(0.4f);
                    } else {
                        max_speed_ = Fixnum(1.2f);
                    }
                } else {
                    speed_ = Fixnum(1.f);
                    dir = dir * (1.4f / 2);
                }

                direction_.x = Fixnum(dir.x);
                direction_.y = Fixnum(dir.y);
            }
            break;

        case 49:
            if (engine().difficulty_ == Difficulty::hard) {
                timeline_ = 40;
            }
            break;

        case 59:
            timeline_ = 40;
            break;
        }

        if (engine().difficulty_ == Difficulty::hard) {
            if (speed_ < max_speed_) {
                speed_ = speed_ + Fixnum(0.2f);
            }
            if (speed_ > max_speed_) {
                speed_ = speed_ - Fixnum(0.125f);
            }
        }

        if (not place_free({position_.x + (direction_ * speed_).x,
                            position_.y + (direction_ * speed_).y})) {
            direction_.x = direction_.x * -1;
            direction_.y = direction_.y * -1;
        }
    }


private:

    int timeline_ = 0;

};



}
