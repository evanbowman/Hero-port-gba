#pragma once

#include "objects/enemies/enemy.hpp"
#include "object.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/particles/explo.hpp"



namespace herocore
{



class Crusher : public Enemy
{
private:
    Vec2<Fixnum> direction_;
    Fixnum speed_;
    Fixnum max_speed_;

public:

    Crusher(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::crusher, Health{8})
    {
        position_ = pos;

        sprite_index_ = 9;
        sprite_subimage_ = 0;

        hitbox_.dimension_.size_ = {8, 8};
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            engine().add_object<Explo>(position_);
            return;
        }

        move(position_ + direction_ * speed_);

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
            if (engine().g_.hp_ > 0) {
                auto dir = direction(fvec(position_),
                                     fvec(engine().hero()->position()));

                if (engine().g_.difficulty_ == Difficulty::hard) {
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

        if (engine().g_.difficulty_ == Difficulty::hard) {
            if (speed_ < max_speed_) {
                speed_ += Fixnum(0.1f);
            }
        }

        if (not place_free({position_.x + (direction_ * speed_).x,
                            position_.y + (direction_ * speed_).y})) {

            auto prev_dir = direction_;
            auto v = prev_dir * speed_;

            if (not place_free({position_.x + v.x, position_.y})) {
                direction_.x *= -1;
                position_.x += direction_.x * speed_;
            }

            if (not place_free({position_.x, position_.y + v.y})) {
                direction_.y *= -1;
                position_.y += direction_.y * speed_;
            }

        }
    }


private:

    int timeline_ = 0;

};



}
