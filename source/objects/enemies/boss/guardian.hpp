#pragma once


#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/megashot.hpp"


namespace herocore
{



class GuardianCore : public Enemy
{
public:


    GuardianCore(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y);


    void step() override;


    void flip();


    bool flip_ = false;


    bool damage(Health dmg, Shot& s) override;


    int collision_damage() const override
    {
        return 0;
    }


private:
    u8 spawn_x_;
    u8 spawn_y_;
    u8 shotcyc_ = 0;
    u8 detachcyc_ = 0;
    bool dir_ = false;
    int timeline_ = 0;
    int animcyc_ = 0;
    Fixnum upwards_speed_;
    Fixnum downwards_speed_;
    Fixnum gravity_;

    Enemy* upper_;
    Enemy* lower_;
};



}
