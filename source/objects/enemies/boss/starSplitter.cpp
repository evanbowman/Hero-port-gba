#include "starSplitter.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/gigashot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/supershot.hpp"



namespace herocore
{



class StarSplitterCore : public Enemy
{
private:
    StarSplitter* parent_ = nullptr;
    int animcyc_ = 0;
    s8 x_offset_;
    s8 y_offset_;

public:
    StarSplitterCore(StarSplitter* parent, s8 x_offset, s8 y_offset)
        : Enemy(TaggedObject::Tag::ignored, Health{1}),
          parent_(parent),
          x_offset_(x_offset),
          y_offset_(y_offset)
    {
        hitbox_.dimension_.size_ = {4, 4};
        hitbox_.dimension_.origin_ = {3, 3};
        origin_ = {3, 3};

        sprite_index_ = 164;
    }



    int collision_damage() const override
    {
        return 4;
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
            parent_->core_destroyed();
            platform().speaker().play_sound("snd_bossroar", 9);

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

        position_.x = parent_->position().x + x_offset_;
        position_.y = parent_->position().y + y_offset_;
    }
};



// Just created for the hitbox. Pinned to the boss' position
class StarSplitterDummy : public Enemy
{
public:

    StarSplitterDummy(StarSplitter* parent) :
        Enemy(TaggedObject::Tag::ignored, Health{9999}),
        parent_(parent)
    {
        hitbox_.dimension_.size_ = {28, 4};
        hitbox_.dimension_.origin_ = {16, 2};
    }


    bool damage(Health dmg, Object& s)
    {
        return parent_->damage(dmg, s);
    }


    void step()
    {
        position_ = parent_->position();
    }


private:
    StarSplitter* parent_;
};



class StarSplitterFoot : public Enemy
{
public:

    StarSplitterFoot(const Vec2<Fixnum>& position, StarSplitter* parent) :
        Enemy(TaggedObject::Tag::ignored, Health{1}),
        parent_(parent)
    {
        hitbox_.dimension_.size_ = {13, 13};
        hitbox_.dimension_.origin_ = {7, 7};
        origin_ = {8, 8};

        position_ = position;
        sprite_index_ = 225;
    }


    void step() override
    {
        auto prev_pos = position_;

        speed_ = speed_ + gravity_;
        Enemy::step();

        ++move_duration_;

        u8 tries_ = 0;
        bool collide = false;
        while (not place_free(position_)) {
            collide = true;
            position_ = (position_ + prev_pos) / Fixnum(2);
            ++tries_;

            if (tries_ > 20) {
                position_ = prev_pos;
                break;
            }
        }

        if (collide) {
            if (bounces_ <= 0) {
                speed_ = {};
                gravity_ = {};
                parent_->foot_ready_ -= 1;
                parent_->feet_moving_ -= 1;
                // TODO: create falling blocks
            } else {
                bounces_ -= 1;
                speed_ = speed_ * Fixnum(0.8f);

                bool gotit = false;
                if (not place_free({x() + 1, y()}) or not place_free({x() - 1, y()})) {
                    speed_.x *= -1;
                    gotit = true;
                }
                if (not place_free({x(), y() + 1}) or not place_free({x(), y() - 1})) {
                    speed_.y *= -1;
                    gotit = true;
                }
                if (not gotit) {
                    speed_.x *= -1;
                    speed_.y *= -1;
                }
            }
            if (move_duration_ > 6) {
                stuck_ = false;
                engine().g_.screenshake_ = 2;
            } else {
                stuck_ = true;
            }
        }
    }


    bool damage(Health, Object& s) override
    {
        s.kill();
        return false;
    }


    void set_dir(int rot)
    {
        auto dir = rotate({1, 0}, rot);
        dir_.x = Fixnum(dir.x);
        dir_.y = Fixnum(dir.y) * -1;
    }


    void set_grav(Fixnum grav, int grav_dir)
    {
        auto dir = rotate({1, 0}, grav_dir);
        gravity_.x = Fixnum(dir.x) * grav;
        gravity_.y = Fixnum(dir.y) * grav * -1;
        move_duration_ = 0;
    }


    void set_speed(Fixnum speed)
    {
        speed_ = speed * dir_;
    }


    void set_bounces(u8 count)
    {
        bounces_ = count;
    }


    bool stuck() const
    {
        return stuck_;
    }


private:
    StarSplitter* parent_;
    Vec2<Fixnum> dir_;
    Vec2<Fixnum> gravity_;

    bool flash_ = false;
    mutable u8 flashcyc_ = 0;
    s8 bounces_ = 0;
    u16 move_duration_ = 0;
    bool stuck_ = false;
};



class StarSplitterMidpoint : public Enemy
{
public:
    StarSplitterMidpoint(StarSplitter* lhs, StarSplitterFoot* rhs) :
        Enemy(TaggedObject::Tag::ignored, Health{64}),
        lhs_(lhs),
        rhs_(rhs)
    {
        sprite_index_ = 125;

        hitbox_.dimension_.size_ = {8, 8};
        hitbox_.dimension_.origin_ = {4, 4};
        origin_ = {4, 4};
    }


    bool damage(Health, Object& s) override
    {
        s.kill();
        return false;
    }


    void step() override
    {
        position_ = (lhs_->position() + rhs_->position()) / Fixnum(2.f);
    }


    int collision_damage() const override
    {
        return 3;
    }


private:
    StarSplitter* lhs_;
    StarSplitterFoot* rhs_;
};




StarSplitter::StarSplitter(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y)
    : Enemy(TaggedObject::Tag::ignored, Health{64}), spawn_x_(spawn_x),
      spawn_y_(spawn_y)
{
    position_ = pos;
    hitbox_.dimension_.size_ = {4, 28};
    hitbox_.dimension_.origin_ = {2, 16};

    engine().add_object<StarSplitterDummy>(this);
    engine().add_object<StarSplitterCore>(this, -5, -5);
    engine().add_object<StarSplitterCore>(this, -5, 5);
    engine().add_object<StarSplitterCore>(this, 5, 5);
    engine().add_object<StarSplitterCore>(this, 5, -5);
}



bool StarSplitter::damage(Health dmg, Object& s)
{
    s.kill();

    if (core_count_ == 0) {
        return Enemy::damage(dmg, s);
    }

    return false;
}



void StarSplitter::step()
{
    if (health_ == 0 and not dead_) {
        platform().speaker().play_sound("snd_bossroar", 9);
        platform().speaker().stop_music();

        engine().g_.screenshake_ = 6;
        dummy_speed_ = 0;
        dead_ = true;

        engine().enemy_projectiles_.clear();
        return;
    }

    if (dead_) {
        deadcyc_ += 1;
        if (deadcyc_ % 4 == 0 and deadcyc_ <= 80) {
            platform().speaker().play_sound("snd_explo1", deadcyc_ / 8);
            engine().add_object<ExploSpewer>(rng::sample<32>(position_,
                                                             rng::critical_state));
            engine().g_.screenshake_ = 2;
        }

        if (deadcyc_ == 105) {
            kill();

            platform().speaker().play_sound("snd_explo4", 100);

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

            engine().g_.screenshake_ = 24;

            engine().add_object<Pickup>(position_, Pickup::suit);

            engine().boss_completed();
            engine().p_->objects_removed_.push_back({(u8)engine().room_.coord_.x,
                                                     (u8)engine().room_.coord_.y,
                                                     spawn_x_,
                                                     spawn_y_});
        }

        return;
    }

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
        if (auto f = engine().add_object<StarSplitterFoot>(position_, this)) {
            f->set_grav(Fixnum(0.1f), 0);
            foot_1_ = f;

            engine().add_object<StarSplitterMidpoint>(this, f);
        }
        if (auto f = engine().add_object<StarSplitterFoot>(position_, this)) {
            f->set_grav(Fixnum(0.1f), 180);
            foot_2_ = f;

            engine().add_object<StarSplitterMidpoint>(this, f);
        }
        break;

    case 80:
        shotcyc_ += 1;
        if (shotcyc_ < 4) {
            for (int i = 0; i < 2; ++i) {
                auto origin = position_;
                origin.x -= 3;
                origin.y -= 3;
                if (auto s = engine().add_object<Supershot>(origin)) {
                    int rot = 180 * i + shotcyc_ * 45;
                    while (rot > 360) {
                        rot -= 360;
                    }
                    s->set_speed(1, rot);
                }
            }
        } else {
            shotcyc_ = 0;
            shotcyc2_ += 1;
            if (shotcyc2_ == 1) {
                for (int i = 0; i < 4; ++i) {
                    auto o = position_;
                    o.x -= 6;
                    o.y -= 6;
                    if (auto s = engine().add_object<Gigashot>(o)) {
                        auto d = rotate({1, 0}, i * 90);
                        auto g = d;
                        g = g * 0.025f;
                        s->set_grav({Fixnum(g.x), Fixnum(g.y)});
                        d = d * (0.8f / 2);
                        s->set_speed({Fixnum(d.x), Fixnum(d.y)});
                    }
                }
            } else if (shotcyc2_ == 2) {
                auto o = position_;
                o.x -= 4;
                o.y -= 4;
                if (auto s = engine().add_object<Megashot>(o)) {
                    auto dir = direction(fvec(o), fvec(engine().hero()->position()));
                    Vec2<Fixnum> fv;
                    fv.x = Fixnum(dir.x);
                    fv.y = Fixnum(dir.y);
                    s->set_speed(fv);
                }
            } else if (shotcyc2_ == 3) {
                shotcyc2_ = 0;
                auto origin = position_;
                origin.x -= 3;
                origin.y -= 3;
                for (int i = 0; i < 4; ++i) {
                    if (auto s = engine().add_object<Supershot>(origin)) {
                        s->set_speed(0.25f, i * 90);
                    }
                }
            }
        }
        break;

    case 139:
        timeline_ = 80;
        break;

    case 1080:
        shotcycx_ += 1;
        if (shotcycx_ < 8) {
            for (int i = 0; i < 2; i += 1) {
                auto o = position_;
                o.x -= 3;
                o.y -= 3;
                if (auto s = engine().add_object<Supershot>(o)) {
                    int dir = i * 180 + shotcycx_ * 45;
                    while (dir > 360) {
                        dir -= 360;
                    }
                    s->set_speed(1, dir);
                }
            }
        } else {
            shotcycx_ = 0;
            for (int i = 0;  i < 2; ++i) {
                auto o = position_;
                o.x -= 4;
                o.y -= 4;
                if (auto s = engine().add_object<Megashot>(o)) {
                    s->set_speed(1, i * 180);
                }
            }
        }
        break;

    case 1099:
        timeline_ = 1080;
        break;
    }

    if (foot_1_ and foot_2_) {

        if (core_count_ == 0) {
            foot_1_->kill();
            foot_2_->kill();
            foot_1_ = nullptr;
            foot_2_ = nullptr;

            for (auto& e : engine().enemies_) {
                if (e.get() not_eq this) {
                    e->kill();
                    engine().add_object<ExploSpewer>(e->position());
                    engine().g_.screenshake_ = 6;
                }
            }
            timeline_ = 1080;
            return;
        }

        position_ = (foot_1_->position() + foot_2_->position()) / Fixnum(2);

        if (foot_ready_ <= 0 and feet_moving_ == 0) {
            footcyc_ += 1;

            if (footcyc_ >= 40) {
                footcyc_ = 0;

                if (footmode_ == 1) {
                    footmode_ = 2;
                } else {
                    footmode_ = 1;
                }

                footmode2_ += 1;

                if (footmode2_ == 7) {
                    footmode2_ = 1;
                }

                auto foot = footmode_ == 1 ?
                    (StarSplitterFoot*)foot_1_ :
                    (StarSplitterFoot*)foot_2_;

                if (footmode2_ == 1 or footmode2_ == 2 or footmode2_ == 3) {
                    foot->set_bounces(0);
                } else if (footmode2_ == 4 or footmode2_ == 5) {
                    foot->set_bounces(1);
                } else {
                    foot->set_bounces(2);
                }

                auto x = foot->x() - 40;
                auto y = foot->y();

                feet_moving_ += 1;

                int dir = 0;
                if (x < 80 and y < 80) {
                    dir = rng::choice<70>(rng::critical_state) + 280;
                } else if (x >= 80 and y < 80) {
                    dir = rng::choice<70>(rng::critical_state) + 190;
                } else if (x < 80 and y >= 80) {
                    dir = rng::choice<70>(rng::critical_state) + 10;
                } else if (x >= 80 and y >= 80) {
                    dir = rng::choice<70>(rng::critical_state) + 100;
                } else {
                    dir = rng::choice<360>(rng::critical_state);
                }
                if (dir > 360) {
                    dir -= 360;
                }

                if (foot->stuck()) {
                    dir = rng::choice<360>(rng::critical_state);
                }

                foot->set_dir(dir);
                foot->set_grav(Fixnum(0.05f), dir);
            }
        }

    } else {
        auto prev_pos = position_;

        dummy_speed_ += 15;
        if (dummy_speed_ > 300) {
            // To avoid needing to calculate the magnitude of speed_
            dummy_speed_ = 300;
        } else {
            speed_ = speed_ + gravity_;
        }
        Enemy::step();


        u8 tries_ = 0;
        bool collide = false;
        auto cached_hb = hitbox_;
        hitbox_.dimension_.size_ = {28, 28};
        hitbox_.dimension_.origin_ = {14, 14};
        while (not place_free(position_, 8)) {
            collide = true;
            position_ = (position_ + prev_pos) / Fixnum(2);
            ++tries_;

            if (tries_ > 20) {
                position_ = prev_pos;
                break;
            }
        }
        hitbox_ = cached_hb;
        if (collide) {
            engine().g_.screenshake_ = 6;
            moving_ = false;
            speed_ = {};
            gravity_ = {};
            wait_ = 0;
            bwaitcyc_ += 1;
            if (bwaitcyc_ >= 4) {
                bwaitcyc_ = 0;
                bwait_ = 120;
            } else {
                bwait_ = 10;
            }
        }

        if (not moving_) {
            if (wait_ < bwait_) {
                wait_ += 1;
            }

            if (wait_ == bwait_) {
                moving_ = true;
                dummy_speed_ = 0;
                int dir = 0;
                auto x = this->x() - 40;
                auto y = this->y();
                if (x < 80 and y < 80) {
                    dir = rng::choice<70>(rng::critical_state) + 280;
                } else if (x >= 80 and y < 80) {
                    dir = rng::choice<70>(rng::critical_state) + 190;
                } else if (x < 80 and y >= 80) {
                    dir = rng::choice<70>(rng::critical_state) + 10;
                } else if (x >= 80 and y >= 80) {
                    dir = rng::choice<70>(rng::critical_state) + 100;
                } else {
                    dir = rng::choice<360>(rng::critical_state);
                }
                if (dir > 360) {
                    dir -= 360;
                }

                auto d = rotate({1, 0}, dir);
                gravity_.x = Fixnum(d.x);
                gravity_.y = -1 * Fixnum(d.y);
                gravity_ = gravity_ * Fixnum(0.15f / 2);
            }
        }
    }
}



}
