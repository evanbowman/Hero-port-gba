#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "number/random.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/gigashot.hpp"
#include "objects/particles/bigExplo.hpp"



namespace herocore
{



class BattleDoor : public Enemy
{
private:
    u8 spawn_x_;
    u8 spawn_y_;

    int timeline_ = 0;
    int shotcyc_ = 0;
    int shotcyc2_ = 0;
    int shotcyc3_ = 0;

    bool awake_ = false;
    u8 animcyc_ = 0;

public:

    BattleDoor(const Vec2<Fixnum>& pos,
               u8 spawn_x,
               u8 spawn_y) :
        Enemy(TaggedObject::Tag::ignored, Health{32}),
        spawn_x_(spawn_x),
        spawn_y_(spawn_y)
    {
        position_ = pos;

        sprite_index_ = 104;

        hitbox_.dimension_.size_ = {12, 16};
        hitbox_.dimension_.origin_.x = 2;

        engine().room_.walls_[spawn_x_ / 8][spawn_y_ / 8] = true;
        engine().room_.walls_[spawn_x_ / 8][spawn_y_ / 8 + 1] = true;
    }


    void timeline_easy()
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

        case 40: {
            awake_ = true;
            shotcyc_ += 1;
            if (engine().hero()->x() < x() + 4) {
                if (auto e = engine().add_object<EnemyShot>(Vec2<Fixnum>{
                            x() - 4, y() + 6
                        })) {
                    auto dir = rotate({1, 0}, 135 + rng::choice<90>(rng::utility_state));
                    dir = dir * 0.5f;
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            } else {
                if (auto e = engine().add_object<EnemyShot>(Vec2<Fixnum>{
                            x() + 9, y() + 6
                        })) {
                    auto angle = -45 + rng::choice<90>(rng::utility_state);
                    if (angle < 0) {
                        angle += 360;
                    }
                    auto dir = rotate({1, 0}, angle);
                    dir = dir * 0.5f;
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
            if (shotcyc3_ == 0) {
                for (int i = 0; i < 4; ++i) {
                    if (engine().hero()->x() < x() + 4) {
                        if (auto e = engine().add_object<EnemyShot>(Vec2<Fixnum>{
                            x() - 4, y() + 6
                                })) {
                            auto angle = 265 - shotcyc_ * 10 - i * 40;
                            if (angle < 0) {
                                angle += 360;
                            }
                            if (angle >= 360) {
                                angle -= 360;
                            }
                            auto dir = rotate({1, 0}, angle);
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                    } else {
                        if (auto e = engine().add_object<EnemyShot>(Vec2<Fixnum>{
                            x() + 9, y() + 6
                                })) {
                            auto angle = 85 - shotcyc_ * 10 - i * 40;
                            if (angle < 0) {
                                angle += 360;
                            }
                            if (angle >= 360) {
                                angle -= 360;
                            }
                            auto dir = rotate({1, 0}, angle);
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                    }
                }
            }
            if (shotcyc_ == 4) {
                if (shotcyc3_ == 0) {
                    shotcyc3_ = 1;
                } else {
                    shotcyc3_ = 0;
                }
                shotcyc2_ += 1;
                if (shotcyc2_ == 1) {
                    // TODO missiles!
                } else if (shotcyc2_ == 2) {
                    if (engine().hero()->x() < x() + 4) {
                        Vec2<Fixnum> o{x() - 8, y() + 4};
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 140);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 180);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 220);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                    } else {
                        Vec2<Fixnum> o{x() + 17, y() + 4};
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 40);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 0);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 320);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                    }
                    shotcyc2_ = 0;
                }
                shotcyc_ = 0;
            }
            break;
        }
        case 69:
            timeline_ = 40;
            break;
        }
    }


    void timeline_hard()
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

        case 40: {
            awake_ = true;
            shotcyc_ += 1;
            if (engine().hero()->x() < x() + 4) {
                if (auto e = engine().add_object<EnemyShot>(Vec2<Fixnum>{
                            x() - 4, y() + 6
                        })) {
                    auto dir = rotate({1, 0}, 140 + rng::choice<80>(rng::utility_state));
                    dir = dir * 0.5f;
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            } else {
                if (auto e = engine().add_object<EnemyShot>(Vec2<Fixnum>{
                            x() + 9, y() + 6
                        })) {
                    auto angle = -40 + rng::choice<80>(rng::utility_state);
                    if (angle < 0) {
                        angle += 360;
                    }
                    auto dir = rotate({1, 0}, angle);
                    dir = dir * 0.5f;
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
            if (shotcyc3_ == 0) {
                for (int i = 0; i < 4; ++i) {
                    if (engine().hero()->x() < x() + 4) {
                        if (auto e = engine().add_object<Supershot>(Vec2<Fixnum>{
                            x() - 8, y() + 6
                                })) {
                            auto angle = 265 - shotcyc_ * 10 - i * 40;
                            if (angle < 0) {
                                angle += 360;
                            }
                            if (angle >= 360) {
                                angle -= 360;
                            }
                            auto dir = rotate({1, 0}, angle);
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                    } else {
                        if (auto e = engine().add_object<Supershot>(Vec2<Fixnum>{
                            x() + 12, y() + 6
                                })) {
                            auto angle = 85 - shotcyc_ * 10 - i * 40;
                            if (angle < 0) {
                                angle += 360;
                            }
                            if (angle >= 360) {
                                angle -= 360;
                            }
                            auto dir = rotate({1, 0}, angle);
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                    }
                }
            }
            if (shotcyc_ == 4) {
                if (shotcyc3_ == 0) {
                    shotcyc3_ = 1;
                } else {
                    shotcyc3_ = 0;
                }
                shotcyc2_ += 1;
                if (shotcyc2_ == 1) {
                    // TODO missiles!
                } else if (shotcyc2_ == 2) {
                    if (engine().hero()->x() < x() + 4) {
                        Vec2<Fixnum> o{x() - 8, y() + 4};
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 140);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Gigashot>(Vec2<Fixnum>{
                                    o.x - 4,
                                        o.y - 2
                                })) {
                            auto d = rotate({1, 0}, 180);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 220);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Supershot>(o)) {
                            auto d = rotate({1, 0}, 160);
                            d = d * (1.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<EnemyShot>(o)) {
                            auto d = rotate({1, 0}, 120);
                            d = d * (1.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Supershot>(o)) {
                            auto d = rotate({1, 0}, 200);
                            d = d * (1.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<EnemyShot>(o)) {
                            auto d = rotate({1, 0}, 240);
                            d = d * (1.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                    } else {
                        Vec2<Fixnum> o{x() + 16, y() + 4};
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 40);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Gigashot>(Vec2<Fixnum>{
                                    o.x + 4,
                                    o.y - 2
                                })) {
                            auto d = rotate({1, 0}, 0);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Megashot>(o)) {
                            auto d = rotate({1, 0}, 320);
                            d = d * (2.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Supershot>(o)) {
                            auto d = rotate({1, 0}, 20);
                            d = d * (1.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<EnemyShot>(o)) {
                            auto d = rotate({1, 0}, 360 - 20);
                            d = d * (1.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<Supershot>(o)) {
                            auto d = rotate({1, 0}, 60);
                            d = d * (1.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                        if (auto e = engine().add_object<EnemyShot>(o)) {
                            auto d = rotate({1, 0}, 40);
                            d = d * (1.5f / 2);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                    }
                    shotcyc2_ = 0;
                }
                shotcyc_ = 0;
            }
            break;
        }
        case 69:
            timeline_ = 40;
            break;
        }

        if (awake_) {
            animcyc_ += 1;
            if (animcyc_ == 2) {
                animcyc_ = 0;
                sprite_subimage_ += 1;
                if (sprite_subimage_ == 6) {
                    sprite_subimage_ = 1;
                }
            }
        }
    }


    void step() override
    {
        if (health_ == 0) {
            kill();

            engine().room_.walls_[spawn_x_ / 8][spawn_y_ / 8] = false;
            engine().room_.walls_[spawn_x_ / 8][spawn_y_ / 8 + 1] = false;

            engine().add_object<BigExplo>(Vec2<Fixnum>{
                    position_.x + 4, position_.y + 8});
            return;
        }


        Enemy::step();

        if (engine().g_.difficulty_ == Difficulty::normal) {
            timeline_easy();
        } else {
            timeline_hard();
        }
    }


    int collision_damage() const override
    {
        return 3;
    }
};



}
