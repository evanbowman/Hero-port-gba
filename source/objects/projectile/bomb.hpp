#pragma once

#include "enemyProjectile.hpp"
#include "engine.hpp"
#include "objects/particles/explo.hpp"


namespace herocore
{


class Bomb : public EnemyProjectile
{
private:
    int anim_ = 0;
    int count_ = 0;
    int bounce_ = 0;

public:
    Bomb(const Vec2<Fixnum>& pos) : EnemyProjectile(pos, 2, engine().g_.maxheat_ / 4)
    {
        sprite_index_ = 32;
        hitbox_.dimension_.size_ = {2, 2};
        hitbox_.dimension_.origin_ = {-1, -1};
    }


    void set_hspeed(Fixnum hsp)
    {
        speed_.x = hsp;
    }


    void set_speed(Vec2<Fixnum> spd)
    {
        speed_ = spd;
    }


    void step() override
    {
        hitbox_.dimension_.size_ = {4, 4};
        hitbox_.dimension_.origin_ = {0, 0};

        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            bounce_ += 1;
            if (bounce_ > 2) {
                auto pos = position_;
                pos.x -= 2;
                pos.y -= 4;
                engine().add_object<Explo>(pos);
                kill();
                return;
            }

            auto prev_speed = speed_;

            if (not place_free({position_.x + prev_speed.x, position_.y})) {
                speed_.x *= Fixnum(-0.5f);

                position_.x += speed_.x;

                while (not place_free({position_.x + speed_.x, position_.y})) {
                    position_.x += speed_.x;
                }
            }

            if (not place_free({position_.x, position_.y + prev_speed.y})) {
                speed_.y *= Fixnum(-0.5f);
                position_.y += speed_.y;

                while (not place_free({position_.x, position_.y + speed_.y})) {
                    position_.y += speed_.y;
                }
            }


        } else {
            EnemyProjectile::step();
        }

        hitbox_.dimension_.size_ = {2, 2};
        hitbox_.dimension_.origin_ = {-1, -1};


        if (speed_.y > Fixnum(2.5f)) {
            speed_.y = Fixnum(2.5f);
        }

        anim_ += 1;
        if (anim_ == 2) {
            anim_ = 0;
            sprite_subimage_ = not sprite_subimage_;
        }

        speed_.y = speed_.y + Fixnum(0.06f);
    }
};


} // namespace herocore
