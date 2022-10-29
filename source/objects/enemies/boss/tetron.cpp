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



Tetron::Tetron(const Vec2<Fixnum>& pos)
    : Enemy(TaggedObject::Tag::ignored, Health{256})
{
    position_ = {pos.x - 4, pos.y - 4};
    sprite_index_ = 206;

    hitbox_.dimension_.size_ = {32, 60};
    hitbox_.dimension_.origin_ = {16, 30};
}



bool Tetron::damage(Health dmg, Object& s)
{
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

        engine().add_object<ExploSpewer>(position_);
        play_sound("snd_bossroar", 20);
        engine().g_.screenshake_ = 12;
    }

    if (phase_ == 3) {
        // TODO
        return;
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
        // TODO: liquid shift
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
        // TODO: liquidshift
        break;

    case 1160:
        spd_ = 0;
        break;

    case 1164:
        hflip_ = false;
        break;

    case 1168:
        laser(rotate({1, 0}, 202), -16, 4);
        laser(rotate({1, 0}, 270), -16, 4);
        break;

    case 1178:
        hflip_ = true;
        break;

    case 1188:
        laser(rotate({1, 0}, 337), 14, 4);
        laser(rotate({1, 0}, 270), 14, 4);
        break;

    case 1198:
        sprite_index_ = 214;
        hflip_ = false;
        break;

    case 1200:
        // todo: liquid
        break;

    case 1208:
        laser(rotate({1, 0}, 0), 0, -12);
        laser(rotate({1, 0}, 45), 0, -12);
        laser(rotate({1, 0}, 90), 0, -12);
        laser(rotate({1, 0}, 135), 0, -12);
        laser(rotate({1, 0}, 180), 0, -12);
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
        break;

    case 2030:
        break;

    case 2050:
        break;

    case 2070:
        break;

    case 2120:
        break;

    case 2160:
        break;

    case 2198:
        break;

    case 2200:
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
