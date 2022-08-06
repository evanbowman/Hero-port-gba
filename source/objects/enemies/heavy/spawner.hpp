#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/enemies/light/drone.hpp"
#include "objects/enemies/light/reaver.hpp"
#include "objects/enemies/light/spew.hpp"



namespace herocore
{



class Spawner : public Enemy
{
public:

    Spawner(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::ignored, Health{16})
    {
        position_ = pos;

        sprite_index_ = 58;

        hitbox_.dimension_.size_ = {16, 12};
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            engine().add_object<BigExplo>(Vec2<Fixnum>{
                    position_.x + 8, position_.y + 5});
            return;
        }


        Enemy::step();

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

        case 40: {
            if (auto s = engine().add_object<EnemyShot>(Vec2<Fixnum>{x() + 6,
                                                                     y() - 2})) {
                auto d = rotate({1, 0}, rng::choice<180>(rng::utility_state));
                d = d * (1.5f / 2);
                Vec2<Fixnum> spd;
                spd.x = Fixnum(d.x);
                spd.y = Fixnum(-d.y);
                s->set_speed(spd);
            }
            if (TaggedObject::count(Tag::drone) +
                TaggedObject::count(Tag::reaver) +
                TaggedObject::count(Tag::spew) < 4) {

                auto o = position_;
                o.x += 4;
                o.y -= 6;
                switch (rng::choice<3>(rng::utility_state)) {
                case 0:
                    engine().add_object<Drone>(o);
                    break;

                case 1:
                    engine().add_object<Reaver>(o);
                    break;

                case 2:
                    engine().add_object<Spew>(o);
                    break;
                }
            }
            break;
        }

        case 80:
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (auto s = engine().add_object<Megashot>(Vec2<Fixnum>{x() + 4,
                                                                        y() - 2})) {
                    auto d = rotate({1, 0}, rng::choice<180>(rng::utility_state));
                    d = d * (1.5f / 2);
                    Vec2<Fixnum> spd;
                    spd.x = Fixnum(d.x);
                    spd.y = Fixnum(-d.y);
                    s->set_speed(spd);
                }
            }
            break;

        case 100:
            if (engine().g_.difficulty_ == Difficulty::hard) {
                break;
            }
            if (auto s = engine().add_object<EnemyShot>(Vec2<Fixnum>{x() + 6,
                                                                     y() - 2})) {
                auto d = rotate({1, 0}, rng::choice<180>(rng::utility_state));
                d = d * (1.5f / 2);
                Vec2<Fixnum> spd;
                spd.x = Fixnum(d.x);
                spd.y = Fixnum(-d.y);
                s->set_speed(spd);
            }
            break;

        case 159:
            timeline_ = 40;
            break;
        }
    }


    int collision_damage() const override
    {
        return 3;
    }


private:
    int timeline_ = 0;
};



}
