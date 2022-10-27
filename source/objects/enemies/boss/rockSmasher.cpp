#include "rockSmasher.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/projectile/shot.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/misc/pickup.hpp"



namespace herocore
{



class EnemyShotTimeout : public EnemyProjectile
{
private:

    int anim_ = 0;
    int count_ = 0;

public:

    EnemyShotTimeout(const Vec2<Fixnum>& pos) : EnemyProjectile(pos, 1, 0)
    {
        sprite_index_ = 17;

        hitbox_.dimension_.size_ = {5, 5};
    }


    void step() override
    {
        Object::step();

        anim_ += 1;
        if (anim_ == 2) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ > 3) {
                sprite_subimage_ = 0;
            }
        }

        count_ += 1;
        if (count_ > 80) {
            kill();
        }
    }

};



class RockSmasherCore : public Enemy
{
private:
    RockSmasher* owner_ = nullptr;
    int animcyc_ = 0;

public:

    Fixnum x_offset_;
    Fixnum y_offset_;

    s8 rfire_ = 0;
    u8 rfirecyc_ = 0;
    u8 dir_ = 0;

    u8 mode_ = 0;
    u8 id_ = 0;

    RockSmasherCore(RockSmasher* owner,
                    Fixnum x_offset,
                    Fixnum y_offset,
                    u8 mode,
                    u8 id) :
        Enemy(TaggedObject::Tag::ignored, Health{32}),
        owner_(owner),
        x_offset_(x_offset),
        y_offset_(y_offset),
        mode_(mode),
        id_(id)
    {
        hitbox_.dimension_.size_ = {4, 5};
        hitbox_.dimension_.origin_ = {3, 3};
        origin_ = {3, 3};

        sprite_index_ = 164;

        position_.x = owner_->position().x + x_offset_;
        position_.y = owner_->position().y + y_offset_;
    }


    void draw(Platform::Screen&) const override
    {
    }


    void step() override
    {
        if (health_ == 0) {
            engine().add_object<ExploSpewer>(Vec2<Fixnum>{
                    x(), y()
                });
            platform().sleep(4);
            kill();
            owner_->hp_--;
            owner_->c_->cores_[id_].hp_ = 0;
            platform().speaker().play_sound("snd_explo2", 10);
            engine().g_.screenshake_ = 6;
        }

        if (rfire_ > 0) {
            rfirecyc_ += 1;
            if (rfirecyc_ == 5) {
                rfirecyc_ = 0;
                rfire_ = -1;

                if (auto e = engine().add_object<EnemyShotTimeout>(position_)) {
                    if (dir_ == 180) {
                        e->set_speed({Fixnum(-1.5f), Fixnum(0)});
                    } else {
                        e->set_speed({Fixnum(1.5f), Fixnum(0)});
                    }

                }
            }
        }
    }
};



RockSmasher::RockSmasher(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y) :
    spawn_x_(spawn_x),
    spawn_y_(spawn_y),
    c_(allocate_dynamic<Components>(platform()))
{
    position_ = pos;

    platform().load_tile1_texture("rock_smasher");

    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            platform().set_tile(Layer::map_1, x, y, 54);
        }
    }

    draw_image(platform(), 2, 0, 0, 10, 10, Layer::map_1);

    for (int x = 2; x < 8; ++x) {
        for (int y = 2; y < 8; ++y) {
            platform().set_tile(Layer::map_1, x, y, 0);
        }
    }


    auto setup_core = [&](int n, u8 tx, u8 ty)
                      {
                          c_->cores_[n].tx_ = tx;
                          c_->cores_[n].ty_ = ty;
                          c_->cores_[n].hp_ = 32;
                      };

    setup_core(0, 1, 1);
    setup_core(1, 1, 3);
    setup_core(2, 1, 6);
    setup_core(3, 1, 7);

    setup_core(4, 8, 1);
    setup_core(5, 8, 3);
    setup_core(6, 8, 6);
    setup_core(7, 8, 7);

    // While cores are drawn with tiles for speed, we still create invisible
    // core sentinel objects for collision checking.
    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(-54 / 2),
                                         Fixnum(-50 / 2),
                                         u8(18),
                                         0);

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(-54 / 2),
                                         Fixnum(-22 / 2),
                                         u8(17),
                                         1);


    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(-54 / 2),
                                         Fixnum(50 / 2),
                                         u8(15),
                                         3);

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(-54 / 2),
                                         Fixnum(22 / 2),
                                         u8(16),
                                         2);


    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(54 / 2),
                                         Fixnum(-50 / 2),
                                         11,
                                         4);

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(54 / 2),
                                         Fixnum(-22 / 2),
                                         12,
                                         5);


    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(54 / 2),
                                         Fixnum(50 / 2),
                                         14,
                                         7);

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(54 / 2),
                                         Fixnum(22 / 2),
                                         13,
                                         6);

}



void RockSmasher::draw(Platform::Screen& s) const
{
    for (auto& orb : (c_->orbs_)) {
        orb.draw(s);
    }

    Solid::draw(s);

    c_->core_animcyc_ += 1;
    if (c_->core_animcyc_ == 4) {
        c_->core_animcyc_ = 0;
        c_->core_anim_ += 1;
        if (c_->core_anim_ == 4) {
            c_->core_anim_ = 0;
        }

        // Originally I was using actual objects, but switched to tiles due to
        // lag.
        auto draw_cores =
            [&](u8 tl, u8 tr)
            {
                if (c_->cores_[0].hp_) {
                    platform().set_tile(Layer::map_1, 1, 1, (tl - 1) + 1);
                    platform().set_tile(Layer::map_1, 1, 2, (tl - 1) + 2);
                } else {
                    platform().set_tile(Layer::map_1, 1, 1, 13);
                    platform().set_tile(Layer::map_1, 1, 2, 23);
                }
                if (c_->cores_[1].hp_) {
                    platform().set_tile(Layer::map_1, 1, 3, (tl - 1) + 3);
                } else {
                    platform().set_tile(Layer::map_1, 1, 3, 33);
                }
                if (c_->cores_[2].hp_) {
                    platform().set_tile(Layer::map_1, 1, 6, (tl - 1) + 4);
                } else {
                    platform().set_tile(Layer::map_1, 1, 6, 63);
                }
                if (c_->cores_[3].hp_) {
                    platform().set_tile(Layer::map_1, 1, 7, (tl - 1) + 5);
                    platform().set_tile(Layer::map_1, 1, 8, (tl - 1) + 6);
                } else {
                    platform().set_tile(Layer::map_1, 1, 7, 73);
                    platform().set_tile(Layer::map_1, 1, 8, 83);
                }

                if (c_->cores_[4].hp_) {
                    platform().set_tile(Layer::map_1, 8, 1, (tr - 1) + 1);
                    platform().set_tile(Layer::map_1, 8, 2, (tr - 1) + 2);
                } else {
                    platform().set_tile(Layer::map_1, 8, 1, 30);
                    platform().set_tile(Layer::map_1, 8, 2, 30);
                }
                if (c_->cores_[5].hp_) {
                    platform().set_tile(Layer::map_1, 8, 3, (tr - 1) + 3);
                } else {
                    platform().set_tile(Layer::map_1, 8, 3, 40);
                }
                if (c_->cores_[6].hp_) {
                    platform().set_tile(Layer::map_1, 8, 6, (tr - 1) + 4);
                } else {
                    platform().set_tile(Layer::map_1, 8, 6, 70);
                }
                if (c_->cores_[7].hp_) {
                    platform().set_tile(Layer::map_1, 8, 7, (tr - 1) + 5);
                    platform().set_tile(Layer::map_1, 8, 8, (tr - 1) + 6);
                } else {
                    platform().set_tile(Layer::map_1, 8, 7, 80);
                    platform().set_tile(Layer::map_1, 8, 8, 90);
                }
            };

        switch (c_->core_anim_) {
        case 0:
            draw_cores(24, 64);
            break;

        case 1:
            draw_cores(34, 74);
            break;

        case 2:
            draw_cores(44, 102);
            break;

        case 3:
            draw_cores(34, 74);
            break;
        }
    }

}



bool RockSmasher::overlapping_impl(Object& other)
{
    auto& hb = other.hitbox();

    // We can't realistically do pixel-perfect collisions on a gba. Or maybe we
    // could, but we'd have to optimize the crap out of it, and I don't feel
    // like it's worth the trouble. Instead, represent the rocksmasher hitbox as
    // a set of discrete bounding boxes.

    Vec2<Fixnum> pos;
    pos.x = position_.x - 40;
    pos.y = position_.y - 40;
    Hitbox temp_hb{&pos, {{80, 7}, {0, -2}}};

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    temp_hb.dimension_.size_ = {7, 80};
    temp_hb.dimension_.origin_ = {-2, 0};

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    pos.x = position_.x + 40;
    temp_hb.dimension_.origin_ = {9, 0};

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    pos.y = position_.y + 29;
    pos.x = position_.x - 40;

    temp_hb.dimension_.size_ = {32, 7};
    temp_hb.dimension_.origin_ = {0, -2};

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    pos.x = position_.x + 8;
    if (temp_hb.overlapping(hb)) {
        return true;
    }

    temp_hb.dimension_.size_ = {12, 5};
    temp_hb.dimension_.origin_ = {0, 0};

    pos.x = position_.x - 6;
    pos.y = position_.y - 32;

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    temp_hb.dimension_.size_ = {7, 10};
    temp_hb.dimension_.origin_ = {0, 0};
    pos.x = position_.x - 33;
    pos.y = position_.y - 5;

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    pos.x = position_.x + 24;
    pos.y = position_.y - 5;

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    temp_hb.dimension_.size_ = {3, 4};
    temp_hb.dimension_.origin_ = {0, 0};
    pos.x = position_.x + 3;
    pos.y = position_.y + 27;

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    pos.x = position_.x - 6;

    if (temp_hb.overlapping(hb)) {
        return true;
    }

    return false;
}



bool RockSmasher::overlapping(Object& other)
{
    // Checking intersection with the rocksmasher is pretty costly. Only test
    // collisions with the player and the player's shots.
    if (&other not_eq engine().hero()) {
        return false;
    }

    return overlapping_impl(other);
}



void RockSmasher::step()
{
    if (hp_ == 0 and not dead_) {
        dead_ = true;
        platform().speaker().play_sound("snd_bossroar", 9);
        platform().speaker().stop_music();
        return;
    }
    if (dead_) {

        deadcyc_ += 1;

        engine().enemy_projectiles_.clear();

        if (deadcyc_ == 20) {
            for (auto& o : c_->orbs_) {
                o.d_max_ = 2;
            }
        }

        for (auto& orb : (c_->orbs_)) {
            orb.step();
        }

        if (deadcyc_ > 20 and deadcyc_ % 8 == 0) {
            platform().speaker().play_sound("snd_explo2", deadcyc_);
        }

        if (deadcyc_ >= 60) {
            c_->orbs_.clear();
            kill();

            platform().speaker().play_sound("snd_explo4", 200);

            for (int x = 0; x < 64; ++x) {
                for (int y = 0; y < 64; ++y) {
                    platform().set_tile(Layer::map_1, x, y, 0);
                }
            }

            engine().g_.screenshake_ = 12;

            auto pos = position_;
            pos.x += 40;
            pos.y += 40;
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
                                                     spawn_y_
                });

            engine().boss_completed();

            engine().add_object<Pickup>(position_, Pickup::suit);
        }
        return;
    }

    int shake = 0;
    if (engine().g_.screenshake_) {
        shake = (engine().g_.screenshake_ % 2) * 2;
    }

    platform().scroll(Layer::map_1,
                      -(position_.x - 40).as_integer(),
                      -(position_.y - 40).as_integer() + -shake);


    for (auto& s : engine().player_projectiles_) {
        if (s->x() < x() - 36 or
            s->x() > x() + 30 or
            s->y() < y() - 30 or
            s->y() > y() + 36) {
            if (overlapping_impl(*s)) {
                s->kill();
            }
        }
    }

    for (auto& orb : (c_->orbs_)) {
        orb.step();
    }

    if (move_) {
        movecyc_ += 1;
    }

    if (movecyc_ >= 4) {
        movecyc_ = 0;

        if (move_) {
            if (x() > 120 + 40 - 16) {
                move_ = 3;
            }
            if (y() < 80 - (40 - 16)) {
                move_ = 4;
            }
            if (x() < 120 - (40 - 16)) {
                move_ = 1;
            }
            if (y() > 80 + 40 - 16) {
                move_ = 2;
            }
        }

        switch (move_) {
        case 1:
            position_.x += 1;
            if (overlapping_impl(*engine().hero())) {
                engine().hero()->x() += 1;
            }
            break;

        case 2:
            position_.y -= 1;
            if (overlapping_impl(*engine().hero())) {
                engine().hero()->y() -= 1;
            }
            break;

        case 3:
            position_.x -= 1;
            if (overlapping_impl(*engine().hero())) {
                engine().hero()->x() -= 1;
            }
            break;

        case 4:
            position_.y += 1;
            if (overlapping_impl(*engine().hero())) {
                engine().hero()->y() += 1;
            }
            break;
        }

        for (auto& e : engine().enemies_) {
            if (auto p = dynamic_cast<RockSmasherCore*>(e.get())) {
                p->x() = x() + p->x_offset_;
                p->y() = y() + p->y_offset_;
            }
        }
    }

    switch (timeline_++) {
    case 2:
        if (!(engine().hero()->y() > y() - 23 and
              engine().hero()->y() < y() + 23 and
              engine().hero()->x() > x() - 23 and
              engine().hero()->x() < x() + 23)) {
            timeline_ = 0;
        } else {
            engine().swapsong("boss");
        }
        break;

    case 40:
        for (int i = 0; i < 8; ++i) {
            c_->orbs_.emplace_back(this,
                                Fixnum(i * 45),
                                Fixnum((256 + 32 * i) / 2),
                                Fixnum(128 / 2),
                                Fixnum(-0.5f),
                                1);

            c_->orbs_.emplace_back(this,
                                Fixnum(i * 45),
                                Fixnum((256 + 32 * i) / 2),
                                Fixnum(112 / 2),
                                Fixnum(1),
                                2);
        }
        break;

    case 80:
        move_ = rng::choice<5>(rng::utility_state);
        shotcyc_ += 1;
        shotcyc3_ += 1;

        if (shotcyc3_ == 1) {
            shotcyc3_ = 0;
            shotcyc4_ += 1;

            for (auto& e : engine().enemies_) {
                if (auto p = dynamic_cast<RockSmasherCore*>(e.get())) {
                    if (p->mode_ > 10 and shotcyc4_ + 10 == p->mode_) {
                        p->rfire_ = 1;
                        if (p->x() < x()) {
                            p->dir_ = 0;
                        } else {
                            p->dir_ = 180;
                        }
                    }
                }
            }

            if (shotcyc4_ == 8) {
                shotcyc4_ = 0;
            }
        }

        if (shotcyc_ == 2 or shotcyc_ == 6) {
            for (auto& o : (c_->orbs_)) {
                if (o.mode_ == 1) {
                    Vec2<Fixnum> v{o.x_, o.y_};
                    if (auto e = engine().add_object<EnemyShot>(v)) {
                        auto dir = direction(fvec(v),
                                             fvec(position()));
                        dir = rotate(dir, 32);
                        dir = dir * (1.5f / 2);
                        e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                    }
                }
            }
        }

        if (shotcyc_ == 4 or shotcyc_ == 8) {
            for (auto& o : (c_->orbs_)) {
                if (o.mode_ == 2) {
                    Vec2<Fixnum> v{o.x_, o.y_};
                    if (auto e = engine().add_object<Supershot>(v)) {
                        auto dir = direction(fvec(v),
                                             fvec(position()));
                        dir = rotate(dir, 360 - 26);
                        dir = dir * (1.5f / 2);
                        e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                    }
                }
            }
        }

        if (shotcyc_ == 10) {
            shotcyc_ = 0;
            shotcyc2_ += 1;
            if (shotcyc2_ == 1) {
                for (auto& o : (c_->orbs_)) {
                    if (o.mode_ == 2) {
                        Vec2<Fixnum> v{o.x_, o.y_};
                        if (auto e = engine().add_object<Supershot>(v)) {
                            auto dir = rotate({1, 0}, 270);
                            if (o.x_ > x()) {
                                dir = rotate({1, 0}, 90);
                            }
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                    }
                }
            }

            if (shotcyc2_ == 2) {
                shotcyc2_ = 0;
                for (auto& o : (c_->orbs_)) {
                    if (o.mode_ == 2) {
                        Vec2<Fixnum> v{o.x_, o.y_};
                        if (auto e = engine().add_object<Supershot>(v)) {
                            auto dir = rotate({1, 0}, 180);
                            if (o.y_ > y()) {
                                dir = rotate({1, 0}, 0);
                            }
                            e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                        }
                    }
                }
            }
        }

        break;

    case 119:
        timeline_ = 80;
        break;
    }

}



}
