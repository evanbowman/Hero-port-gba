#pragma once

#include "objects/enemies/enemy.hpp"
#include "objects/enemies/light/drone.hpp"
#include "objects/enemies/light/reaver.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/megashot.hpp"



namespace herocore
{



class ReaperDroneNeck : public Enemy
{
private:
    Fixnum dmax_;
    Fixnum d_ = 0;

    Fixnum rot_speed_;
    Fixnum rot_;

    Enemy* owner_;
    u8 rot_cycle_ = 0;

public:

    ReaperDroneNeck(const Vec2<Fixnum>& pos,
                    float rot,
                    float rot_speed,
                    float dmax,
                    Enemy* owner,
                    bool reaver = false) :
        Enemy(TaggedObject::Tag::drone, Health{2}),
        dmax_(dmax),
        rot_speed_(rot_speed),
        rot_(rot),
        owner_(owner)
    {
        position_ = pos;

        sprite_index_ = 2;
        sprite_subimage_ = 1;

        if (reaver) {
            sprite_index_ = 5;
            sprite_subimage_ = 0;
            health_ = 4;
        }

        hitbox_.dimension_.size_ = {8, 8};
    }


    void step() override
    {
        bool explo = true;
        if (health_ == -1) {
            health_ = 0;
            if (place_free(position_)) {
                if (sprite_index_ == 5) {
                    engine().add_object<Reaver>(position_);
                } else {
                    engine().add_object<Drone>(position_);
                }
                explo = false;
            }
        }
        if (health_ == 0) {
            kill();
            if (explo) {
                engine().add_object<Explo>(position_);
            }
            return;
        }

        if (d_ < dmax_) {
            d_ += Fixnum(0.5f);
        }

        if (d_ > dmax_) {
            d_ -= Fixnum(0.5f);
        }

        rot_ += rot_speed_;
        if (rot_ >= 360) {
            rot_ -= 360;
            rot_cycle_ += 1;
        }
        if (rot_ < 0) {
            rot_ += 360;
            if (rot_ >= 360) {
                rot_ -= 1;
            }
            rot_cycle_ += 1;
        }

        static constexpr auto rot_lut = make_rotation_lut(0);
        auto rvec = rot_lut[rot_.as_integer()];

        auto pos = owner_->position();
        position_ = pos + rvec * d_;
    }


private:

    int timeline_ = 0;

};




class ReaperDrone : public Enemy
{
private:
    int neck_ = 0;
    u8 spawn_x_;
    u8 spawn_y_;
    int explocyc_ = 0;
    int explo_ = 0;
    int superexplo_ = 0;
    bool dead_ = false;
    bool destroy_ = false;;

public:

    ReaperDrone(const Vec2<Fixnum>& pos,
                u8 spawn_x,
                u8 spawn_y) :
        Enemy(TaggedObject::Tag::drone, Health{64}),
        spawn_x_(spawn_x),
        spawn_y_(spawn_y)
    {
        position_ = pos;

        sprite_index_ = 2;

        hitbox_.dimension_.size_ = {8, 8};
    }


    void timeline_hard_mode()
    {

        switch (timeline_++) {
        case 0:
            platform().speaker().play_music("boss", 0);
            sprite_index_ = 2;
            break;

        case 10:
            sprite_index_ = 3;
            break;

        case 20:
            sprite_index_ = 2;
            break;

        case 30:
            sprite_index_ = 3;
            break;

        case 40:
            neck_ += 1;
            if (neck_ == 1) {
                for (int i = 0; i < 15; ++i) {
                    engine().add_object<ReaperDroneNeck>(position_,
                                                         i * 24.f,
                                                         2.f,
                                                         24.f,
                                                         this);
                }
                for (int i = 0; i < 15; ++i) {
                    engine().add_object<ReaperDroneNeck>(position_,
                                                         i * 24.f + 14,
                                                         -1.5f,
                                                         12.f,
                                                         this);
                }
            } else if (neck_ == 2) {
                for (int i = 0; i < 5; ++i){
                    for (int j = 0; j < 5; ++j){
                        engine().add_object<ReaperDroneNeck>(position_,
                                                             i * 72.f + j * 15,
                                                             -2.f,
                                                             j == 4 ? 128.f :
                                                             0.5f * (10.f + j * 10),
                                                             this);
                    }
                }
            } else if (neck_ == 3) {
                for (int i = 0; i < 3; i+=1){
                    for (int j = 0; j < 5; j += 1) {
                        engine().add_object<ReaperDroneNeck>(position_,
                                                             i * 120.f,
                                                             2.f,
                                                             0.5f * (10.f + j * 10),
                                                             this);
                    }
                    for (int j = 0; j < 5; j += 1) {
                        engine().add_object<ReaperDroneNeck>(position_,
                                                             i * 120.f,
                                                             -3.5f,
                                                             0.5f * (10.f + j * 10),
                                                             this);
                    }
                }
            } else if (neck_ == 4) {
                for (int i = 0; i < 6; i++) {
                    engine().add_object<ReaperDroneNeck>(position_,
                                                         i * 90.f,
                                                         0.5f * (2.f + i),
                                                         0.5f * (16.f + i * 4),
                                                         this,
                                                         true);

                }
                for (int i = 6; i < 8; i++) {
                    engine().add_object<ReaperDroneNeck>(position_,
                                                         i * 90.f,
                                                         0.5f * (2.f + i),
                                                         0.5f * (16.f + i * 4),
                                                         this);

                }
                neck_ = 0;
            }
            break;

        case 80:
        case 120:
        case 160: {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));

            speed_.x = Fixnum((2.f / 2) * dir.x);
            speed_.y = Fixnum((2.f / 2) * dir.y);
            break;

        }

        case 200: {
            int kill_count = 0;
            if (length(engine().enemies_) > 16) {
                kill_count = length(engine().enemies_) - 16;
            }
            for (auto& e : engine().enemies_) {
                if (e.get() not_eq this) {
                    if (kill_count > 0) {
                        e->kill();
                        engine().add_object<Explo>(e->position());
                        --kill_count;
                    }
                    e->destroy();
                }
            }
            for (int i = 0; i < 4; ++i) {
                if (auto m = engine().add_object<Megashot>(position_)) {
                    if (neck_ == 1 or neck_ == 3) {
                        auto dir = rotate({1, 0}, i * 90);
                        m->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                    } else {
                        auto angle = i * 90 + 45;
                        if (angle > 360) {
                            angle -= 360;
                        }
                        auto dir = rotate({1, 0}, angle);
                        m->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                    }
                }
            }
            speed_ = {};
            break;
        }

        case 300: {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));
            rotate(dir, 90);
            speed_.x = Fixnum((1.8f / 2) * dir.x);
            speed_.y = Fixnum((1.8f / 2) * dir.y);
            break;
        }

        case 340: {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));
            rotate(dir, 360 - 90);
            speed_.x = Fixnum((1.8f / 2) * dir.x);
            speed_.y = Fixnum((1.8f / 2) * dir.y);
            break;
        }

        case 380: {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));
            rotate(dir, rng::choice<360>(rng::utility_state));
            speed_.x = Fixnum((1.8f / 2) * dir.x);
            speed_.y = Fixnum((1.8f / 2) * dir.y);
            break;
        }

        case 442:
            for (auto& e : engine().enemies_) {
                if (e.get() not_eq this) {
                    e->destroy();
                }
            }
            break;

        case 449:
            timeline_ = 40;
            break;
        }
    }


    void timeline_easy_mode()
    {

        switch (timeline_++) {
        case 0:
            platform().speaker().play_music("boss", 0);
            sprite_index_ = 2;
            break;

        case 10:
            sprite_index_ = 3;
            break;

        case 20:
            sprite_index_ = 2;
            break;

        case 30:
            sprite_index_ = 3;
            break;

        case 40:
            neck_ += 1;
            if (neck_ == 1) {
                for (int i = 0; i < 15; ++i) {
                    engine().add_object<ReaperDroneNeck>(position_,
                                                         i * 24.f,
                                                         2.f,
                                                         16.f,
                                                         this);
                }
                for (int i = 0; i < 15; ++i) {
                    engine().add_object<ReaperDroneNeck>(position_,
                                                         i * 24.f + 14,
                                                         -1.5f,
                                                         10.f,
                                                         this);
                }
            } else if (neck_ == 2) {
                for (int i = 0; i < 5; ++i){
                    for (int j = 0; j < 4; ++j){
                        engine().add_object<ReaperDroneNeck>(position_,
                                                             i * 72.f + j * 15,
                                                             -2.f,
                                                             0.5f * (10.f + j * 10),
                                                             this);
                    }
                }
            } else if (neck_ == 3) {
                for (int i = 0; i < 3; i+=1){
                    for (int j = 0; j < 4; j += 1) {
                        engine().add_object<ReaperDroneNeck>(position_,
                                                             i * 120.f,
                                                             2.f,
                                                             0.5f * (10.f + j * 10),
                                                             this);
                    }
                    for (int j = 0; j < 4; j += 1) {
                        engine().add_object<ReaperDroneNeck>(position_,
                                                             i * 120.f,
                                                             -3.5f,
                                                             0.5f * (10.f + j * 10),
                                                             this);
                    }
                }
            } else if (neck_ == 4) {
                for (int i = 0; i < 6; i++) {
                    engine().add_object<ReaperDroneNeck>(position_,
                                                         i * 90.f,
                                                         0.5f * (2.f + i),
                                                         0.5f * (16.f + i * 4),
                                                         this,
                                                         true);

                }
                neck_ = 0;
            }
            break;

        case 80:
        case 120:
        case 160: {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));

            speed_.x = Fixnum((1.3f / 2) * dir.x);
            speed_.y = Fixnum((1.3f / 2) * dir.y);
            break;

        }

        case 200: {
            int kill_count = 0;
            if (length(engine().enemies_) > 16) {
                kill_count = length(engine().enemies_) - 16;
            }
            for (auto& e : engine().enemies_) {
                if (e.get() not_eq this) {
                    if (kill_count > 0) {
                        e->kill();
                        engine().add_object<Explo>(e->position());
                        --kill_count;
                    }
                    e->destroy();
                }
            }
            speed_ = {};
            break;
        }

        case 330: {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));
            rotate(dir, 90);
            speed_.x = Fixnum((1.8f / 2) * dir.x);
            speed_.y = Fixnum((1.8f / 2) * dir.y);
            break;
        }

        case 370: {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));
            rotate(dir, 360 - 90);
            speed_.x = Fixnum((1.8f / 2) * dir.x);
            speed_.y = Fixnum((1.8f / 2) * dir.y);
            break;
        }

        case 410: {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));
            rotate(dir, rng::choice<360>(rng::utility_state));
            speed_.x = Fixnum((1.8f / 2) * dir.x);
            speed_.y = Fixnum((1.8f / 2) * dir.y);
            break;
        }

        case 442:
            for (auto& e : engine().enemies_) {
                if (e.get() not_eq this) {
                    e->destroy();
                }
            }
            break;

        case 449:
            timeline_ = 40;
            break;
        }
    }


    void step() override
    {
        if (destroy_) {
            kill();
            engine().add_object<ExploSpewer>(Vec2<Fixnum>{
                    x() + 4, y() + 4
                });

            engine().p_->objects_removed_.push_back({
                    (u8)engine().room_.coord_.x,
                    (u8)engine().room_.coord_.y,
                    spawn_x_,
                    spawn_y_
                });

            return;
        }

        if (dead_) {
            explocyc_ += 1;
            if (explocyc_ >= explo_ and explo_ > 0) {
                explocyc_ = 0;
                explo_ -= 1;
                auto temprand = rng::choice<360>(rng::utility_state);
                for (int i = 0; i < 4; ++i) {
                    if (auto e = engine().add_object<Explo>(position_)) {
                        auto d = rotate({1, 0}, temprand);
                        d = d * (0.5f * (i * 2 + 1));
                        e->set_speed({Fixnum(d.x), Fixnum{d.y}});
                    }
                    if (auto e = engine().add_object<Explo>(position_)) {
                        auto r = temprand + 180;
                        if (r > 360) {
                            r -= 360;
                        }
                        auto d = rotate({1, 0}, r);
                        d = d * (0.5f * (i * 2 + 1));
                        e->set_speed({Fixnum(d.x), Fixnum{d.y}});
                    }
                }
                if (explo_ == 0) {
                    superexplo_ = 1;
                    explocyc_ = 0;
                }
            }
            if (superexplo_) {
                explocyc_ += 1;
                if (explocyc_ > 20) {
                    destroy_ = true;
                }
            }
            return;
        }

        if (health_ <= 0) {
            explo_ = 20;
            dead_ = true;

            platform().speaker().stop_music();

            for (auto& e : engine().enemies_) {
                if (e.get() not_eq this) {
                    e->kill();
                    engine().add_object<Explo>(e->position());
                }
            }

            return;
        }

        move(position_ + speed_);

        if (engine().g_.difficulty_ == Difficulty::hard) {
            timeline_hard_mode();
        } else {
            timeline_easy_mode();
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


private:

    int timeline_ = 0;

};



}
