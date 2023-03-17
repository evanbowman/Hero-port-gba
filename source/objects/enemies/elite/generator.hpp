#pragma once

#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/projectile/bomb.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"


namespace herocore
{


class Generator : public Enemy
{
private:
    int anim_ = 0;
    int timeline_ = 0;
    int timcyc_ = 0;
    bool shot_40_ = false;
    u8 spawn_x_;
    u8 spawn_y_;


public:
    Generator(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y)
        : Enemy(TaggedObject::Tag::ignored, Health{32}), spawn_x_(spawn_x),
          spawn_y_(spawn_y)
    {
        position_ = pos;

        sprite_index_ = 92;
        sprite_subimage_ = 0;
        hitbox_.dimension_.size_ = {14, 14};
        hitbox_.dimension_.origin_ = {7, 7};
        origin_ = {7, 7};
        position_.x += 7;
        position_.y += 5;
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            platform().speaker().play_sound("snd_explo4", 6);
            if (engine().g_.screenshake_ < 8) {
                engine().g_.screenshake_ = 8;
            }
            engine().add_object<ExploSpewer>(position_);
            engine().room_.clear_adjacent_barriers();

            engine().p_->objects_removed_.push_back({(u8)engine().room_.coord_.x,
                                                     (u8)engine().room_.coord_.y,
                                                     spawn_x_,
                                                     spawn_y_});

            engine().show_dialog(7,
                                 "Generator destroyed - nearby "
                                 "barriers lowered! These barriers will always"
                                 " remain lowered.");
        }

        anim_ += 1;
        if (anim_ == 4) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 4) {
                sprite_subimage_ = 0;
            }
        }

        switch (timeline_++) {
        case 40:
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (not shot_40_) {
                    shot_40_ = true;
                    auto o = position_;
                    o.x -= 2;
                    o.y -= 2;
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({-0.25f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({-0.5f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({-1.f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({-1.5f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({0.25f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({0.5f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({1.f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({1.5f, 0.f});
                    }
                }
                timcyc_ += 1;
                if (timcyc_ == 16) {
                    timcyc_ = 0;
                }

                auto o = position_;
                o.x -= 2;
                o.y -= 2;
                if (auto b = engine().add_object<Bomb>(o)) {
                    auto speed = 0.5f * (2.5f + timcyc_ * 0.2f);
                    Vec2<float> dir;
                    if (x() < engine().hero()->x()) {
                        dir = rotate({1, 0}, timcyc_ * 4);
                    } else {
                        dir = rotate({1, 0}, 180 - timcyc_ * 4);
                    }
                    dir = dir * speed;
                    b->set_speed({Fixnum(dir.x), Fixnum(-dir.y)});
                }

                if (timcyc_ % 8 == 0) {
                    auto dir = direction(fvec(o), fvec(engine().hero()->position()));

                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 300);
                        s->set_speed({Fixnum(d.x * 0.25f), Fixnum(d.y * 0.25f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 60);
                        s->set_speed({Fixnum(d.x * 0.25f), Fixnum(d.y * 0.25f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 320);
                        s->set_speed({Fixnum(d.x * 0.5f), Fixnum(d.y * 0.5f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 40);
                        s->set_speed({Fixnum(d.x * 0.5f), Fixnum(d.y * 0.5f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 340);
                        s->set_speed({Fixnum(d.x * 0.75f), Fixnum(d.y * 0.75f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        s->set_speed({Fixnum(dir.x * 1.f), Fixnum(dir.y * 1.f)});
                    }

                } else if (timcyc_ % 4 == 0) {
                    auto o = position_;
                    o.x -= 2;
                    o.y -= 2;
                    for (int i = 1; i < 10; ++i) {
                        if (auto s = engine().add_object<Supershot>(o)) {
                            Vec2<Float> dir;
                            if (x() < engine().hero()->x()) {
                                auto v = -80 + i * 16;
                                if (v < 0) {
                                    v += 360;
                                }
                                dir = rotate({1, 0}, v);
                            } else {
                                auto v = 260 - i * 16;
                                if (v < 0) {
                                    v += 360;
                                }
                                dir = rotate({1, 0}, v);
                            }
                            auto spd = 0.5f * (1 + (i % 2));
                            s->set_speed({Fixnum(dir.x * spd), Fixnum(dir.y * spd)});
                        }
                    }
                } else {
                    auto o = position_;
                    o.x -= 2;
                    o.y -= 2;
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        Vec2<Float> dir;
                        if (x() < engine().hero()->x()) {
                            auto v = rng::choice<160>(rng::utility_state) - 80;
                            if (v < 0) {
                                v += 360;
                            }
                            dir = rotate({1, 0}, v);
                        } else {
                            auto v = rng::choice<160>(rng::utility_state) + 100;
                            if (v < 0) {
                                v += 360;
                            }
                            dir = rotate({1, 0}, v);
                        }
                        auto d = rotate(dir, 20);
                        s->set_speed({Fixnum(d.x * 0.5f), Fixnum(d.y * 0.5f)});
                    }

                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        auto dir = direction(fvec(o), fvec(engine().hero()->position()));
                        s->set_speed({Fixnum(dir.x * 0.75f), Fixnum(dir.y * 0.75f)});
                    }
                }
            } else {
                if (not shot_40_) {
                    shot_40_ = true;
                    auto o = position_;
                    o.x -= 2;
                    o.y -= 2;
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({-0.25f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({-0.5f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({0.25f, 0.f});
                    }
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        s->set_speed({0.5f, 0.f});
                    }
                }
                timcyc_ += 1;
                if (timcyc_ == 16) {
                    timcyc_ = 0;
                }

                auto o = position_;
                o.x -= 2;
                o.y -= 2;

                if (timcyc_ % 8 == 0) {
                    auto dir = direction(fvec(o), fvec(engine().hero()->position()));

                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 300);
                        s->set_speed({Fixnum(d.x * 0.25f), Fixnum(d.y * 0.25f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 60);
                        s->set_speed({Fixnum(d.x * 0.25f), Fixnum(d.y * 0.25f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 320);
                        s->set_speed({Fixnum(d.x * 0.5f), Fixnum(d.y * 0.5f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 40);
                        s->set_speed({Fixnum(d.x * 0.5f), Fixnum(d.y * 0.5f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 340);
                        s->set_speed({Fixnum(d.x * 0.75f), Fixnum(d.y * 0.75f)});
                    }
                    if (auto s = engine().add_object<Supershot>(o)) {
                        auto d = rotate(dir, 20);
                        s->set_speed({Fixnum(d.x * 0.75f), Fixnum(d.y * 0.75f)});
                    }
                } else if (timcyc_ % 4 == 0) {
                    auto o = position_;
                    o.x -= 2;
                    o.y -= 2;
                    for (int i = 1; i < 10; ++i) {
                        if (auto s = engine().add_object<Supershot>(o)) {
                            Vec2<Float> dir;
                            if (x() < engine().hero()->x()) {
                                auto v = -80 + i * 16;
                                if (v < 0) {
                                    v += 360;
                                }
                                dir = rotate({1, 0}, v);
                            } else {
                                auto v = 260 - i * 16;
                                if (v < 0) {
                                    v += 360;
                                }
                                dir = rotate({1, 0}, v);
                            }
                            auto spd = 0.5f * (1 + (i % 2));
                            s->set_speed({Fixnum(dir.x * spd), Fixnum(dir.y * spd)});
                        }
                    }
                } else {
                    auto o = position_;
                    o.x -= 2;
                    o.y -= 2;
                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        Vec2<Float> dir;
                        if (x() < engine().hero()->x()) {
                            auto v = rng::choice<160>(rng::utility_state) - 80;
                            if (v < 0) {
                                v += 360;
                            }
                            dir = rotate({1, 0}, v);
                        } else {
                            auto v = rng::choice<160>(rng::utility_state) + 100;
                            if (v < 0) {
                                v += 360;
                            }
                            dir = rotate({1, 0}, v);
                        }
                        auto d = rotate(dir, 20);
                        s->set_speed({Fixnum(d.x * 0.5f), Fixnum(d.y * 0.5f)});
                    }

                    if (auto s = engine().add_object<EnemyShot>(o)) {
                        auto dir = direction(fvec(o), fvec(engine().hero()->position()));
                        s->set_speed({Fixnum(dir.x * 0.75f), Fixnum(dir.y * 0.75f)});
                    }
                }
            }
            break;

        case 109:
            timeline_ = 40;
            break;
        }
    }


    bool damage(Health dmg, Object& s) override
    {
        platform().speaker().play_sound("snd_hit3", 1);
        health_ = std::max(0, health_ - dmg);
        s.kill();
        if (engine().g_.screenshake_ < 2) {
            engine().g_.screenshake_ = 2;
        }
        return true;
    }


    int collision_damage() const override
    {
        return 4;
    }
};


} // namespace herocore
