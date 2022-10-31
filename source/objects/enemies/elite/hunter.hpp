#pragma once

#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/enemies/light/spew.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/vortex.hpp"


namespace herocore
{


class HunterDormant : public Solid
{
public:
    HunterDormant(const Vec2<Fixnum>& pos)
    {
        position_ = pos;
        position_.x -= 8;
        position_.y -= 8;
        sprite_index_ = 126;
        hitbox_.dimension_.size_ = {14, 14};
    }
};


class Hunter : public Enemy
{
private:
    u16 dir_ = 0;
    u16 fireoffset_ = 0;

public:
    Hunter(const Vec2<Fixnum>& pos) : Enemy(TaggedObject::Tag::hunter, Health{32})
    {
        position_ = pos;

        sprite_index_ = 126;

        hitbox_.dimension_.size_ = {14, 14};
        hitbox_.dimension_.origin_ = {8, 8};
        origin_ = {8, 8};
        position_.x += 8;
        position_.y += 8;
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            if (engine().g_.screenshake_ < 6) {
                engine().g_.screenshake_ = 6;
            }
            platform().speaker().play_sound("snd_explo3", 6);
            engine().add_object<BigExplo>(Vec2<Fixnum>{position_.x, position_.y});
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

        case 40: {
            if (not flash_) {
                if (x() > engine().hero()->x()) {
                    sprite_subimage_ = 2;
                } else {
                    sprite_subimage_ = 3;
                }
            }
            {
                auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
                dir = dir * 0.5f;
                speed_.x = Fixnum(dir.x);
                speed_.y = Fixnum(dir.y);
            }
            if (auto s = engine().add_object<Supershot>(Vec2<Fixnum>{x() - 4, y() - 4})) {
                auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
                dir = dir * 1.5f;
                s->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
            }
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (auto s =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 4, y() - 4})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = rotate(dir, 30);
                    s->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
                if (auto s =
                        engine().add_object<Supershot>(Vec2<Fixnum>{x() - 4, y() - 4})) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = rotate(dir, 330);
                    s->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            } else {
                if (auto s = engine().add_object<EnemyShot>(position_)) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = rotate(dir, 30);
                    s->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
                if (auto s = engine().add_object<EnemyShot>(position_)) {
                    auto dir =
                        direction(fvec(position_), fvec(engine().hero()->position()));
                    dir = rotate(dir, 330);
                    s->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }

            break;
        }

        case 90: {
            if (x() > engine().hero()->x()) {
                sprite_subimage_ = 2;
            } else {
                sprite_subimage_ = 3;
            }
            auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
            auto dir2 = dir;
            dir = rotate(dir, 135 + rng::choice<90>(rng::utility_state));
            speed_.x = Fixnum(0.75f * dir.x);
            speed_.y = Fixnum(0.75f * dir.y);
            EnemyProjectile* p = nullptr;
            if (engine().g_.difficulty_ == Difficulty::hard) {
                p = engine().add_object<Megashot>(Vec2<Fixnum>{x() - 8, y() - 8});
                p->set_speed({Fixnum(dir2.x), Fixnum(dir2.y)});
            } else {
                p = engine().add_object<Supershot>(Vec2<Fixnum>{x() - 4, y() - 4});
                p->set_speed({Fixnum(dir2.x * 0.25f), Fixnum(dir2.y * 0.25f)});
            }
            break;
        }

        case 140: {
            fireoffset_ = 0;
            if (fireoffset_ == 0) {
                fireoffset_ = 45;
            } else {
                fireoffset_ = 0;
            }
            if (engine().g_.difficulty_ == Difficulty::hard) {
                for (int i = 0; i < 8; ++i) {
                    if (auto e = engine().add_object<EnemyShot>(position_)) {
                        auto dir = rotate({1, 0}, i * 45);
                        e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                    }
                }
            } else {
                for (int i = 0; i < 4; ++i) {
                    if (auto e = engine().add_object<EnemyShot>(position_)) {
                        auto dir = rotate({1, 0}, i * 90 + fireoffset_);
                        e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                    }
                }
            }

            if (x() > engine().hero()->x()) {
                sprite_subimage_ = 2;
            } else {
                sprite_subimage_ = 3;
            }
            flash_ = true;
            play_sound("snd_charge", 4);
            break;
        }

        case 190:
            flash_ = false;
            if (TaggedObject::count(Tag::spew) < 3) {
                engine().add_object<Spew>(position_);
            }

            if (engine().g_.difficulty_ == Difficulty::hard) {
                for (int i = 0; i < 4; ++i) {
                    if (auto e = engine().add_object<Supershot>(
                            Vec2<Fixnum>{x() - 4, y() - 4})) {
                        int d = i * 90 + fireoffset_;
                        if (d > 360) {
                            d -= 360;
                        }
                        auto dir = rotate({1, 0}, d);
                        e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                    }
                }
            }

            if (x() > engine().hero()->x()) {
                if (auto v =
                        engine().add_object<Vortex>(Vec2<Fixnum>{x() + 4, y() - 4})) {
                    v->set_left();
                }
            } else {
                engine().add_object<Vortex>(Vec2<Fixnum>{x() - 8, y() - 4});
            }
            break;

        case 239:
            timeline_ = 40;
            break;
        }

        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            auto x = position_.x;
            auto y = position_.y;
            if (not place_free({x + 2, y}) or not place_free({x - 2, y})) {
                speed_.x = speed_.x * -1;
            }
            if (not place_free({x, y + 2}) or not place_free({x, y - 2})) {
                speed_.y = speed_.y * -1;
            }
        }

        Enemy::step();
    }


    void draw(Platform::Screen& screen) const override
    {
        if (flash_) {
            flashcyc_ += 1;
            if (flashcyc_ == 2) {
                flashcyc_ = 0;
                Sprite spr_;
                spr_.set_texture_index(52);
                if (sprite_subimage_ == 3) {
                    spr_.set_flip({true, false});
                    spr_.set_position({position_.x - 3, position_.y - 11});
                } else {
                    spr_.set_position({position_.x - 13, position_.y - 11});
                }

                screen.draw(spr_);

                spr_.set_texture_index(53);

                if (sprite_subimage_ == 3) {
                    spr_.set_position({position_.x - 3, position_.y + 5});
                } else {
                    spr_.set_position({position_.x - 13, position_.y + 5});
                }

                screen.draw(spr_);
            }
        }

        Enemy::draw(screen);
    }


    int collision_damage() const override
    {
        return 4;
    }


private:
    bool flash_ = false;
    bool freeze_ = false;
    int timeline_ = 0;
    int move_ = 0;
    int shotcyc_ = 0;

    // NOTE: the draw call in the gamemaker game updates flashcyc. Just doing
    // the same to mirror the original layout of the code.
    mutable int flashcyc_ = 0;
};


} // namespace herocore
