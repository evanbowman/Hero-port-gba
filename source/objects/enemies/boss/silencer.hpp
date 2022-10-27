#pragma once

#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/enemies/light/drone.hpp"
#include "objects/enemies/light/spew.hpp"
#include "objects/misc/pickup.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/megashot.hpp"


namespace herocore
{


class SilencerCore : public Enemy
{
private:
    Enemy* owner_ = nullptr;
    int animcyc_ = 0;
    u8 spawn_x_;
    u8 spawn_y_;

public:
    SilencerCore(Enemy* owner, u8 spawn_x, u8 spawn_y)
        : Enemy(TaggedObject::Tag::ignored, Health{32}), owner_(owner), spawn_x_(spawn_x),
          spawn_y_(spawn_y)
    {
        hitbox_.dimension_.size_ = {4, 4};
        hitbox_.dimension_.origin_ = {3, 3};
        origin_ = {3, 3};

        sprite_index_ = 164;
    }


    int hit_sound() const override
    {
        return 4;
    }


    void step() override
    {
        if (health_ <= 0) {
            engine().add_object<ExploSpewer>(Vec2<Fixnum>{x(), y()});
            kill();
            owner_->destroy();
            platform().speaker().play_sound("snd_bossroar", 9);
            platform().speaker().stop_music();


            engine().g_.screenshake_ = 6;
        }

        animcyc_ += 1;
        if (animcyc_ == 4) {
            animcyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 4) {
                sprite_subimage_ = 0;
            }
        }

        position_.x = owner_->position().x;
        position_.y = owner_->position().y;
    }
};


class Silencer : public Enemy
{
private:
    Fixnum spd_ = 0.f;
    int move_ = 0;

    Vec2<float> dir_ = {0, 1};
    u8 deadcyc_ = 0;

public:
    u8 spawn_x_;
    u8 spawn_y_;

private:
    struct Shield
    {
        u8 health_ = 2;
        s8 x_offset_ = 0;
    };

    using Shields = std::array<Shield, 10>;
    DynamicMemory<Shields> shields_;


public:
    Silencer(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y)
        : Enemy(TaggedObject::Tag::ignored, Health{32}), spawn_x_(spawn_x),
          spawn_y_(spawn_y), shields_(allocate_dynamic<Shields>(platform()))
    {
        position_ = pos;

        sprite_index_ = 162;

        hitbox_.dimension_.size_ = {32, 16};
        hitbox_.dimension_.origin_ = {16, 8};
        origin_ = {16, 8};

        engine().add_object<SilencerCore>(this, spawn_x_, spawn_y_);


        (*shields_)[9].x_offset_ = -5;
        (*shields_)[8].x_offset_ = -7;
        (*shields_)[7].x_offset_ = -9;
        (*shields_)[6].x_offset_ = -11;
        (*shields_)[5].x_offset_ = -13;

        (*shields_)[4].x_offset_ = 4;
        (*shields_)[3].x_offset_ = 6;
        (*shields_)[2].x_offset_ = 8;
        (*shields_)[1].x_offset_ = 10;
        (*shields_)[0].x_offset_ = 12;

        speed_.y = Fixnum(0.001f);
    }


    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(sprite_index_ + sprite_subimage_);
        spr_.set_position(position_);
        screen.draw(spr_);

        spr_.set_position({x() + 16, y()});
        spr_.set_flip({true, false});
        screen.draw(spr_);

        {
            Fixnum posx = position_.x;

            Sprite spr;
            spr.set_texture_index(163);

            for (auto& s : *shields_) {
                if (s.health_ > 0) {
                    spr.set_position({posx + s.x_offset_, y() - 2});
                    screen.draw(spr);
                }
            }
        }
    }


    bool damage(Health dmg, Object& s)
    {
        Hitbox hb;
        hb.dimension_.size_ = {32, 5};
        hb.dimension_.origin_ = hitbox_.dimension_.origin_;
        auto p = position_;
        hb.position_ = &p;
        if (hb.overlapping(s.hitbox())) {
            s.kill();
            return false;
        }

        p.y += 11;
        if (hb.overlapping(s.hitbox())) {
            s.kill();
            return false;
        }

        Hitbox sh;
        Vec2<Fixnum> pos;
        sh.position_ = &pos;

        for (auto& s_ : *shields_) {
            if (s_.health_ > 0) {
                pos.x = x() + s_.x_offset_;
                pos.y = y() - 2;
                sh.dimension_.size_ = {2, 4};
                sh.dimension_.origin_ = {0, 0};
                if (s_.x_offset_ > 0) {
                    sh.dimension_.origin_ = {-2, 0};
                }
                if (sh.overlapping(s.hitbox())) {
                    s.kill();
                    platform().speaker().play_sound("snd_hit2", 1);
                    s_.health_ = std::max(0, (int)s_.health_ - dmg);
                    return true;
                }
            }
        }

        return false;
    }


    void step() override
    {
        if (health_ <= 0) {
            deadcyc_ += 1;
            if (place_free({x(), y() + 1})) {
                y() += 1;
                if (deadcyc_ % 6 == 0 and deadcyc_ < 80) {
                    engine().g_.screenshake_ = 6;
                    if (auto exp = engine().add_object<BigExplo>(
                            rng::sample<16>(position_, rng::critical_state))) {
                        exp->set_speed({0, -rng::choice<3>(rng::utility_state)});
                        engine().g_.screenshake_ = 6;
                        platform().speaker().play_sound("snd_explo2", deadcyc_ / 8);
                    }
                }
                if (deadcyc_ == 80 and place_free({x(), y() + 1})) {
                    deadcyc_ -= 6;
                }
                return;
            } else if (deadcyc_ >= 90) {

                engine().g_.screenshake_ = 12;

                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        if (auto exp = engine().add_object<BigExplo>(position_)) {
                            auto dir = rotate({1, 0}, j * 90 + 45 + i * 3);
                            dir = dir * ((i + 1 / 2.f) * 1.5f);
                            Vec2<Fixnum> spd;
                            spd.x = Fixnum(dir.x);
                            spd.y = Fixnum(dir.y);
                            exp->set_speed(spd);
                        }
                    }
                }

                engine().add_object<ExploSpewer>(position_);

                engine().p_->objects_removed_.push_back({(u8)engine().room_.coord_.x,
                                                         (u8)engine().room_.coord_.y,
                                                         spawn_x_,
                                                         spawn_y_});

                engine().boss_completed();

                for (auto& e : engine().enemies_) {
                    e->kill();
                }

                engine().add_object<Pickup>(position_, Pickup::blade);

                platform().speaker().play_sound("snd_explo4", 30);
                return;
            }
        }

        const bool hard = engine().g_.difficulty_ == Difficulty::hard;

        switch (timeline_++) {
        case 0:
            engine().swapsong("boss");
            break;

        case 10:
            break;

        case 20:
            break;

        case 30:
            break;

        case 40:
            move_ = 1;
            break;

        case 80:
        case 90: {
            int sp_count = 2;
            if (hard) {
                sp_count = 3;
            }
            if (TaggedObject::count(Tag::drone) < sp_count) {
                engine().add_object<Drone>(Vec2<Fixnum>{x() - 4, y() - 4});
            }
            break;
        }
        case 100:
            if (hard) {
                if (TaggedObject::count(Tag::drone) < 3) {
                    engine().add_object<Drone>(Vec2<Fixnum>{x() - 4, y() - 4});
                }
            }
            break;

        case 600:
            if (not hard) {
                break;
            }
            spd_ = 0;
            speed_ = {};
            move_ = 0;
            for (int i = 0; i < 4; ++i) {
                if (auto e =
                        engine().add_object<Megashot>(Vec2<Fixnum>{x() - 4, y() - 4})) {
                    e->set_speed(0.5f, i * 90 + 45);
                }
                if (auto e =
                        engine().add_object<EnemyShot>(Vec2<Fixnum>{x() - 2, y() - 2})) {
                    e->set_speed(0.5f, i * 90);
                }
                if (auto e =
                        engine().add_object<EnemyShot>(Vec2<Fixnum>{x() - 2, y() - 2})) {
                    e->set_speed(1.f, i * 90);
                }
            }
            break;

        case 640:
        case 240:
            if (timeline_ - 1 == 640 and hard) {
                break;
            }
            spd_ = 0;
            speed_ = {};
            move_ = 0;
            if (hard) {
                for (int i = 0; i < 5; ++i) {
                    if (auto e = engine().add_object<EnemyShot>(
                            Vec2<Fixnum>{x() - 2, y() - 2})) {
                        e->set_speed(1.8f / 2, i * 22 - 45);
                    }
                    if (auto e = engine().add_object<EnemyShot>(
                            Vec2<Fixnum>{x() - 2, y() - 2})) {
                        e->set_speed(1.8f / 2, i * 22 + 135);
                    }
                }
            } else {
                for (int i = 0; i < 3; ++i) {
                    if (i not_eq 1) {
                        if (auto e = engine().add_object<EnemyShot>(
                                Vec2<Fixnum>{x() - 2, y() - 2})) {
                            e->set_speed(1.5f / 2, i * 45 - 45);
                        }
                        if (auto e = engine().add_object<EnemyShot>(
                                Vec2<Fixnum>{x() - 2, y() - 2})) {
                            e->set_speed(1.5f / 2, i * 45 + 135);
                        }
                    }
                }
            }
            break;

        case 400:
            if (hard) {
                move_ = 2;
            }
            break;

        case 440:
        case 450:
            move_ = 2;
            if (not hard) {
                break;
            }
            if (TaggedObject::count(Tag::spew) < 2) {
                engine().add_object<Spew>(Vec2<Fixnum>{x() - 4, y() - 4});
            }
            break;


        case 480:
        case 490:
            if (hard) {
                break;
            }
            if (TaggedObject::count(Tag::spew) < 2) {
                engine().add_object<Spew>(Vec2<Fixnum>{x() - 4, y() - 4});
            }
            break;

        case 839:
            timeline_ = 40;
            break;
        }

        if (not place_free({position_.x + speed_.x, position_.y + speed_.y}, 3)) {
            auto x = position_.x;
            auto y = position_.y;
            if (not place_free({x + 3, y}, 3) or not place_free({x - 3, y}, 3)) {
                dir_.x = dir_.x * -1.f;
            }
            if (not place_free({x, y + 2}, 3) or not place_free({x, y - 2}, 3)) {
                dir_.y = dir_.y * -1.f;
            }
        }

        Enemy::step();

        Fixnum maxspeed = 1;
        if (engine().g_.difficulty_ == Difficulty::hard) {
            maxspeed = Fixnum(1.f + 1.f / 3);
        }

        if (move_ == 1) {
            if (spd_ < maxspeed) {
                spd_ += Fixnum(0.05f / 2);
            }
            dir_ = rotate(dir_, 1);
        }
        if (move_ == 2) {
            if (spd_ < maxspeed) {
                spd_ += Fixnum(0.05f / 2);
            }
            dir_ = rotate(dir_, 359);
        }

        if (spd_ > 0) {
            trailcyc_ += 1;
            if (trailcyc_ == 30) {
                trailcyc_ = 0;
                EnemyProjectile* e;
                if (engine().g_.difficulty_ == Difficulty::hard) {
                    e = engine().add_object<Supershot>(Vec2<Fixnum>{x() - 2, y() - 2});
                } else {
                    e = engine().add_object<EnemyShot>(Vec2<Fixnum>{x() - 2, y() - 2});
                }

                if (e) {

                    auto d2 = rotate(dir_, 180);
                    d2 = d2 * 0.5f;
                    e->set_speed({Fixnum(d2.x), Fixnum(d2.y)});
                }
            }
        }

        {
            speed_.x = spd_ * Fixnum(dir_.x);
            speed_.y = spd_ * Fixnum(dir_.y);
        }
    }


private:
    int trailcyc_ = 0;
    int timeline_ = 0;
};


} // namespace herocore
