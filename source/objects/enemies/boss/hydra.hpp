#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"



namespace herocore
{




class HydraHead : public Enemy
{
private:

    int timeline_ = 0;
    u16 xstepmax_ = 1;
    u16 ystepmax_ = 1;
    u16 xstep_ = 0;
    u16 ystep_ = 0;
    u16 x_start_ = 0;
    u16 y_start_ = 0;
    u8 xdir_ = rng::choice<2>(rng::utility_state);
    u8 ydir_ = rng::choice<2>(rng::utility_state);
    u8 copy_ = 0;
    bool move_ = false;

public:

    HydraHead(const Vec2<Fixnum>& pos, int timeline) :
        Enemy(TaggedObject::Tag::ignored, Health{16}),
        timeline_(timeline)
    {
        position_ = pos;

        x_start_ = pos.x.as_integer();
        y_start_ = pos.y.as_integer();

        hitbox_.dimension_.size_ = {18, 16};
        hitbox_.dimension_.origin_ = {9, 8};
        origin_ = {9, 8};

        sprite_index_ = 169;
    }


    void draw(Platform::Screen& s) const override
    {
        Enemy::draw(s);

        Sprite spr;
        spr.set_origin({origin_.x, origin_.y});
        spr.set_texture_index(168);
        spr.set_position({x() + 16, y()});
        s.draw(spr);
    }


    bool damage(Health dmg, Shot& s) override
    {
        if (sprite_subimage_ not_eq 2) {
            s.kill();
            return false;
        }
        health_ = std::max(0, health_ - dmg);
        s.kill();
        return true;
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            if (copy_ == 0) {
                auto h = engine().add_object<HydraHead>(Vec2<Fixnum>{x_start_, y_start_}, 40);
                if (h) {
                    h->copy_++;
                    h->health_ = 8;
                }
            } else if (copy_ == 1) {

            }
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
        if (x().as_integer() < x_start_ - 32) {
            xdir_ = 1;
        }
        if (x().as_integer() > x_start_ - 8) {
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
        if (y().as_integer() < y_start_ - 40) {
            ydir_ = 1;
        }
        if (y().as_integer() > y_start_ + 8) {
            ydir_ = 0;
        }

        Enemy::step();
    }

};




class Hydra : public Enemy
{
private:
    int timeline_ = 0;

public:

    Hydra(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y) :
        Enemy(TaggedObject::Tag::ignored, Health{9999})
    {
        position_ = pos;

        sprite_index_ = 0;

        hitbox_.dimension_.size_ = {42, 48};

        engine().add_object<HydraHead>(Vec2<Fixnum>{x(), y() + 7}, 40);
        engine().add_object<HydraHead>(Vec2<Fixnum>{x(), y() + 7 + 8}, 20);
        engine().add_object<HydraHead>(Vec2<Fixnum>{x(), y() + 7 + 16}, 0);
    }


    void step() override
    {
        if (health_ <= 0) {
            kill();
            engine().add_object<Explo>(position_);
            return;
        }

        switch (timeline_++) {
        case 0:
            platform().speaker().play_music("boss", 0);
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

};



}
