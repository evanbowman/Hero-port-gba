#include "rockSmasher.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/projectile/shot.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"



namespace herocore
{



class RockSmasherCore : public Enemy
{
private:
    RockSmasher* owner_ = nullptr;
    int animcyc_ = 0;
    Fixnum x_offset_;
    Fixnum y_offset_;

public:

    u8 rfire_ = 0;
    u8 rfirecyc_ = 0;
    u8 dir_ = 0;

    u8 mode_ = 0;

    RockSmasherCore(RockSmasher* owner,
                    Fixnum x_offset,
                    Fixnum y_offset,
                    u8 mode) :
        Enemy(TaggedObject::Tag::ignored, Health{32}),
        owner_(owner),
        x_offset_(x_offset),
        y_offset_(y_offset),
        mode_(mode)
    {
        hitbox_.dimension_.size_ = {4, 5};
        hitbox_.dimension_.origin_ = {3, 3};
        origin_ = {3, 3};

        sprite_index_ = 164;
    }


    void step() override
    {
        if (health_ == 0) {
            engine().add_object<ExploSpewer>(Vec2<Fixnum>{
                    x(), y()
                });
            kill();
            owner_->hp_--;
        }

        animcyc_ += 1;
        if (animcyc_ == 4) {
            animcyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 4) {
                sprite_subimage_ = 0;
            }
        }

        position_.x = owner_->position().x + x_offset_;
        position_.y = owner_->position().y + y_offset_;
    }
};



RockSmasher::RockSmasher(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y) :
    c_(allocate_dynamic<Components>(platform()))
{
    position_ = pos;

    platform().load_tile1_texture("rock_smasher");

    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            platform().set_tile(Layer::map_1, x, y, 24);
        }
    }

    draw_image(platform(), 2, 0, 0, 10, 10, Layer::map_1);

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(-54 / 2),
                                         Fixnum(-50 / 2),
                                         u8(18));

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(-54 / 2),
                                         Fixnum(-22 / 2),
                                         u8(17));


    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(-54 / 2),
                                         Fixnum(50 / 2),
                                         u8(15));

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(-54 / 2),
                                         Fixnum(22 / 2),
                                         u8(16));


    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(54 / 2),
                                         Fixnum(-50 / 2),
                                         11);

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(54 / 2),
                                         Fixnum(-22 / 2),
                                         12);


    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(54 / 2),
                                         Fixnum(50 / 2),
                                         14);

    engine().add_object<RockSmasherCore>(this,
                                         Fixnum(54 / 2),
                                         Fixnum(22 / 2),
                                         13);

}



void RockSmasher::draw(Platform::Screen& s) const
{
    for (auto& orb : (c_->orbs_)) {
        orb.draw(s);
    }

    Solid::draw(s);
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
    if (hp_ == 0) {
        c_->orbs_.clear();
        kill();
    }

    platform().scroll(Layer::map_1,
                      -(position_.x - 40).as_integer(),
                      -(position_.y - 40).as_integer());


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
    }

    switch (timeline_++) {
    case 2:
        if (!(engine().hero()->y() > y() - 23 and
              engine().hero()->y() < y() + 23 and
              engine().hero()->x() > x() - 23 and
              engine().hero()->x() < x() + 23)) {
            timeline_ = 0;
        } else {
            platform().speaker().play_music("boss", 0);
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
