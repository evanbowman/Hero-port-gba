#pragma once

#include "enemyProjectile.hpp"
#include "supershot.hpp"
#include "enemyShot.hpp"
#include "objects/particles/bigExplo.hpp"



namespace herocore
{



class Megashot : public EnemyProjectile
{
private:

    int anim_ = 0;
    int count_ = 0;

public:

    Megashot(const Vec2<Fixnum>& pos) : EnemyProjectile(pos, 3, 0)
    {
        sprite_index_ = 43;
        hitbox_.dimension_.size_ = {8, 8};
    }


    void kill() override
    {
        EnemyProjectile::kill();

        if (engine().g_.difficulty_ == Difficulty::hard) {
            auto c = position_;
            c.x += 1;
            c.y += 1;
            for (int i = 0; i < 4; ++i) {
                if (auto p = engine().add_object<Supershot>(c)) {
                    float speed = 1;
                    auto dir = rotate({1, 0}, i * 90 + 45);
                    Vec2<Fixnum> spd;
                    spd.x = Fixnum(speed * dir.x);
                    spd.y = Fixnum(speed * dir.y);
                    p->set_speed(spd);
                }
            }
        } else {
            for (int i = 0; i < 16; ++i) {
                auto c = position_;
                c.x += 2;
                c.y += 2;
                if (auto p = engine().add_object<EnemyShot>(c)) {
                    auto speed = 1.f;
                    auto dir = rotate({1, 0}, i * 90 + 45);
                    Vec2<Fixnum> spd;
                    spd.x = Fixnum(speed * dir.x);
                    spd.y = Fixnum(speed * dir.y);
                    p->set_speed(spd);
                }
            }
        }

        engine().add_object<BigExplo>(Vec2<Fixnum>{position_.x + 4, position_.y + 4});
    }


    void on_hero_collision() override
    {
        // Passes through fliphero.
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            kill();
            return;
        }

        Object::step();

        anim_ += 1;
        if (anim_ == 1) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ > 4) {
                sprite_subimage_ = 0;
            }
        }
    }
};



}
