#include "hydra.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/projectile/bomb.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/vortex.hpp"


namespace herocore
{


void HydraHead::draw(Platform::Screen& s) const
{
    if (deadcyc_ == 0) {
        Enemy::draw(s);
    }

    Sprite spr;
    spr.set_origin({origin_.x, origin_.y});

    auto xdiff_ = Fixnum(0.2f) * (x_start_ - x());
    auto ydiff_ = Fixnum(0.2f) * (y_start_ - y());

    auto neck = [&](Fixnum xfrac, Fixnum yfrac) {
        Fixnum x = (position_.x + xdiff_ * xfrac) + 12;
        Fixnum y = (position_.y + ydiff_ * yfrac) + 8;
        spr.set_position({x, y});
        s.draw(spr);
    };

    spr.set_texture_index(141);
    spr.set_priority(2);

    if (deadcyc_ < 15) {
        neck(1, Fixnum(1.5f));
    }

    if (deadcyc_ < 30) {
        neck(2, Fixnum(3.f));
    }

    if (deadcyc_ < 45) {
        neck(3, Fixnum(4.f));
    }

    if (deadcyc_ < 60) {
        neck(4, Fixnum(4.6f));
    }

    if (deadcyc_ < 75) {
        neck(5, Fixnum(5.f));
    }


    if (flash_) {
        flashcyc_ += 1;
        if (flashcyc_ == 2) {
            flashcyc_ = 0;
            Sprite spr_;
            spr_.set_texture_index(52);
            spr_.set_position({position_.x - 13, position_.y - 11});

            s.draw(spr_);

            spr_.set_texture_index(53);
            spr_.set_position({position_.x - 13, position_.y + 5});

            s.draw(spr_);
        }
    }
}


void HydraHead::step()
{
    const bool hard = engine().g_.difficulty_ == Difficulty::hard;

    if (deadcyc_) {
        flash_ = false;
        ++deadcyc_;

        auto xdiff_ = Fixnum(0.2f) * (x_start_ - x());
        auto ydiff_ = Fixnum(0.2f) * (y_start_ - y());

        auto boom = [&](Fixnum xfrac, Fixnum yfrac) {
            Fixnum x = (position_.x + xdiff_ * xfrac) + 12;
            Fixnum y = (position_.y + ydiff_ * yfrac) + 8;
            engine().add_object<BigExplo>(Vec2<Fixnum>{x - 6, y - 6});
            if (engine().g_.screenshake_ < 2) {
                engine().g_.screenshake_ = 2;
            }
        };


        switch (deadcyc_) {
        case 1:
            boom(1, 1);
            break;

        case 10:
            boom(1, Fixnum(1.5f));
            break;

        case 20:
            boom(2, Fixnum(3.f));
            break;

        case 30:
            boom(3, Fixnum(4.f));
            break;

        case 40:
            boom(4, Fixnum(4.6f));
            break;

        case 50:
            boom(5, Fixnum(5.f));
            break;
        }
    }

    if (health() == 0 and deadcyc_ == 0) {
        deadcyc_++;
        platform().speaker().play_sound("snd_bossroar", 9);
        platform().speaker().play_sound("snd_explo3", 8);
    }

    if (deadcyc_ == 60) {
        kill();
        if (engine().g_.screenshake_ < 6) {
            engine().g_.screenshake_ = 6;
        }
        platform().speaker().play_sound("snd_explo1", 6);
        if (copy_ == 0) {

            if (hard) {
                auto h = engine().add_object<HydraHead>(
                    Vec2<Fixnum>{x_start_, y_start_ - 2}, 40);
                if (h) {
                    h->copy_++;
                    h->health_ = 8;
                }

                h = engine().add_object<HydraHead>(Vec2<Fixnum>{x_start_, y_start_ + 2},
                                                   40);
                if (h) {
                    h->copy_++;
                    h->health_ = 8;
                    h->hyper_ = true;
                }
            } else {
                auto h =
                    engine().add_object<HydraHead>(Vec2<Fixnum>{x_start_, y_start_}, 40);
                if (h) {
                    h->copy_++;
                    h->health_ = 8;
                }
            }

        } else if (copy_ == 1 and hyper_) {
            auto h = engine().add_object<HydraHead>(Vec2<Fixnum>{x_start_, y_start_}, 40);
            if (h) {
                h->copy_ = 2;
                h->health_ = 8;
            }
        }
    }

    if (deadcyc_) {
        return;
    }

    if (copy_ == 1) {
        sprite_index_ = 172;
    } else if (copy_ == 2) {
        sprite_index_ = 175;
    }

    switch (timeline_++) {
    case 50:
        sprite_subimage_ = 0;
        break;

    case 60:
        sprite_subimage_ = 1;
        break;

    case 70:
        sprite_subimage_ = 0;
        break;

    case 80:
        sprite_subimage_ = 1;
        move_ = 1;
        break;

    case 160:
        sprite_subimage_ = 2;
        if (copy_ == 0) {
            if (auto obj =
                    engine().add_object<EnemyShot>(Vec2<Fixnum>{x() - 2, y() + 1})) {
                auto dir = direction(fvec(position_), fvec(engine().hero()->position()));

                obj->set_speed(Vec2<Fixnum>{dir.x, dir.y});
            }
            // if (hard) {
            if (auto obj =
                    engine().add_object<EnemyShot>(Vec2<Fixnum>{x() - 2, y() + 1})) {
                auto dir = rotate({1, 0}, 120 + rng::choice<120>(rng::critical_state));

                dir = dir * 0.5f;

                obj->set_speed(Vec2<Fixnum>{dir.x, dir.y});
            }
            // }
        } else if (copy_ == 1) {
            if (auto b = engine().add_object<Bomb>(position_)) {
                auto d = rotate({1, 0}, 180 - 60);
                b->set_speed({2.8f * Fixnum(d.x), Fixnum(1.5f * -d.y)});
            }
            if (hard) {
                if (auto obj =
                        engine().add_object<EnemyShot>(Vec2<Fixnum>{x() - 2, y() + 1})) {
                    auto dir = rotate({1, 0}, 180);

                    dir = dir * 0.5f;

                    obj->set_speed(Vec2<Fixnum>{dir.x, dir.y});
                }
            }
        } else if (copy_ == 2) {
            flash_ = true;
        }
        break;

    case 200:
        if (flash_) {
            if (auto v = engine().add_object<Vortex>(position_)) {
                v->set_left();
            }
            flash_ = false;
        }
        break;

    case 240:
        sprite_subimage_ = 1;
        break;

    case 279:
        timeline_ = 160;
        break;
    }

    if (not move_) {
        return;
    }

    xstep_ += 1;
    if (xstep_ >= xstepmax_) {
        xstep_ = 0;
        xdir_ = rng::choice<2>(rng::utility_state);
        xstepmax_ = 16 + rng::choice<24>(rng::utility_state);
    }
    if (xdir_ == 0) {
        speed_.x -= Fixnum(0.06f);
    }
    if (xdir_ == 1) {
        speed_.x += Fixnum(0.06f);
    }
    if (speed_.x < Fixnum(-1.4f / 2)) {
        speed_.x = Fixnum(-1.4f / 2);
    }
    if (speed_.x > Fixnum(1.4f / 2)) {
        speed_.x = Fixnum(1.4f / 2);
    }
    if (x() < x_start_ - 32) {
        xdir_ = 1;
    }
    if (x() > x_start_ - 8) {
        xdir_ = 0;
    }

    ystep_ += 1;
    if (ystep_ >= ystepmax_) {
        ystep_ = 0;
        ydir_ = rng::choice<2>(rng::utility_state);
        ystepmax_ = 16 + rng::choice<24>(rng::utility_state);
    }
    if (ydir_ == 0) {
        speed_.y -= Fixnum(0.06f);
    }
    if (ydir_ == 1) {
        speed_.y += Fixnum(0.06f);
    }
    if (speed_.y < Fixnum(-1.4f / 2)) {
        speed_.y = Fixnum(-1.4f / 2);
    }
    if (speed_.y > Fixnum(1.4f / 2)) {
        speed_.y = Fixnum(1.4f / 2);
    }
    if (y() < y_start_ - 40) {
        ydir_ = 1;
    }
    if (y() > y_start_ + 8) {
        ydir_ = 0;
    }

    Enemy::step();
}


Hydra::Hydra(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y)
    : Enemy(TaggedObject::Tag::ignored, Health{9999}), spawn_x_(spawn_x),
      spawn_y_(spawn_y)
{
    position_ = pos;

    sprite_index_ = 0;

    hitbox_.dimension_.size_ = {42, 48};

    engine().add_object<HydraHead>(Vec2<Fixnum>{x(), y() + 5}, 40);
    engine().add_object<HydraHead>(Vec2<Fixnum>{x(), y() + 5 + 8}, 20);
    engine().add_object<HydraHead>(Vec2<Fixnum>{x(), y() + 5 + 16}, 0);

    draw_image(platform(), 49, spawn_x / 8 + 5, spawn_y / 8, 6, 6, Layer::map_0);
}


void Hydra::step()
{
    if (health_ > 0) {
        miscyc_ += 1;
        if (miscyc_ == 280) {
            miscyc_ = 0;

            Vec2<Fixnum> o{x() + 33, y() + 23};
            if (auto m = engine().add_object<Missile>(o)) {
                m->set_dir(rotate({1, 0}, 360 - 75));
            }
            o.x = x() + (76 / 2);
            o.y = y() + (58 / 2);
            if (auto m = engine().add_object<Missile>(o)) {
                m->set_dir(rotate({1, 0}, 360 - 65));
            }
        }
    }

    if ((health_ <= 0 or length(engine().enemies_) == 1) and not dead_) {
        platform().speaker().stop_music();
        dead_ = true;
        return;
    }

    auto o = position_;
    o.x += 24;
    o.y += 24;

    if (dead_) {

        ecyc_ += 1;
        if (ecyc_ >= 6) {
            ecyc_ = 0;
            totcyc_ += 1;
            if (totcyc_ < 15) {
                engine().g_.screenshake_ = 6;
                platform().speaker().play_sound("snd_explo1", 9);
                engine().add_object<BigExplo>(rng::sample<16>(o, rng::critical_state));
            } else if (totcyc_ == 15) {

                platform().speaker().play_sound("snd_explo3", 10);

                engine().add_object<ExploSpewer>(Vec2<Fixnum>{o.x + 20, o.y + 25});

                if (auto e =
                        engine().add_object<ExploSpewer>(Vec2<Fixnum>{o.x - 16, o.y})) {
                    e->set_speed({-1, 0});
                }
                if (auto e =
                        engine().add_object<ExploSpewer>(Vec2<Fixnum>{o.x + 16, o.y})) {
                    e->set_speed({1, 0});
                }
                if (auto e =
                        engine().add_object<ExploSpewer>(Vec2<Fixnum>{o.x, o.y - 16})) {
                    e->set_speed({0, -1});
                }
                if (auto e =
                        engine().add_object<ExploSpewer>(Vec2<Fixnum>{o.x, o.y + 16})) {
                    e->set_speed({0, 1});
                }
            } else if (totcyc_ >= 20) {
                platform().speaker().play_sound("snd_explo4", 14);
                for (int i = 0; i < 8; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        if (auto exp = engine().add_object<BigExplo>(o)) {
                            auto dir = rotate({1, 0}, j * 90 + 45 + i * 3);
                            dir = dir * ((i + 1 / 2.f) * 1.5f);
                            Vec2<Fixnum> spd;
                            spd.x = Fixnum(dir.x);
                            spd.y = Fixnum(dir.y);
                            exp->set_speed(spd);
                        }
                    }
                }
                kill();
                engine().add_object<Explo>(o);

                engine().p_->objects_removed_.push_back({(u8)engine().room_.coord_.x,
                                                         (u8)engine().room_.coord_.y,
                                                         spawn_x_,
                                                         spawn_y_});

                u8 start_x = spawn_x_ / 8 + 5;
                u8 start_y = spawn_y_ / 8;
                for (u16 y = start_y; y < start_y + 6; ++y) {
                    for (u16 x = start_x; x < start_x + 6; ++x) {
                        platform().set_tile(Layer::map_0, x, y, 0);
                    }
                }

                engine().boss_completed();

                engine().add_object<Pickup>(o, Pickup::blaster);

                return;
            }
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
        break;

    case 75:
        break;

    case 99:
        timeline_ = 40;
        break;
    }
}


} // namespace herocore
