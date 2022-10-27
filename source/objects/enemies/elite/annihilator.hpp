#pragma once

#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/gigashot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/supershot.hpp"


namespace herocore
{


class Annihilator : public Enemy
{
private:
    int shotcyc_ = 0;
    bool flash_ = false;
    bool spreadfire_ = false;
    int sfcyc_ = 15;
    int sfcyc2_ = 0;
    u8 spawn_x_;
    u8 spawn_y_;
    bool dead_ = false;
    u8 deadcyc_ = 0;

public:
    Annihilator(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y)
        : Enemy(TaggedObject::Tag::ignored, Health{128}), spawn_x_(spawn_x),
          spawn_y_(spawn_y)
    {
        position_ = pos;

        sprite_index_ = 150;
        sprite_subimage_ = 0;

        hitbox_.dimension_.size_ = {30, 30};
        hitbox_.dimension_.origin_ = {16, 16};
    }


    void draw(Platform::Screen& s) const
    {
        Sprite spr_;
        if (hflip_) {
            spr_.set_origin({origin_.x, origin_.y});
            spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 4);
            spr_.set_position({x(), y() - 16});
            spr_.set_flip({true, false});
            s.draw(spr_);

            spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 4 + 1);
            spr_.set_position({x() - 16, y() - 16});
            s.draw(spr_);

            spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 4 + 2);
            spr_.set_position({x(), y()});
            s.draw(spr_);

            spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 4 + 3);
            spr_.set_position({x() - 16, y()});
            s.draw(spr_);
        } else {
            spr_.set_origin({origin_.x, origin_.y});
            spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 4);
            spr_.set_position({x() - 16, y() - 16});
            s.draw(spr_);

            spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 4 + 1);
            spr_.set_position({x(), y() - 16});
            s.draw(spr_);

            spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 4 + 2);
            spr_.set_position({x() - 16, y()});
            s.draw(spr_);

            spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 4 + 3);
            spr_.set_position({x(), y()});
            s.draw(spr_);
        }
    }


    bool damage(Health dmg, Object& s) override
    {
        if (dead_) {
            s.kill();
            return false;
        }

        s.kill();

        if (sprite_subimage_ not_eq 2) {
            return false;
        }

        Vec2<Fixnum> position = position_;
        if (hflip_) {
            position.x += 16;
        } else {
            position.x -= 3;
        }

        Hitbox hb = hitbox_;
        hb.position_ = &position;
        hb.dimension_.size_ = {14, 13};
        // head
        if (hb.overlapping(s.hitbox())) {
            health_ = std::max(0, health_ - dmg);
            play_sound("snd_hit1", 1);
            return true;
        }


        hb.dimension_.size_ = {10, 11};

        position = position_;
        if (hflip_) {
            position.x += 0;
        } else {
            position.x += 22;
        }
        position.y += 21;
        // tail
        if (hb.overlapping(s.hitbox())) {
            health_ = std::max(0, health_ - dmg);
            play_sound("snd_hit1", 1);
            return true;
        }

        return false;
    }


    void step() override
    {
        if (health_ == 0 and not dead_) {
            dead_ = true;
            return;
        }

        if (dead_) {

            deadcyc_ += 1;
            if (deadcyc_ % 4 == 0) {
                auto p = position_;
                auto sp = rng::sample<32>(p, rng::critical_state);
                if (engine().add_object<BigExplo>(sp)) {
                    play_sound("snd_explo1", 20 + deadcyc_ / 4);
                    engine().g_.screenshake_ = 2;
                }
            }

            if (deadcyc_ == 80) {
                kill();
                engine().add_object<Explo>(position_);

                engine().g_.screenshake_ = 6;

                play_sound("snd_explo4", 80);

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

                engine().p_->objects_removed_.push_back({(u8)engine().room_.coord_.x,
                                                         (u8)engine().room_.coord_.y,
                                                         spawn_x_,
                                                         spawn_y_});

                return;
            }
            return;
        }

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
            sprite_subimage_ = 2;
            if (x() < engine().hero()->x()) {
                hflip_ = true;
            } else {
                hflip_ = false;
            }
            break;

        case 140:
            if (not hflip_) {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 140);
                }
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 220);
                }
            } else {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 40);
                }
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 360 - 40);
                }
            }
            break;

        case 160:
            speed_ = {};
            if (not hflip_) {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 80);
                }
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 200);
                }
            } else {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 100);
                }
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 360 - 100);
                }
            }
            break;

        case 80:
        case 200: {
            if (x() < engine().hero()->x()) {
                hflip_ = true;
            } else {
                hflip_ = false;
            }
            {
                auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
                speed_.x = Fixnum(0.5f * dir.x);
                speed_.y = Fixnum(0.5f * dir.y);
            }
            for (int i = 0; i < 4; ++i) {
                if (auto e =
                        engine().add_object<EnemyShot>(Vec2<Fixnum>{x() - 1, y() - 1})) {
                    e->set_speed(0.25f, i * 90);
                }
            }
            if (not hflip_) {
                if (auto e =
                        engine().add_object<Gigashot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            } else {
                if (auto e =
                        engine().add_object<Gigashot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
            auto s2 = [&](const Vec2<Fixnum>& pos) -> EnemyProjectile* {
                if (engine().g_.difficulty_ == Difficulty::hard) {
                    return engine().add_object<Megashot>(pos);
                } else {
                    return engine().add_object<Supershot>(pos);
                }
            };
            if (not hflip_) {
                if (auto e = s2(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = rotate(dir, 30);
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            } else {
                if (auto e = s2(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = rotate(dir, 30);
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
            if (not hflip_) {
                if (auto e = s2(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = rotate(dir, 330);
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            } else {
                if (auto e = s2(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = rotate(dir, 330);
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
            break;
        }

        case 120:
        case 240: {
            auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
            int amount = 90 - rng::choice<180>(rng::utility_state);
            if (amount < 0)
                amount += 360;
            dir = rotate(dir, amount);
            dir = dir * (1.5f / 2);
            speed_.x = Fixnum(dir.x);
            speed_.y = Fixnum(dir.y);
        }
            if (not hflip_) {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 180);
                }
            } else {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 0);
                }
            }
            break;

        case 260:
            if (not hflip_) {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 140);
                }
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 220);
                }
            } else {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 40);
                }
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 360 - 40);
                }
            }
            break;

        case 280:
            speed_ = {};
            if (not hflip_) {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 80);
                }
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 20, y() - 2})) {
                    e->set_speed(1, 280);
                }
            } else {
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 100);
                }
                if (auto e =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() + 16, y() - 2})) {
                    e->set_speed(1, 360 - 100);
                }
            }
            break;

        case 320: {
            auto dir = direction(fvec(position_), Vec2<float>{40 + 80, 80.f});
            speed_.x = Fixnum((1.5f / 2) * dir.x);
            speed_.y = Fixnum((1.5f / 2) * dir.y);
            flash_ = true;
            if (x() < engine().hero()->x()) {
                hflip_ = true;
            } else {
                hflip_ = false;
            }
            break;
        }

        case 360:
            spreadfire_ = true;
            speed_.x = 0;
            speed_.y = 0;
            flash_ = 0;
            sprite_subimage_ = 1;
            break;

        case 440:
            spreadfire_ = false;
            break;

        case 480:
            sprite_subimage_ = 2;
            if (x() < engine().hero()->x()) {
                hflip_ = true;
            } else {
                hflip_ = false;
            }
            break;

        case 519:
            timeline_ = 80;
            break;
        }

        if (spreadfire_) {
            if (engine().g_.difficulty_ == Difficulty::hard) {
                sfcyc_ += 2;
            } else {
                sfcyc_ += 1;
            }
            if (sfcyc_ >= 16) {
                sfcyc_ = 0;
                if (auto e =
                        engine().add_object<Gigashot>(Vec2<Fixnum>{x() - 8, y() - 8})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = dir * 2.f;
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                    sfcyc2_ += 1;
                    if (sfcyc2_ == 2) {
                        sfcyc2_ = 0;
                    }
                    if (engine().g_.difficulty_ == Difficulty::hard and sfcyc2_ == 1) {
                        if (auto e = engine().add_object<Supershot>(position_)) {
                            e->set_speed(1, rng::choice<360>(rng::utility_state));
                        }
                    }
                }
            }
        }


        auto x = position_.x;
        auto y = position_.y;
        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            if (not place_free({x + 2, y}) or not place_free({x - 2, y})) {
                speed_.x = speed_.x * -1;
            }
            if (not place_free({x, y + 2}) or not place_free({x, y - 2})) {
                speed_.y = speed_.y * -1;
            }
        }

        Enemy::step();
    }


private:
    int timeline_ = 0;
};


} // namespace herocore
