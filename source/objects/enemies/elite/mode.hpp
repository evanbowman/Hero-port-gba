#pragma once

#include "objects/enemies/enemy.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/missile.hpp"
#include "objects/projectile/vortex.hpp"
#include "objects/projectile/bomb.hpp"
#include "objects/particles/bigExplo.hpp"
#include "engine.hpp"
#include "number/random.hpp"



namespace herocore
{



class Mode;



class ModeNeck : public Enemy
{
private:
    Mode* owner_;
    Fixnum rot_ = 0;
    Fixnum d_ = 0;
    int dmax_ = 16;

    // NOTE: this diverges from the original code. Rendering lasers (vortex)
    // requires lots of sprites, and drawing five lasers at once significantly
    // lags the engine. I've decided to stagger the lasers by an offset, kind of
    // like the warp-eidolon does, due to hardware constraints.
    int offset_ = 0;

    mutable u8 flashcyc_ = 0;

public:

    ModeNeck(Mode* owner, Fixnum rot, int offset);


    void step() override;
    void draw(Platform::Screen&) const override;


    Mode* owner()
    {
        return owner_;
    }


};



class Mode : public Enemy
{
private:

    int timeline_ = 0;
    bool neckinit_ = false;
    bool flash_ = false;


public:

    Mode(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::ignored, Health{32})
    {
        position_ = pos;

        sprite_index_ = 123;
        sprite_subimage_ = 0;
        hitbox_.dimension_.size_ = {14, 14};
        hitbox_.dimension_.origin_ = {7, 7};
        origin_ = {7, 7};
        position_.x += 7;
        position_.y += 7;
    }


    bool flash() const
    {
        return flash_;
    }


    int timeline() const
    {
        return timeline_;
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            platform().speaker().play_sound("snd_explo3", 6);
            if (engine().g_.screenshake_ < 6) {
                engine().g_.screenshake_ = 6;
            }
            engine().add_object<BigExplo>(position_);
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() - 4, y() - 4});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() + 4, y() - 4});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() - 4, y() + 4});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() + 4, y() + 4});

            for (auto& e : engine().enemies_) {
                if (auto m = dynamic_cast<ModeNeck*>(e.get())) {
                    if (m->owner() == this) {
                        engine().add_object<BigExplo>(e->position());
                        e->kill();
                    }
                }
            }
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

        case 40:
            if (not neckinit_) {
                if (engine().g_.difficulty_ == Difficulty::normal) {
                    for (int i = 0; i < 3; ++i) {
                        engine().add_object<ModeNeck>(this, Fixnum(i * 120), 0);
                    }
                } else {
                    for (int i = 0; i < 5; ++i) {
                        engine().add_object<ModeNeck>(this, Fixnum(i * 72),
                                                      -20 + i * 9);
                    }
                }

                neckinit_ = true;
            }
            {
                auto dir = rotate({1, 0}, rng::choice<360>(rng::utility_state));
                dir = dir * 0.5f;
                speed_.x = Fixnum(dir.x);
                speed_.y = Fixnum(dir.y);
            }
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir(rotate({1, 0}, 45));
                }
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir(rotate({1, 0}, 135));
                }
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir(rotate({1, 0}, 225));
                }
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir(rotate({1, 0}, 315));
                }
            } else {
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir({0, -1});
                }
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir({0, 1});
                }
            }
            break;

        case 120:
            speed_ = {};
            for (int i = 0; i < 5; ++i) {
                if (auto b = engine().add_object<Bomb>(position_)) {
                    auto d = rotate({1, 0}, i * 45);
                    b->set_speed({Fixnum(d.x), Fixnum(d.y)});
                }
            }
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (auto em = engine().add_object<Megashot>(position_)) {
                    auto dir = direction(fvec(position_),
                                         fvec(engine().hero()->position()));

                    dir = dir * (2.5f / 2);
                    em->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
            break;

        case 135:
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (auto em = engine().add_object<Megashot>(position_)) {
                    auto dir = direction(fvec(position_),
                                         fvec(engine().hero()->position()));

                    dir = dir * (2.5f / 2);
                    em->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
            break;

        case 150:
            for (int i = 0; i < 5; ++i) {
                if (auto b = engine().add_object<Bomb>(position_)) {
                    auto d = rotate({1, 0}, i * 45);
                    d = d * 1.5f;
                    b->set_speed({Fixnum(d.x), Fixnum(d.y)});
                }
            }
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (auto em = engine().add_object<Megashot>(position_)) {
                    auto dir = direction(fvec(position_),
                                         fvec(engine().hero()->position()));

                    dir = dir * (2.5f / 2);
                    em->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
            break;

        case 180:
            {
                auto dir = rotate({1, 0}, rng::choice<360>(rng::utility_state));
                dir = dir * 0.5f;
                speed_.x = Fixnum(dir.x);
                speed_.y = Fixnum(dir.y);
            }
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir({1, 0});
                }
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir({-1, 0});
                }
            } else {
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir({0, -1});
                }
                if (auto m = engine().add_object<Missile>(Vec2<Fixnum>{x() + 4, y()})) {
                    m->set_dir({0, 1});
                }
            }
            break;

        case 240:
            if (engine().g_.difficulty_ == Difficulty::hard) {
                flash_ = true;
            }
            break;

        case 260:
            flash_ = true;
            break;

        case 300:
            flash_ = false;
            break;

        case 339:
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
        return 4;
    }



};



inline ModeNeck::ModeNeck(Mode* owner, Fixnum rot, int offset) :
        Enemy(TaggedObject::Tag::ignored, Health{16}),
        owner_(owner),
        rot_(rot),
        offset_(offset)
{
    position_ = owner_->position();
    sprite_index_ = 125;

    hitbox_.dimension_.size_ = {8, 8};
    hitbox_.dimension_.origin_ = {4, 4};
    origin_ = {4, 4};
}



inline void ModeNeck::draw(Platform::Screen& s) const
{
    Enemy::draw(s);

    if (owner_->timeline() > 260 + offset_ and owner_->timeline() < 300 + offset_) {
        flashcyc_ += 1;
        if (flashcyc_ == 2) {
            flashcyc_ = 0;

            Sprite spr_;
            spr_.set_texture_index(52);
            if (x() < engine().hero()->x()) {
                spr_.set_flip({true, false});
                spr_.set_position({position_.x - 6, position_.y - 12});
            } else {
                spr_.set_position({position_.x - 10, position_.y - 12});
            }

            s.draw(spr_);

            spr_.set_texture_index(53);

            if (x() < engine().hero()->x()) {
                spr_.set_position({position_.x - 6, position_.y + 4});
            } else {
                spr_.set_position({position_.x - 10, position_.y + 4});
            }

            s.draw(spr_);
        }
    }

}



inline void ModeNeck::step()
{
    if (health_ == 0) {
        kill();
        engine().add_object<BigExplo>(position_);
        return;
    }

    if (owner_->timeline() == 300 + offset_) {
        if (x() > engine().hero()->x()) {
            if (auto v = engine().add_object<Vortex>(Vec2<Fixnum>{x(),
                                                                  y() - 4})) {
                v->set_left();
            }
        } else {
            engine().add_object<Vortex>(Vec2<Fixnum>{x() - 4, y() - 4});
        }

    }

    if (d_ < dmax_) {
        d_ = d_ + Fixnum(0.25f);
    }
    if (d_ > dmax_) {
        d_ = d_ - Fixnum(0.25f);
    }

    if (engine().g_.difficulty_ == Difficulty::normal) {
        rot_ = rot_ + Fixnum(1.5f);
    } else {
        rot_ = rot_ + Fixnum(2.f);
    }

    if (rot_ > 360) {
        rot_ = rot_ - 360;
    }

    static constexpr auto rot_lut = make_rotation_lut(0);
    auto rvec = rot_lut[rot_.as_integer()];

    position_ = owner_->position() + rvec * d_;
    position_.x += Fixnum(0.5f);
    position_.y += Fixnum(0.5f);
}



}
