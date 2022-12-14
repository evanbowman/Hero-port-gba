#pragma once

#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/particles/explo.hpp"


namespace herocore
{


class Reaver : public Enemy
{
private:
    Vec2<Fixnum> direction_;
    Fixnum speed_;
    Fixnum max_speed_;

public:
    Reaver(const Vec2<Fixnum>& pos) : Enemy(TaggedObject::Tag::reaver, Health{4})
    {
        position_ = pos;

        sprite_index_ = 4;
        sprite_subimage_ = 0;

        hitbox_.dimension_.size_ = {8, 8};
    }


    void step() override
    {
        if (health_ <= 0) {
            kill();
            platform().speaker().play_sound("snd_explo1", 6);
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
                auto dir = direction(fvec(position_), fvec(engine().hero()->position()));

                sprite_subimage_ = 2;
                origin_.x = 4;

                if (position_.x > engine().hero()->position().x) {
                    hflip_ = false;
                } else {
                    hflip_ = true;
                }

                if (engine().g_.difficulty_ == Difficulty::hard) {
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
            if (engine().g_.difficulty_ == Difficulty::hard) {
                timeline_ = 40;
            }
            break;

        case 59:
            timeline_ = 40;
            break;
        }

        if (engine().g_.difficulty_ == Difficulty::hard) {
            if (speed_ < max_speed_) {
                speed_ = speed_ + Fixnum(0.2f);
            }
            if (speed_ > max_speed_) {
                speed_ = speed_ - Fixnum(0.125f);
            }
        }

        if (not place_free({position_.x + (direction_ * speed_).x,
                            position_.y + (direction_ * speed_).y})) {
            auto x = position_.x;
            auto y = position_.y;
            if (not place_free({x + 2, y}) or not place_free({x - 2, y})) {
                direction_.x = direction_.x * -1;
            }
            if (not place_free({x, y + 2}) or not place_free({x, y - 2})) {
                direction_.y = direction_.y * -1;
            }
        }
    }


private:
    int timeline_ = 0;
};


} // namespace herocore
