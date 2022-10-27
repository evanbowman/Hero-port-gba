#include "guardian.hpp"
#include "fmt.hpp"
#include "objects/enemies/elite/hunter.hpp"
#include "objects/enemies/heavy/soldier.hpp"
#include "objects/misc/pickup.hpp"
#include "objects/projectile/bomb.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/gigashot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/vortex.hpp"


namespace herocore
{


// obj_guardianneck == GuardianUpper and GuardianLower
// obj_guardian == GuardianCore


class GuardianUpper : public Enemy
{
private:
    bool detaching_ = false;
    u8 shotcyc_ = 0;
    int timeline_ = 0;

public:
    GuardianUpper(GuardianCore* core)
        : Enemy(TaggedObject::Tag::ignored, Health{9999}), core_(core)
    {
        hitbox_.dimension_.size_ = {26, 18};
        hitbox_.dimension_.origin_ = {-3, 0};
    }


    void step() override
    {
        if (core_) {
            flip_ = core_->flip_;
            position_ = core_->position();
            position_.y -= 23;
            if (not flip_) {
                position_.x -= 14;
            } else {
                position_.x -= 18;
            }
        } else {
            if (detaching_) {
                speed_.y *= Fixnum(0.92f);
                if (speed_.y > -0.1f) {
                    speed_ = {};
                    detaching_ = false;
                }
            } else {
                switch (timeline_++) {
                case 80:
                    if (x() > engine().hero()->x()) {
                        flip_ = false;
                    } else {
                        flip_ = true;
                    }
                    {
                        auto dir =
                            rotate({1, 0}, rng::choice<8>(rng::critical_state) * 45);
                        speed_.x = Fixnum(dir.x);
                        speed_.y = Fixnum(dir.y);
                    }
                    if (y() > 100) {
                        auto dir = rotate({1, 0},
                                          (1 + rng::choice<3>(rng::critical_state)) * 45);
                        speed_.x = Fixnum(dir.x);
                        speed_.y = Fixnum(dir.y);
                    }
                    if (shotcyc_ % 2 == 0) {
                        speed_ = {};
                    }
                    shotcyc_ += 1;
                    if (shotcyc_ == 4) {
                        int xo = (not flip_) ? -2 : 24;
                        if (auto e = engine().add_object<Megashot>(
                                Vec2<Fixnum>{x() + xo, y() + 7})) {
                            auto dir = direction(fvec(position_),
                                                 fvec(engine().hero()->position()));
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                        if (auto e = engine().add_object<EnemyShot>(
                                Vec2<Fixnum>{x() + xo, y() + 7})) {
                            auto dir = direction(fvec(position_),
                                                 fvec(engine().hero()->position()));
                            dir = rotate(dir, 340);
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                        if (auto e = engine().add_object<EnemyShot>(
                                Vec2<Fixnum>{x() + xo, y() + 7})) {
                            auto dir = direction(fvec(position_),
                                                 fvec(engine().hero()->position()));
                            dir = rotate(dir, 20);
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                    } else if (shotcyc_ == 8) {
                        int xo = (not flip_) ? -2 : 24;
                        if (auto e = engine().add_object<Megashot>(
                                Vec2<Fixnum>{x() + xo, y() + 7})) {
                            auto dir = direction(fvec(position_),
                                                 fvec(engine().hero()->position()));
                            dir = dir * (2.5f / 2);
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                    } else {
                        int xo = (not flip_) ? -2 : 24;
                        if (auto e = engine().add_object<Supershot>(
                                Vec2<Fixnum>{x() + xo, y() + 7})) {
                            if (flip_) {
                                e->set_speed({1, 0});
                            } else {
                                e->set_speed({-1, 0});
                            }
                        }
                    }
                    if (shotcyc_ == 12) {
                        shotcyc_ = 0;
                    }
                    break;

                case 139:
                    timeline_ = 80;
                    break;
                }
            }
            if (not place_free({position_.x + speed_.x, position_.y + speed_.y}, 3)) {
                auto x = position_.x;
                auto y = position_.y;
                if (not place_free({x + 2, y}, 3) or not place_free({x - 2, y}, 3)) {
                    speed_.x = speed_.x * -1;
                }
                if (not place_free({x, y + 2}, 3) or not place_free({x, y - 2}, 3)) {
                    speed_.y = speed_.y * -1;
                }
            }
            position_ = position_ + speed_;
        }
    }


    void detach()
    {
        core_ = nullptr;
        detaching_ = true;
        speed_.y = -4;
        hitbox_.dimension_.size_ = {26, 24};
    }


    void vortex()
    {
        int xo = (not flip_) ? -1 : 24;
        if (auto e = engine().add_object<Vortex>(Vec2<Fixnum>{x() + xo, y() + 9})) {
            if (not flip_) {
                e->set_left();
            }
        }
    }


    void gigashot()
    {
        for (int i = 0; i < 3; ++i) {
            int xo = (not flip_) ? -2 : 24;
            if (auto e = engine().add_object<Gigashot>(Vec2<Fixnum>{x() + xo, y() + 7})) {
                auto dir = direction(fvec(position_), fvec(engine().hero()->position()));
                auto d = dir * 0.25f;
                auto g = dir * (0.04f + i * 0.01f);
                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                e->set_grav({Fixnum(g.x), Fixnum(g.y)});
            }
        }
    }


    void megashot()
    {
        int xo = (not flip_) ? -2 : 24;
        if (auto e = engine().add_object<Megashot>(Vec2<Fixnum>{x() + xo, y() + 7})) {
            if (flip_) {
                e->set_speed({1, 0});
            } else {
                e->set_speed({-1, 0});
            }
        }
    }


    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(flip_ ? 179 : 178);
        spr_.set_position(position_);
        spr_.set_flip({flip_, vflip_});
        screen.draw(spr_);

        auto pos = position_;
        pos.x += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(flip_ ? 178 : 179);
        screen.draw(spr_);

        pos = position_;
        pos.x += 16;
        pos.y += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(flip_ ? 180 : 181);
        screen.draw(spr_);

        pos = position_;
        pos.y += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(flip_ ? 181 : 180);
        screen.draw(spr_);
    }


    bool damage(Health dmg, Object& s) override
    {
        s.kill();
        return false;
    }


private:
    GuardianCore* core_;
    bool flip_ = false;
};


class GuardianLower : public Enemy
{
public:
    GuardianLower(GuardianCore* core)
        : Enemy(TaggedObject::Tag::ignored, Health{9999}), core_(core)
    {
        hitbox_.dimension_.size_ = {26, 18};
        hitbox_.dimension_.origin_ = {-3, -5};
    }


    void step() override
    {
        flip_ = core_->flip_;

        position_ = core_->position();
        // position_.y += 23;
        if (not flip_) {
            position_.x -= 14;
        } else {
            position_.x -= 18;
        }
        if (flaming_) {
            flamecyc_ += 1;
            if (flamecyc_ == 4) {
                flamecyc_ = 0;
            }
        }
    }


    void stop_flame()
    {
        flaming_ = false;
        flamecyc_ = 0;
    }


    void start_flame()
    {
        flaming_ = true;
        flamecyc_ = 0;
    }


    bool damage(Health dmg, Object& s) override
    {
        s.kill();
        return false;
    }


    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index((flip_ ? 185 : 184) + ((flamecyc_ > 2) ? -2 : 0));
        spr_.set_position(position_);
        spr_.set_flip({flip_, vflip_});
        screen.draw(spr_);

        auto pos = position_;
        pos.x += 16;
        spr_.set_position(pos);
        spr_.set_texture_index((flip_ ? 184 : 185) + ((flamecyc_ > 2) ? -2 : 0));
        screen.draw(spr_);

        pos = position_;
        pos.x += 16;
        pos.y += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(flip_ ? 186 : 187);
        screen.draw(spr_);

        pos = position_;
        pos.y += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(flip_ ? 187 : 186);
        screen.draw(spr_);
    }


private:
    GuardianCore* core_;
    bool flip_ = false;
    bool flaming_ = false;
    u8 flamecyc_ = 0;
};


class DetachedCore : public Enemy
{
public:
    DetachedCore(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y)
        : Enemy(TaggedObject::Tag::ignored,
                Health{64}),
          spawn_x_(spawn_x), spawn_y_(spawn_y)
    {
        hitbox_.dimension_.size_ = {6, 6};
        hitbox_.dimension_.origin_ = {3, 3};
        origin_ = {3, 3};

        sprite_index_ = 164;
        position_ = pos;
        speed_.y = -2;
    }


    void step() override
    {
        if (health_ == 0 and not dead_) {
            platform().speaker().stop_music();
            engine().add_object<Explo>(position_);
            dead_ = true;

            for (auto& e : engine().enemies_) {
                if (dynamic_cast<Hunter*>(e.get()) or dynamic_cast<Soldier*>(e.get()) or
                    dynamic_cast<Spew*>(e.get())) {
                    e->kill();
                }
            }

            platform().speaker().play_sound("snd_bossroar", 12);
            platform().speaker().play_sound("snd_explo3", 14);
            return;
        }

        if (dead_) {
            deadcyc_ += 1;
            if (deadcyc_ % 4 == 0) {
                platform().speaker().play_sound("snd_explo1", deadcyc_ / 4);
                auto p = position_;
                p.x -= 4;
                p.y -= 4;
                engine().add_object<Explo>(rng::sample<8>(p, rng::critical_state));
                engine().g_.screenshake_ = 2;
            }


            if (deadcyc_ == 130) {
                kill();
                for (int i = 0; i < 6; ++i) {
                    for (int j = 0; j < 8; ++j) {
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

                engine().boss_completed();

                for (auto& e : engine().enemies_) {
                    e->kill();
                }

                engine().g_.screenshake_ = 12;

                engine().add_object<Pickup>(position_, Pickup::blade);
                platform().speaker().play_sound("snd_explo4", 90);
            }
        }

        animcyc_ += 1;
        if (animcyc_ == 4) {
            animcyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 4) {
                sprite_subimage_ = 0;
            }
        }

        if (detaching_) {
            speed_.y *= Fixnum(0.92f);
            if (speed_.y > -0.1f) {
                speed_ = {};
                detaching_ = false;
            }
        } else {
            if (dead_) {
                speed_ = {};
                return;
            }
            switch (timeline_++) {
            case 80:
                if (dead_) {
                    break;
                }
                if (shotcyc_ < 12) {
                    auto dir = rotate({1, 0}, rng::choice<8>(rng::critical_state) * 45);
                    dir = dir * (1.4f / 2);
                    speed_.x = Fixnum(dir.x);
                    speed_.y = Fixnum(dir.y);
                }
                if (health_ > 0) {
                    shotcyc_ += 1;
                    if (shotcyc_ == 4) {
                        speed_ = {};
                        for (int i = 0; i < 4; ++i) {
                            auto o = position_;
                            o.x -= 3;
                            o.y -= 3;
                            if (auto e = engine().add_object<Supershot>(o)) {
                                auto d = rotate({1, 0}, i * 90);
                                d = d * (1.5f / 2);
                                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                            }
                        }
                    }
                    if (shotcyc_ == 8) {
                        for (int i = 1; i < 4; ++i) {
                            auto o = position_;
                            if (auto e = engine().add_object<Bomb>(o)) {
                                auto d = rotate({1, 0}, i * 45);
                                d = d * (2.8f / 2);
                                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                            }
                        }
                    }
                    if (shotcyc_ == 12) {
                        for (int i = 0; i < 8; ++i) {
                            auto o = position_;
                            o.x -= 2;
                            o.y -= 2;
                            if (auto e = engine().add_object<EnemyShot>(o)) {
                                auto d = rotate({1, 0}, i * 45 + 22.5);
                                d = d * (1.5f / 2);
                                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                            }
                        }
                        speed_ = {};
                    }
                    if (shotcyc_ == 16) {
                        shotcyc_ = 0;
                    }
                }
                break;

            case 119:
                timeline_ = 80;
                break;
            }
        }

        Enemy::step();

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
    }


private:
    u8 spawn_x_;
    u8 spawn_y_;
    u8 animcyc_ = 0;
    bool detaching_ = true;
    u8 shotcyc_ = 0;
    int timeline_ = 0;

    bool dead_ = false;
    u8 deadcyc_ = 0;
};


GuardianCore::GuardianCore(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y)
    : Enemy(TaggedObject::Tag::ignored,
            Health{128}),
      spawn_x_(spawn_x), spawn_y_(spawn_y)
{
    hitbox_.dimension_.size_ = {6, 6};
    hitbox_.dimension_.origin_ = {3, 3};
    origin_ = {3, 3};

    sprite_index_ = 164;

    position_ = pos;
    position_.y += 32;
    position_.x -= 2;


    if (not(lower_ = engine().add_object<GuardianLower>(this))) {
        platform().fatal("failed to alloc guardian segment");
    }
    if (not(upper_ = engine().add_object<GuardianUpper>(this))) {
        platform().fatal("failed to alloc guardian segment");
    }
}


void GuardianCore::step()
{
    if (sprite_index_ > 0) {
        animcyc_ += 1;
        if (animcyc_ == 4) {
            animcyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 4) {
                sprite_subimage_ = 0;
            }
        }
    }


    auto inside_floor = [this] {
        return not place_free({x(), y() + 21}) and (not place_free({x() - 6, y() + 21}) or
                                                    not place_free({x() + 10, y() + 21}));
    };

    // Hit wall
    if (speed_.x < 0 and not place_free({x() - 10, y()})) {
        speed_.x *= -1;
        dir_ = not dir_;
    } else if (speed_.x > 0 and not place_free({x() + 14, y()})) {
        speed_.x *= -1;
        dir_ = not dir_;
    } else if (speed_.y > 0 and inside_floor()) {
        // round off fraction.
        position_.y = position_.y.as_integer();

        while (inside_floor()) {
            position_.y -= 1;
        }
        position_.y += 3;

        ((GuardianLower*)lower_)->stop_flame();

        engine().g_.screenshake_ = 6;

        gravity_ = 0;
        upwards_speed_ = 0;
        downwards_speed_ = 0;
        speed_ = {};
    }

    if (detachcyc_ == 0 or detachcyc_ >= 60) {
        switch (timeline_++) {
        case 0:
            engine().swapsong("boss");
            ((GuardianLower*)lower_)->start_flame();
            break;

        case 40:
            gravity_ = Fixnum(0.1f);
            downwards_speed_ = 0;
            upwards_speed_ = -4;
            break;

        case 140:
            ((GuardianLower*)lower_)->start_flame();
            break;

        case 160:
            shotcyc_ += 1;
            if (shotcyc_ == 4) {
                shotcyc_ = 1;
            }
            downwards_speed_ = 0;
            if (not upper_) {
                upwards_speed_ = -3.f;
            } else {
                if (TaggedObject::get_taglist(TaggedObject::Tag::hunter)) {
                    upwards_speed_ = -2;
                } else {
                    upwards_speed_ = -2.5f;
                }
            }

            speed_ = {};
            gravity_ = Fixnum(0.1f);
            if (not dir_) {
                speed_.x = Fixnum(-0.5f);
            } else {
                speed_.x = Fixnum(0.5f);
            }
            if (x() > engine().hero()->x()) {
                flip_ = false;
            } else {
                flip_ = true;
            }
            break;

        case 180:
            if (shotcyc_ == 1) {
                if (upper_) {
                    ((GuardianUpper*)upper_)->megashot();
                }
            } else if (shotcyc_ == 2) {
                if (upper_) {
                    ((GuardianUpper*)upper_)->gigashot();
                }
            } else {
                // ((GuardianUpper*)upper_)->gigashot();
            }
            break;

        case 200:
            if (shotcyc_ == 3) {
                if (upper_) {
                    ((GuardianUpper*)upper_)->vortex();
                }
            }
            break;

        case 259:
            timeline_ = 140;
            break;
        }
    }

    if (health_ < 96) {
        for (auto& e : engine().generic_solids_) {
            if (dynamic_cast<SoldierDormant*>(e.get())) {
                e->kill();
                engine().add_object<Soldier>(e->position());
            }
        }
    }

    if (health_ < 64) {
        for (auto& e : engine().generic_solids_) {
            if (dynamic_cast<HunterDormant*>(e.get())) {
                e->kill();
                engine().add_object<Hunter>(e->position());
            }
        }
    }

    if (health_ == 0) {
        health_ = -1;
    }
    if (health_ == -1 and detachcyc_ < 60) {
        detachcyc_ += 1;
        if (detachcyc_ % 4 == 0) {
            Fixnum x = rng::sample<16>(position_.x.as_integer(), rng::critical_state);
            Fixnum y = rng::sample<16>(position_.y.as_integer(), rng::critical_state);
            engine().add_object<BigExplo>(Vec2<Fixnum>{x, y});
            platform().speaker().play_sound("snd_explo1", detachcyc_ / 4);
            engine().g_.screenshake_ = 2;
        }
        if (detachcyc_ >= 8 and speed_.y not_eq 0) {
            detachcyc_ = 0;
        }
        if (detachcyc_ >= 60) {
            sprite_index_ = 0;
            sprite_subimage_ = 0;
            ((GuardianUpper*)upper_)->detach();
            upper_ = nullptr;
            engine().add_object<DetachedCore>(position_, spawn_x_, spawn_y_);
        }
    }

    speed_.y = upwards_speed_ + downwards_speed_;

    downwards_speed_ += gravity_;


    Enemy::step();
}


bool GuardianCore::damage(Health dmg, Object& s)
{
    if (health_ == -1) {
        return false;
    } else {
        return Enemy::damage(dmg, s);
    }
}


} // namespace herocore
