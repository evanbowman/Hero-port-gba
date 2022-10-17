#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "number/random.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/particles/bigExplo.hpp"



namespace herocore
{



class Soldier : public Enemy
{
public:

    Soldier(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::soldier, Health{16})
    {
        position_ = pos;

        sprite_index_ = 60;

        hitbox_.dimension_.size_ = {6, 8};
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            platform().speaker().play_sound("snd_explo2", 1);
            engine().add_object<BigExplo>(Vec2<Fixnum>{
                    position_.x + 3, position_.y + 4});
            return;
        }

        const bool hard = engine().g_.difficulty_ == Difficulty::hard;


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
            auto dir = rotate({1, 0}, rng::choice<360>(rng::utility_state));
            auto speed = dir * ((hard ? 1.5f : 1.3f) / 2);
            if (x() > engine().hero()->x()) {
                sprite_subimage_ = 2;
            } else {
                sprite_subimage_ = 3;
            }
            speed_.x = Fixnum(speed.x);
            speed_.y = Fixnum(speed.y);
            if (auto s = engine().add_object<Supershot>(position_)) {
                auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
                Vec2<Fixnum> spd;
                spd.x = Fixnum(dir.x);
                spd.y = Fixnum(dir.y);
                s->set_speed(spd);
            }
            break;
        }

        case 90: {
            if (x() > engine().hero()->x()) {
                sprite_subimage_ = 2;
            } else {
                sprite_subimage_ = 3;
            }

            if (hard) {
                movecyc_ += 1;
            }

            if (hard and movecyc_ == 4) {
                movecyc_ = 0;
                speed_.x = 0;
                speed_.y = 0;
                if (auto m = engine().add_object<Megashot>(Vec2<Fixnum>{x(),
                                                                        y()})) {
                    auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
                    Vec2<Fixnum> spd;
                    spd.x = Fixnum(dir.x);
                    spd.y = Fixnum(dir.y);
                    m->set_speed(spd);
                }
            } else {
                auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
                auto angle = rng::choice<20>(rng::utility_state);
                if (angle < 10) {
                    angle = 360 - angle;
                } else {
                    angle -= 10;
                }
                dir = rotate(dir, angle) * (0.8f / 2);
                speed_.x = Fixnum(dir.x);
                speed_.y = Fixnum(dir.y);


                if (auto s = engine().add_object<EnemyShot>(position_)) {
                    auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
                    auto angle = rng::choice<90>(rng::utility_state);
                    if (angle < 45) {
                        angle = 360 - angle;
                    } else {
                        angle -= 45;
                    }
                    dir = rotate(dir, angle);
                    Vec2<Fixnum> spd;
                    spd.x = Fixnum(dir.x);
                    spd.y = Fixnum(dir.y);
                    s->set_speed(spd);
                }
            }
            break;
        }

        case 139:
            timeline_ = 40;
            break;
        }

        if (not place_free({position_.x + speed_.x,
                            position_.y + speed_.y})) {
            auto x = position_.x;
            auto y = position_.y;
            if (not place_free({x + 2, y}) or not place_free({x - 2, y})) {
                speed_.x = speed_.x * -1;
            }
            if (not place_free({x, y + 2}) or not place_free({x, y - 2})) {
                speed_.y = speed_.y * -1;
            }
        }
    }


    int collision_damage() const override
    {
        return 3;
    }


private:
    int timeline_ = 0;
    int movecyc_ = 0;
};



}
