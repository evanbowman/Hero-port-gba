#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/megashot.hpp"


namespace herocore
{



class Elite : public Enemy
{
public:


    Elite(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y);


    void step() override;


private:
    u8 spawn_x_;
    u8 spawn_y_;
    u8 friend_spawn_x_ = 0;
    u8 friend_spawn_y_ = 0;
    u8 pain_ = 0;
    u8 gotit_ = 0;
    u8 shotcyc_ = 0;
    u8 expel_ = 0;
    int timeline_ = 0;
    Vec2<Fixnum> dir_;
};



}
