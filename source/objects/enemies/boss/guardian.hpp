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


private:
    u8 spawn_x_;
    u8 spawn_y_;
    int timeline_ = 0;
    int animcyc_ = 0;
    Vec2<Fixnum> dir_;

    Enemy* upper_;
    Enemy* lower_;
};



}