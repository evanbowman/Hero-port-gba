#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "number/random.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/gigashot.hpp"
#include "objects/particles/bigExplo.hpp"



namespace herocore
{



class Eyespy : public Enemy
{
public:

    Eyespy(const Vec2<Fixnum>& pos);


    void step() override;


    int collision_damage() const override
    {
        return 3;
    }


private:
    int timeline_ = 0;
    int movecyc_ = 0;
    u16 gigacyc_ = 0;
    u16 gigacyc2_ = 0;
public:
    bool wake_ = false;
};



}
