#pragma once

#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/enemies/light/drone.hpp"
#include "objects/enemies/light/spew.hpp"
#include "objects/misc/pickup.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/megashot.hpp"


namespace herocore
{



class StarSplitter : public Enemy
{
private:
    Vec2<Fixnum> gravity_;
    Object* foot_1_ = nullptr;
    Object* foot_2_ = nullptr;
    int timeline_ = 0;

    u16 dummy_speed_ = 0;

    u8 deadcyc_ = 0;
    u8 core_count_ = 4;
    u8 shotcyc_ = 0;
    u8 shotcyc2_ = 0;
    u8 shotcycx_ = 0;

    u8 footcyc_ = 0;
    u8 footmode_ = 1;
    u8 footmode2_ = 0;

    s8 wait_ = -40;
    u8 bwait_ = 10;
    u8 bwaitcyc_ = 0;
    bool moving_ = false;

    bool dead_ = false;

public:
    u8 spawn_x_;
    u8 spawn_y_;

    s8 foot_ready_ = 0;
    u8 feet_moving_ = 2;


public:
    StarSplitter(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y);


    int collision_damage() const override
    {
        return 4;
    }


    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        auto x = this->x() - 16;
        auto y = this->y() - 16;
        spr_.set_texture_index(247);
        spr_.set_position({x, y});
        screen.draw(spr_);

        spr_.set_position({x + 16, y});
        spr_.set_flip({true, false});
        screen.draw(spr_);

        spr_.set_position({x, y + 16});
        spr_.set_flip({false, true});
        screen.draw(spr_);

        spr_.set_position({x + 16, y + 16});
        spr_.set_flip({true, true});
        screen.draw(spr_);
    }


    bool damage(Health dmg, Object& s);


    void step() override;


    void core_destroyed()
    {
        --core_count_;
    }

};


} // namespace herocore
