#pragma once

#include "objects/enemies/enemy.hpp"
#include "objects/projectile/bomb.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"



namespace herocore
{



class Blomb : public Enemy
{
private:

    int timeline_ = 0;
    int movedir_ = 0;
    bool move_ = false;

public:

    Blomb(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::blomb, Health{2})
    {
        position_ = pos;

        sprite_index_ = 30;
        sprite_subimage_ = 0;

        hitbox_.dimension_.size_ = {8, 8};
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            platform().speaker().play_sound("snd_explo1", 6);
            engine().add_object<Explo>(position_);
            return;
        }

        auto prev = position_;
        position_ = position_ + speed_ * Fixnum(2.f);
        if (not place_free(position_)) {
            position_ = prev;
            if (movedir_ == 1) {
                movedir_ = 0;
            } else {
                movedir_ = 1;
            }
            if (movedir_ == 0) {
                speed_.x = 1;
            } else {
                speed_.x = -1;
            }
            position_ = position_ + speed_ * Fixnum(2.f);

        } else if (move_) {
            auto ledge = position_;
            ledge.x += speed_.x * 8;
            ledge.y -= 4;
            if (place_free(ledge)) {
                if (movedir_ == 1) {
                    movedir_ = 0;
                } else {
                    movedir_ = 1;
                }
                if (movedir_ == 0) {
                    speed_.x = 1;
                } else {
                    speed_.x = -1;
                }
            }
        }

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
            bomb();
            if (movedir_ == 0) {
                speed_.x = 1;
            } else {
                speed_.x = -1;
            }
            move_ = true;
            break;

        case 50:
            bomb();
            break;

        case 60:
            if (engine().g_.difficulty_ == Difficulty::hard) {
                bomb();
            }
            break;

        case 80:
            if (engine().g_.difficulty_ == Difficulty::hard) {
                bomb();
            }
            break;

        case 119:
            timeline_ = 40;
            break;

        }

        // if (move_) {
        //     auto x = position_.x;
        //     auto y = position_.y + 1;

        //     if (movedir_ == 0) {
        //         if (place_free({x + 2, y}) and not place_free({x + 10, y - 4})) {
        //             position_.x += 2;
        //         }
        //     } else {
        //         if (place_free({x - 2, y}) and not place_free({x - 10, y - 4})) {
        //             position_.x -= 2;
        //         }
        //     }
        // }
    }


    void bomb()
    {
        auto p = position_;
        p.x += 2;
        p.y += 4;
        auto b = engine().add_object<Bomb>(p);
        if (b) {
            if (speed_.x < 0) {
                b->set_hspeed(Fixnum(-1.f));
            } else {
                b->set_hspeed(Fixnum(1.f));
            }
        }
    }
};



}
