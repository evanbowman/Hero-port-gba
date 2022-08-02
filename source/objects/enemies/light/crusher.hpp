#pragma once

#include "objects/enemies/enemy.hpp"
#include "object.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/projectile/supershot.hpp"



namespace herocore
{



class Crusher : public Enemy
{
private:
    Vec2<Fixnum> direction_;
    Fixnum speed_;
    Fixnum max_speed_;

public:

    Crusher(const Vec2<Fixnum>& pos) : Enemy(Tag::crusher, Health{8})
    {
        position_ = pos;

        sprite_index_ = 9;
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

                if (engine().difficulty_ == Difficulty::hard) {
                    speed_ = 0;
                    max_speed_ = 2.8;

                    engine().add_object<Supershot>(position_);

                } else {
                    speed_ = Fixnum(1.8f / 2);
                }

                direction_.x = Fixnum(dir.x);
                direction_.y = Fixnum(dir.y);
            }
            break;

        case 79:
            timeline_ = 40;
            break;
        }

        if (engine().difficulty_ == Difficulty::hard) {
            if (speed_ < max_speed_) {
                speed_ += Fixnum(0.1f);
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
