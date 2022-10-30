#pragma once

#include "enemyProjectile.hpp"
#include "engine.hpp"
#include "objects/particles/bigExplo.hpp"


namespace herocore
{


class Laser : public EnemyProjectile
{
private:
    int imgcyc_ = 0;
    Vec2<Fixnum> grav_;

public:
    Laser(const Vec2<Fixnum>& pos, Vec2<Float> dir) :
        EnemyProjectile(pos, 4, engine().g_.maxheat_)
    {
        sprite_index_ = 222;

        hitbox_.dimension_.size_ = {10, 10};
        hitbox_.dimension_.origin_ = {4, 4};
        origin_ = {8, 8};

        if (abs(dir.y) < Fixnum(0.5f)) {
            sprite_subimage_ = 0;
            hflip_ = dir.x < 0;
        } else if (abs(dir.y) < Fixnum(0.75f)) {
            sprite_subimage_ = 1;
            vflip_ = dir.y > 0;
            hflip_ = dir.x < 0;
        } else {
            sprite_subimage_ = 2;
            vflip_ = dir.y > 0;
            hflip_ = false;
        }

        // TODO: play sound...
    }


    void set_grav(const Vec2<Fixnum>& g)
    {
        grav_ = g;
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            engine().add_object<BigExplo>(position_);
            kill();
            return;
        }

        imgcyc_ += 1;
        if (imgcyc_ == 2) {
            imgcyc_ = 0;
        }

        speed_ = speed_ + grav_;

        Object::step();
    }


    void draw(Platform::Screen& screen) const override
    {
        if (imgcyc_) {
            Object::draw(screen);
        }
    }

};


} // namespace herocore
