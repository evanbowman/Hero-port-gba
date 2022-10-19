#pragma once

#include "enemyProjectile.hpp"
#include "supershot.hpp"
#include "enemyShot.hpp"
#include "objects/particles/bigExplo.hpp"



namespace herocore
{



class GigaExplo : public EnemyProjectile
{
private:

    int ttl_ = 0;
    int anim_ = 0;
    int count_ = 0;

public:

    GigaExplo(const Vec2<Fixnum>& pos) :
        EnemyProjectile(pos, 2, engine().g_.maxheat_ / 4)
    {
        sprite_index_ = 114;
        hitbox_.dimension_.size_ = {16, 16};
    }


    void step() override
    {
        anim_ += 1;
        if (anim_ == 3) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ > 2) {
                sprite_subimage_ = 0;
            }
        }

        if (++ttl_ == 12) {
            kill();
        }
    }
};



class Gigashot : public EnemyProjectile
{
private:

    int anim_ = 0;
    int count_ = 0;
    Vec2<Fixnum> grav_;

public:

    Gigashot(const Vec2<Fixnum>& pos) :
        EnemyProjectile(pos, 4, engine().g_.maxheat_ / 2)
    {
        sprite_index_ = 110;
        hitbox_.dimension_.size_ = {12, 12};
    }


    void set_speed(const Vec2<Fixnum>& speed)
    {
        speed_ = speed;
    }


    void set_grav(const Vec2<Fixnum>& g)
    {
        grav_ = g;
    }


    void on_hero_collision() override
    {
        // Passes through fliphero.
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            engine().add_object<GigaExplo>(Vec2<Fixnum>{x() - 2, y() - 2});
            kill();
            return;
        }

        speed_ = speed_ + grav_;

        Object::step();

        anim_ += 1;
        if (anim_ == 2) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ > 3) {
                sprite_subimage_ = 0;
            }
        }
    }
};



}
