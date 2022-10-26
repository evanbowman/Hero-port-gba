#pragma once

#include "objects/enemies/enemy.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/particles/bigExplo.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"



namespace herocore
{



class Bolt : public Enemy
{
private:
    int anim_ = 0;
    int timeline_ = 0;

public:

    Bolt(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::bolt, Health{1})
    {
        position_ = pos;

        sprite_index_ = 21;
        sprite_subimage_ = 0;
        hitbox_.dimension_.size_ = {7, 7};
    }


    bool damage(Health dmg, Object& s) override
    {
        // TODO: screenshake

        platform().speaker().play_sound("snd_explo1", 6);

        kill();
        s.kill();

        if (engine().g_.difficulty_ == Difficulty::hard) {
            auto c = position_;
            c.x += 1;
            c.y += 1;
            for (int i = 0; i < 16; ++i) {
                if (auto p = engine().add_object<Supershot>(c)) {
                    float speed = (i % 2) * 2 + 1;
                    speed *= 0.5f;
                    // if (i % 2) {
                    //     speed = (2 + 1) * 0.5f;
                    // } else {
                    //     speed = 0.5f;
                    // }
                    auto dir = rotate({1, 0}, i * (360 / 16));
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
                    auto dir = rotate({1, 0}, i * (360 / 8) + 22.5f);
                    Vec2<Fixnum> spd;
                    spd.x = Fixnum(speed * dir.x);
                    spd.y = Fixnum(speed * dir.y);
                    p->set_speed(spd);
                }
            }
        }

        engine().add_object<BigExplo>(Vec2<Fixnum>{position_.x + 4, position_.y + 4});
        return true;
    }


    void step() override
    {
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
            timeline_ = 40;
            anim_ += 1;
            if (anim_ == 3) {
                anim_ = 0;
                if (sprite_subimage_ == 2) {
                    sprite_subimage_ = 0;
                } else {
                    sprite_subimage_++;
                }
            }
            break;
        }
    }

};



}
