#pragma once

#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/megashot.hpp"


namespace herocore
{


class LiquidMetalProcessor : public Enemy
{
public:
    LiquidMetalProcessor(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y);


    void step() override;


private:
    u8 spawn_x_;
    u8 spawn_y_;
    int timeline_ = 0;
};


} // namespace herocore
