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

    Fixnum x_start_ = 0;
    Fixnum y_start_ = 0;
    int timeline_ = 0;
    u16 xstepmax_ = 1;
    u16 ystepmax_ = 1;
    u16 xstep_ = 0;
    u16 ystep_ = 0;
    u8 xdir_ = rng::choice<2>(rng::utility_state);
    u8 ydir_ = rng::choice<2>(rng::utility_state);
    u8 copy_ = 0;
    bool move_ = false;
    bool hyper_ = false;
    bool flash_ = false;
    u8 deadcyc_ = 0;
    mutable u8 flashcyc_ = 0;

public:

    HydraHead(const Vec2<Fixnum>& pos, int timeline) :
        Enemy(TaggedObject::Tag::ignored, Health{16}),
        timeline_(timeline)
    {
        position_ = pos;

        x_start_ = pos.x;
        y_start_ = pos.y;

        hitbox_.dimension_.size_ = {18, 16};
        hitbox_.dimension_.origin_ = {9, 8};
        origin_ = {9, 8};

        sprite_index_ = 169;
    }


    void draw(Platform::Screen& s) const override;


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


    void step() override;
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
