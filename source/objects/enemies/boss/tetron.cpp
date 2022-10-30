#include "tetron.hpp"
#include "objects/projectile/laser.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/gigashot.hpp"



namespace herocore
{



class SupershotG : public EnemyProjectile
{
private:
    int anim_ = 0;
    int count_ = 0;
    Vec2<Fixnum> grav_;

public:
    SupershotG(const Vec2<Fixnum>& pos) : EnemyProjectile(pos, 2, 0)
    {
        sprite_index_ = 14;
        hitbox_.dimension_.size_ = {7, 7};
    }


    void set_grav(const Vec2<Fixnum>& g)
    {
        grav_ = g;
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            kill();
            return;
        }

        speed_ = speed_ + grav_;

        Object::step();

        anim_ += 1;
        if (anim_ == 2) {
            anim_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ > 2) {
                sprite_subimage_ = 0;
            }
        }
        if (speed_.x == 0 and speed_.y == 0) {
            count_ += 1;
            if (count_ > 80) {
                kill();
                // TODO: create obj_explo
            }
        }
    }
};




class SpikeBall : public Enemy
{
public:

    SpikeBall(Tetron* owner,
              const Vec2<Fixnum>& position,
              Fixnum d_max,
              Fixnum rot) :
        Enemy(TaggedObject::Tag::ignored, Health{512}),
        owner_(owner),
        d_max_(d_max),
        rot_(rot)
    {
        hitbox_.dimension_.size_ = {16, 16};
        hitbox_.dimension_.origin_ = {8, 8};
        origin_ = {8, 8};

        position_ = position;
        sprite_index_ = 225;
        center_ = position_;
    }


    bool damage(Health dmg, Object& s) override
    {
        s.kill();
        return false;
    }


    int collision_damage() const override
    {
        return 3;
    }


    void step() override
    {
        if (d_ < d_max_) {
            d_ += Fixnum(0.5f);
        }

        rot_ += Fixnum(0.4f);
        if (rot_ > 360) {
            rot_ -= 360;
        }
        if (rot_ < 0) {
            rot_ += 360;
        }

        static constexpr auto rot_lut = make_rotation_lut(0);
        auto rvec = rot_lut[rot_.as_integer()];

        position_ = center_ + rvec * d_;

        if (x() > 240 or x() < -8 or y() < -8 or y() > 160) {
            kill();
        }
    }


private:
    Tetron* owner_;
    Fixnum d_max_;
    Fixnum d_ = 0;
    Fixnum rot_ = 0;
    Vec2<Fixnum> center_;
};



class TetronArm : public Enemy
{
public:

    TetronArm(Tetron* owner,
              const Vec2<Fixnum>& position) :
        Enemy(TaggedObject::Tag::ignored, Health{9999}),
        owner_(owner)
    {
        position_ = position;
        sprite_index_ = 235;
        origin_ = {16, 8};
        hitbox_.dimension_.origin_ = {0, 2};
        hitbox_.dimension_.size_ = {32, 12};
        xhold_ = x();
        yhold_ = y();
        xorig_ = x();
        yorig_ = y();
    }


    bool damage(Health dmg, Object& s) override
    {
        s.kill();
        return false;
    }


    void step() override
    {
        switch (shift_) {
        case 3:
            break;

        case 1:
            if (owner_->get_flip().x) {
                auto dir = rotate({1, 0}, 45);
                speed_.x = Fixnum(dir.x);
                speed_.y = -1 * Fixnum(dir.y);
            } else {
                auto dir = rotate({1, 0}, 135);
                speed_.x = Fixnum(dir.x);
                speed_.y = -1 * Fixnum(dir.y);
            }
            shiftcyc_ += 1;
            if (shiftcyc_ == 24) {
                speed_ = {};
                shift_ = 0;
                xhold_ = x();
                yhold_ = y();
            }
            break;

        case 2:
            if (owner_->get_flip().x) {
                auto dir = rotate({1, 0}, 45 + 180);
                speed_.x = Fixnum(dir.x);
                speed_.y = -1 * Fixnum(dir.y);
            } else {
                auto dir = rotate({1, 0}, 135 + 180);
                speed_.x = Fixnum(dir.x);
                speed_.y = -1 * Fixnum(dir.y);
            }
            shiftcyc_ += 1;
            if (shiftcyc_ == 24) {
                speed_ = {};
                shift_ = 0;
                x() = xorig_;
                y() = yorig_;
            }
            break;

        default:
            shiftcyc_ = 0;
            break;
        }

        Enemy::step();
    }


    void draw(Platform::Screen& s) const override
    {
        Sprite spr;
        spr.set_priority(2);
        auto p = position_;
        p.x -= 16;

        spr.set_position(position_);

        if (owner_->get_flip().x) {
            spr.set_flip({true, false});
            spr.set_texture_index(236);
            s.draw(spr);
            spr.set_texture_index(235);
            p.x += 32;
            spr.set_position(p);
            s.draw(spr);
        } else {
            spr.set_texture_index(235);
            s.draw(spr);
            spr.set_texture_index(236);
            p.x += 32;
            spr.set_position(p);
            s.draw(spr);
        }
    }


    Tetron* owner_;
    u8 shiftcyc_ = 0;
    u8 shift_ = 0;
    Fixnum xhold_;
    Fixnum yhold_;
    Fixnum xorig_;
    Fixnum yorig_;
};



Tetron::Tetron(const Vec2<Fixnum>& pos)
    : Enemy(TaggedObject::Tag::ignored, Health{256})
{
    position_ = {pos.x - 4, pos.y - 4};
    sprite_index_ = 206;

    hitbox_.dimension_.size_ = {32, 60};
    hitbox_.dimension_.origin_ = {16, 30};

    platform().load_tile1_texture("tile1");
    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            platform().set_tile(Layer::map_1, x, y, 0);
        }
    }

    for (int i = 0; i < 20; ++i) {
        platform().set_tile(Layer::map_1, 5 + i, 20, 9);
        for (int j = 0; j < 19; ++j) {
            platform().set_tile(Layer::map_1, 5 + i, 21 + j, 17);
        }
    }

}



bool Tetron::damage(Health dmg, Object& s)
{
    if (dead_) {
        s.kill();
        return false;
    }

    if (sprite_index_ == 226) {
        auto hb2 = hitbox_;
        hb2.dimension_.size_ = {24, 48};
        hb2.dimension_.origin_ = {13, 14};
        if (hb2.overlapping(s.hitbox())) {
            s.kill();
        }

        Hitbox hb = hitbox_;
        hb.dimension_.size_ = {24, 10};
        hb.dimension_.origin_ = {13, 16};
        if (hb.overlapping(s.hitbox())) {
            health_ = std::max(0, health_ - dmg);
            play_sound("snd_hit1", 1);
            s.kill();
            return true;
        }

        return false;
    }

    auto hb2 = hitbox_;
    hb2.dimension_.size_ = {24, 60};
    hb2.dimension_.origin_ = {13, 30};

    if (hb2.overlapping(s.hitbox())) {
        s.kill();
    } else {
        return false;
    }


    if (sprite_index_ == 214) {
        return false;
    }

    Hitbox hb = hitbox_;
    hb.dimension_.size_ = {24, 11};
    hb.dimension_.origin_ = {13, 30};
    if (hb.overlapping(s.hitbox())) {
        health_ = std::max(0, health_ - dmg);
        play_sound("snd_hit1", 1);
        return true;
    }

    return false;
}



void Tetron::step()
{
    auto laser =
        [&](Vec2<Float> dir,
            int xoff,
            int yoff,
            Float speed = 1.25f,
            Fixnum gravity = 0) {
            auto p = position_;
            p.x += xoff;
            p.y += yoff;
            p.y -= 2;
            if (phase_ == 4) {
                dir.y *= -1;
            }
            if (auto l = engine().add_object<Laser>(p, dir)) {
                dir = dir * speed;
                l->set_speed(Vec2<Fixnum>{Fixnum(dir.x), Fixnum(dir.y)});
                l->set_grav(gravity * Vec2<Fixnum>{Fixnum(dir.x), Fixnum(dir.y)});
            }
        };

    auto laser_fixme =
        [&](Vec2<Float> dir,
            int xoff,
            int yoff,
            Float speed = 1.25f,
            Fixnum gravity = 0) {
            auto p = position_;
            p.x += xoff;
            p.y += yoff;
            p.y -= 2;
            if (auto l = engine().add_object<Laser>(p, dir)) {
                dir = dir * speed;
                l->set_speed(Vec2<Fixnum>{Fixnum(dir.x), Fixnum(dir.y)});
                l->set_grav(gravity * Vec2<Fixnum>{Fixnum(dir.x), Fixnum(dir.y)});
            }
        };


    auto gigashot =
        [&](Vec2<Float> dir,
            int xoff,
            int yoff,
            Float speed = 1.f,
            Fixnum gravity = 0) {
            auto p = position_;
            p.x += xoff;
            p.y += yoff;
            p.x -= 6;
            p.y -= 6;
            if (auto l = engine().add_object<Gigashot>(p)) {
                dir = dir * speed;
                l->set_speed(Vec2<Fixnum>{Fixnum(dir.x), Fixnum(dir.y)});
                l->set_grav(gravity * Vec2<Fixnum>{Fixnum(dir.x), Fixnum(dir.y)});
            }
        };

    auto dir_player =
        [&](int xoff, int yoff) {
            auto p = Vec2<Float>{position_.x.as_float() + xoff,
                             position_.y.as_float() + yoff};
            return direction(p, fvec(engine().hero()->position()));
        };

    auto move_to_hero =
        [&](int rot = 0) {
            auto d = dir_player(0, 0);
            if (rot) {
                d = rotate(d, rot);
            }
            dir_.x = Fixnum(d.x);
            dir_.y = Fixnum(d.y);
        };

    auto update_liquid_metal =
        [&] {
            auto& liquid_level_ = engine().g_.fluid_level_;
            liquid_level_ += liquid_spd_;
            liquid_level_ = clamp(liquid_level_, Fixnum(0), Fixnum(152));
            platform().scroll(Layer::map_1, 0, 160 - engine().g_.fluid_level_.as_integer() - 8);

            if (++liquidcyc_ < 4) {
                return;
            }
            liquidcyc_ = 0;
            auto t = platform().get_tile(Layer::map_1, 5, 20);
            t += 1;
            t %= 256;
            if (t == 16) {
                t = 9;
            }
            int idx = t - 9;
            for (int i = 0; i < 20; ++i) {
                platform().set_tile(Layer::map_1, 5 + i, 20, t);
                for (int j = 0; j < 19; ++j) {
                    platform().set_tile(Layer::map_1, 5 + i, 21 + j, 17 + idx);
                }
            }
        };

    if (phase_ > 1) {
        update_liquid_metal();
    }

    if (phase_ == 1 and health_ < 128) {
        phase_ = 2;
        timeline_ = 1000;
        sprite_index_ = 206;
        engine().add_object<ExploSpewer>(position_);
        play_sound("snd_bossroar", 20);
        engine().g_.screenshake_ = 12;
    }

    if (phase_ == 2 and health_ < 64) {
        phase_ = 3;
        spd_ = 0;

        spd_ = 2;
        if (x() > engine().hero()->x()) {
            dir_ = {1, 0};
        } else {
            dir_ = {-1, 0};
        }
        speed_ = dir_ * spd_;
        liquid_spd_ = 3;
        engine().add_object<ExploSpewer>(position_);
        play_sound("snd_bossroar", 20);
        engine().g_.screenshake_ = 12;
    }

    if (phase_ == 3) {
        auto x = position_.x;
        auto y = position_.y;
        speed_.y += Fixnum(0.05f);

        if (not place_free({position_.x + speed_.x, position_.y + speed_.y}, 4)) {
            if (not place_free({x + 3, y}, 4) or not place_free({x - 3, y}, 4)) {
                speed_.x = 0;
            }
        }

        if (not place_free({x, y + 1}, 4)) {
            speed_.y = speed_.y * -1;
            spd_ = spd_ * -1;

            while (not place_free({this->x(), this->y()}, 4)) {
                this->y() -= 1;
            }

            speed_ = {};
            dir_ = {};
            spd_ = 0;

            phase_ = 4;
            engine().g_.screenshake_ = 12;
            timeline_ = 2000;
            sprite_index_ = 226;

            auto arm_pos = position_;
            if (hflip_) {
                arm_pos.x -= 11;
            } else {
                arm_pos.x -= 18;
            }
            arm_pos.y += 2;

            if (auto e = engine().add_object<TetronArm>(this, arm_pos)) {
                arm_ = e;
            } else {
                // Raise error? Should always be enough entities though...
            }
        }

        Enemy::step();
        return;
    }

    if (phase_ == 4) {
        if (health_ == 0 and not dead_) {
            dead_ = true;
            play_sound("snd_explo3", 80);
            platform().speaker().stop_music();
            engine().g_.tetron_dead_ = true;
            return;
        }

        if (dead_) {
            deadcyc_ += 1;
            if (deadcyc_ == 120) {
                platform().screen().fade(0);
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
            }

            if (deadcyc_ < 180) {
                int interval = 24;
                if (deadcyc_ > 130) {
                    interval = 6;
                } else if (deadcyc_ > 90) {
                    interval = 8;
                } else if (deadcyc_ > 50) {
                    interval = 12;
                } else if (deadcyc_ > 20) {
                    interval = 16;
                }
                if (deadcyc_ % interval == 0) {
                    engine().g_.screenshake_ = 6;
                    if (auto exp = engine().add_object<BigExplo>(
                            rng::sample<16>(position_, rng::critical_state))) {
                        exp->set_speed({0, -rng::choice<3>(rng::utility_state)});
                        engine().g_.screenshake_ = 6;
                        platform().speaker().play_sound("snd_explo2", deadcyc_ / 8);
                    }
                }
            }

            if (deadcyc_ == 1) {
                platform().screen().fade(1.f, ColorConstant::silver_white);
                play_sound("snd_explo2", 100);
            }
            if (deadcyc_ == 200) {
                for (int i = 0; i < 360; i += 20) {
                    for (int j = 0; j < 20; j += 4) {
                        if (auto exp = engine().add_object<BigExplo>(position_)) {
                            auto dir = rotate({1, 0}, i + j * 5);
                            dir = dir * (j / 2.f);
                            Vec2<Fixnum> spd;
                            spd.x = Fixnum(dir.x);
                            spd.y = Fixnum(dir.y);
                            exp->set_speed(spd);
                            engine().g_.screenshake_ = 24;
                        }
                    }
                }
                for (auto& e : engine().enemies_) {
                    e->kill();
                }
                engine().g_.tetron_destroyed_ = true;
                play_sound("snd_explo4", 100);
            }
            return;
        }
    }

    switch (timeline_++) {
    case 0:
        platform().speaker().stop_music();
        break;

    case 40:
        engine().swapsong("tetron");
        break;

    case 119:
        break;

    case 120: {
        spd_ = 1;
        move_to_hero(180);
        sprite_index_ = 206;
        hflip_ = x() < engine().hero()->x();

        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        break;
    }

    case 150:
        spd_ = 0;
        if (not hflip_) {
            laser(rotate({1, 0}, 180), -16, 4);
            laser(rotate({1, 0}, 115), -16, 4);
            laser(rotate({1, 0}, 245), -16, 4);
        } else {
            laser(rotate({1, 0}, 0), 14, 4);
            laser(rotate({1, 0}, 65), 14, 4);
            laser(rotate({1, 0}, 295), 14, 4);
        }
        break;

    case 180:
        spd_ = 1;
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(rotate({1, 0}, 147), -16, 4);
            laser(rotate({1, 0}, 212), -16, 4);
            move_to_hero(180 + 30);
        } else {
            laser(rotate({1, 0}, 32), 14, 4);
            laser(rotate({1, 0}, 360 - 32), 14, 4);
            move_to_hero(180 - 30);
        }
        break;

    case 210:
        sprite_index_ = 214;
        hflip_ = false;

        spd_ = 0;

        for (int i = 0; i < 8; ++i) {
            auto p = position_;
            p.x -= 3;
            p.y -= 3;
            if (auto e = engine().add_object<SupershotG>(p)) {
                auto d = rotate({1, 0}, i * 45);
                d = d * (1.8f / 2);
                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                e->set_grav(Fixnum(0.04f) * Vec2<Fixnum>{Fixnum(d.x), Fixnum(d.y)});
            }
        }

        break;

    case 240:
        for (int i = 0; i < 8; ++i) {
            auto p = position_;
            p.x -= 3;
            p.y -= 3;
            if (auto e = engine().add_object<SupershotG>(p)) {
                auto d = rotate({1, 0}, i * 45 + 22);
                d = d * (1.8f / 2);
                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                e->set_grav(Fixnum(0.04f) * Vec2<Fixnum>{Fixnum(d.x), Fixnum(d.y)});
            }
        }
        break;

    case 270:
        for (int i = 0; i < 8; ++i) {
            auto p = position_;
            p.x -= 3;
            p.y -= 3;
            auto d = rotate({1, 0}, i * 45);
            laser(d, 0, 0, 1.8f / 2, Fixnum(0.04f));
        }
        break;

    case 330:
        spd_ = 1;
        move_to_hero(rng::choice<180>(rng::critical_state) + 90);
        sprite_index_ = 206;
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        break;

    case 360: {
        move_to_hero(rng::choice<180>(rng::critical_state) + 90);
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(rotate({1, 0}, 180), -16, 4);
            laser(rotate({1, 0}, 115), -16, 4);
            laser(rotate({1, 0}, 245), -16, 4);
        } else {
            laser(rotate({1, 0}, 0), 14, 4);
            laser(rotate({1, 0}, 65), 14, 4);
            laser(rotate({1, 0}, 295), 14, 4);
        }
        break;
    }

    case 390: {
        spd_ = Fixnum(0.5f);
        Vec2<Float> d;
        d.x = dir_.x.as_float();
        d.y = dir_.y.as_float();
        d = rotate(d, 180);
        dir_.x = Fixnum(d.x);
        dir_.y = Fixnum(d.y);

        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(rotate(dir_player(-16, 4), 360 - 40), -16, 4);
            laser(rotate(dir_player(-16, 4), 40), -16, 4);
        } else {
            laser(rotate(dir_player(14, 4), 360 - 40), 14, 4);
            laser(rotate(dir_player(14, 4), 40), 14, 4);
        }
        break;
    }

    case 420:
        spd_ = 0;
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(rotate(dir_player(-16, 4), 360 - 60), -16, 4);
            laser(rotate(dir_player(-16, 4), 60), -16, 4);
        } else {
            laser(rotate(dir_player(14, 4), 360 - 60), 14, 4);
            laser(rotate(dir_player(14, 4), 60), 14, 4);
        }
        break;

    case 450:
        spd_ = Fixnum(0.5f);
        move_to_hero();
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            gigashot(dir_player(-16, 4), -16, 4, 1.f, Fixnum(0.025f));
            gigashot(rotate(dir_player(-16, 4), 70), -16, 4, 1.f, Fixnum(0.05f));
            gigashot(rotate(dir_player(-16, 4), 360 - 70), -16, 4, 1.f, Fixnum(0.05f));
        } else {
            gigashot(dir_player(14, 4), 14, 4, 1.f, Fixnum(0.025f));
            gigashot(rotate(dir_player(14, 4), 70), 14, 4, 1.f, Fixnum(0.05f));
            gigashot(rotate(dir_player(14, 4), 360 - 70), 14, 4, 1.f, Fixnum(0.05f));
        }
        break;

    case 480: {
        spd_ = 1;
        Vec2<Float> d;
        d.x = dir_.x.as_float();
        d.y = dir_.y.as_float();
        d = rotate(d, 180);
        dir_.x = Fixnum(d.x);
        dir_.y = Fixnum(d.y);

        if (not hflip_) {
            laser(rotate({1, 0}, 180), -16, 4);
            laser(rotate({1, 0}, 135), -16, 4);
            laser(rotate({1, 0}, 225), -16, 4);
        } else {
            laser(rotate({1, 0}, 0), 14, 4);
            laser(rotate({1, 0}, 45), 14, 4);
            laser(rotate({1, 0}, 315), 14, 4);
        }
        break;
    }

    case 510:
        spd_ = 0.5f;
        move_to_hero();
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        break;

    case 540:
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        break;

    case 570:
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        break;

    case 600:
        spd_ = 0;
        sprite_index_ = 214;
        hflip_ = false;

        for (int i = 0; i < 8; ++i) {
            auto p = position_;
            p.x -= 3;
            p.y -= 3;
            auto d = rotate({1, 0}, i * 45);
            laser(d, 0, 0, 1.8f / 2, Fixnum(0.05f));
        }
        break;

    case 630:
        for (int i = 0; i < 8; ++i) {
            auto p = position_;
            p.x -= 3;
            p.y -= 3;
            auto d = rotate({1, 0}, i * 45 + 30);
            laser(d, 0, 0, 1.8f / 2, Fixnum(0.05f));
        }
        break;

    case 660:
        for (int i = 0; i < 8; ++i) {
            auto p = position_;
            p.x -= 3;
            p.y -= 3;
            engine().add_object<SpikeBall>(this,
                                           position_,
                                           320,
                                           i * 60);
        }
        break;

    case 689:
        timeline_ = 120;
        break;

    case 1000:
        spd_ = 0;
        break;

    case 1040:
        spd_ = Fixnum(1.5f / 2);
        move_to_hero();
        hflip_ = x() < engine().hero()->x();
        if (liquidshift_ == 0) {
            liquidshift_ = 1;
            liquid_spd_ = -2;
        } else {
            liquidshift_ = 0;
            liquid_spd_ = 2;
        }
        break;

    case 1080:
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        break;

    case 1120:
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        liquid_spd_ = 0;
        break;

    case 1160:
        spd_ = 0;
        break;

    case 1164:
        hflip_ = false;
        break;

    case 1168:
        laser(rotate({-1, 0}, 337), -16, 4);
        laser(rotate({-1, 0}, 270), -16, 4);
        break;

    case 1178:
        hflip_ = true;
        break;

    case 1188:
        laser(rotate({-1, 0}, 202), 14, 4);
        laser(rotate({-1, 0}, 270), 14, 4);
        break;

    case 1198:
        sprite_index_ = 214;
        hflip_ = false;
        break;

    case 1200:
        liquid_spd_ = 0;
        break;

    case 1208:
        laser(rotate({-1, 0}, 0), 0, -12);
        laser(rotate({-1, 0}, 45), 0, -12);
        laser(rotate({-1, 0}, 90), 0, -12);
        laser(rotate({-1, 0}, 135), 0, -12);
        laser(rotate({-1, 0}, 180), 0, -12);
        break;

    case 1240:
        sprite_index_ = 206;
        spd_ = 1.5f / 2;
        hflip_ = x() < engine().hero()->x();
        move_to_hero(rng::choice<360>(rng::critical_state));
        break;

    case 1280:
        move_to_hero(rng::choice<360>(rng::critical_state));
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        break;

    case 1320:
        spd_ = 0;
        break;

    case 1340:
        hflip_ = x() < engine().hero()->x();
        if (not hflip_) {
            laser(dir_player(-16, 4), -16, 4);
        } else {
            laser(dir_player(14, 4), 14, 4);
        }
        break;

    case 1379:
        timeline_ = 1040;
        break;

    case 2000:
        if (arm_) {
            ((TetronArm*)arm_)->shift_ = 1;
        }
        break;

    case 2030:
        if (not hflip_) {
            laser(rotate({1, 0}, 157), -24, -5);
        } else {
            laser(rotate({1, 0}, 22), 22, -5);
        }
        break;

    case 2050:
        if (not hflip_) {
            laser(rotate({1, 0}, 157 - 40), -24, -5);
            laser(rotate({1, 0}, 157 + 40), -24, -5);
        } else {
            laser(rotate({1, 0}, 22 + 40), 22, -5);
            laser(rotate({1, 0}, 22 + 360 - 40), 22, -5);
        }
        break;

    case 2070:
        if (not hflip_) {
            laser(rotate({1, 0}, 157 - 80), -24, -5);
            laser(rotate({1, 0}, 157 + 80), -24, -5);
        } else {
            laser(rotate({1, 0}, 22 + 80), 22, -5);
            laser(rotate({1, 0}, 22 + 360 - 80), 22, -5);
        }
        break;

    case 2120:
        if (not hflip_) {
            laser_fixme(dir_player(-24, -5), -24, -5);
        } else {
            laser_fixme(dir_player(22, -5), 22, -5);
        }
        break;

    case 2160:
        if (not hflip_) {
            laser_fixme(rotate(dir_player(-24, 5), 330), -24, -5);
            laser_fixme(rotate(dir_player(-24, 5), 30), -24, -5);
        } else {
            laser_fixme(rotate(dir_player(22, -5), 330), 22, -5);
            laser_fixme(rotate(dir_player(22, -5), 30), 22, -5);
        }
        break;

    case 2198:
        if (arm_) {
            ((TetronArm*)arm_)->shift_ = 0;
            ((TetronArm*)arm_)->x() = ((TetronArm*)arm_)->xhold_;
            ((TetronArm*)arm_)->y() = ((TetronArm*)arm_)->yhold_;
        }
        break;

    case 2200:
        if (arm_) {
            ((TetronArm*)arm_)->shift_ = 2;
        }
        if (not hflip_) {
            for (int i = 0; i < 5; ++i) {
                auto p = position_;
                p.x -= 3;
                p.y -= 3;
                if (auto e = engine().add_object<SupershotG>(p)) {
                    auto d = rotate({1, 0}, i * 45 + 45);
                    d = d * (0.5f / 2);
                    d.y *= -1;
                    e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                    e->set_grav(Fixnum(0.1f) * Vec2<Fixnum>{Fixnum(d.x), Fixnum(d.y)});
                }
                if (auto e = engine().add_object<Gigashot>(p)) {
                    auto d = rotate({1, 0}, i * 45 + 22);
                    d = d * (2.5f / 2);
                    d.y *= -1;
                    e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                }
            }
        } else {
            for (int i = 0; i < 5; ++i) {
                auto p = position_;
                p.x -= 3;
                p.y -= 3;
                if (auto e = engine().add_object<SupershotG>(p)) {
                    auto d = rotate({1, 0}, i * 45 - 45);
                    d = d * (0.5f / 2);
                    d.y *= -1;
                    e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                    e->set_grav(Fixnum(0.1f) * Vec2<Fixnum>{Fixnum(d.x), Fixnum(d.y)});
                }
                if (auto e = engine().add_object<Gigashot>(p)) {
                    auto d = rotate({1, 0}, i * 45 - 22);
                    d = d * (2.5f / 2);
                    d.y *= -1;
                    e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                }
            }
        }

        break;

    case 2279:
        timeline_ = 2000;
        break;
    }

    auto x = position_.x;
    auto y = position_.y;
    speed_ = dir_ * spd_;
    if (not place_free({position_.x + speed_.x, position_.y + speed_.y}, 4)) {
        if (not place_free({x + 1, y}, 4) or not place_free({x - 1, y}, 4)) {
            speed_.x = speed_.x * -1;
            spd_ = spd_ * -1;
        }
        if (not place_free({x, y + 1}, 4) or not place_free({x, y - 1}, 4)) {
            speed_.y = speed_.y * -1;
            spd_ = spd_ * -1;
        }
    }

    Enemy::step();
}



void Tetron::draw(Platform::Screen& s) const
{
    Sprite spr_;
    spr_.set_texture_index(sprite_index_);
    s.draw(spr_);

    spr_.set_flip({hflip_, false});


    auto center = position_;
    center.x -= 16;
    center.y -= 30;


    auto draw_part =
        [&](int x, int y, u8 t) {
            auto pos = center;
            pos.x += x;
            pos.y += y;
            spr_.set_position(pos);
            spr_.set_texture_index(t);
            s.draw(spr_);
        };

    u8 t = sprite_index_;

    // Special case: third form uses different sprite layout, because it's wider
    // and shorter.
    if (sprite_index_ == 226) {
        center.y += 16;
        if (hflip_) {
            center.x -= 16;
            draw_part(32, 0, t++);
            draw_part(16, 0, t++);
            draw_part(0, 0, t++);

            draw_part(32, 16, t++);
            draw_part(16, 16, t++);
            draw_part(0, 16, t++);

            draw_part(32, 32, t++);
            draw_part(16, 32, t++);
            draw_part(0, 32, t++);

        } else {
            draw_part(0, 0, t++);
            draw_part(16, 0, t++);
            draw_part(32, 0, t++);

            draw_part(0, 16, t++);
            draw_part(16, 16, t++);
            draw_part(32, 16, t++);

            draw_part(0, 32, t++);
            draw_part(16, 32, t++);
            draw_part(32, 32, t++);
        }
        return;
    }

    if (hflip_) {
        draw_part(16, 0, t++);
        draw_part(0, 0, t++);
        draw_part(16, 16, t++);
        draw_part(0, 16, t++);
        draw_part(16, 32, t++);
        draw_part(0, 32, t++);
        draw_part(16, 48, t++);
        draw_part(0, 48, t++);
    } else {
        draw_part(0, 0, t++);
        draw_part(16, 0, t++);
        draw_part(0, 16, t++);
        draw_part(16, 16, t++);
        draw_part(0, 32, t++);
        draw_part(16, 32, t++);
        draw_part(0, 48, t++);
        draw_part(16, 48, t++);
    }

}



}
