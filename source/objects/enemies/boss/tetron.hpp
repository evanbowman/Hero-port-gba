#pragma once

#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/megashot.hpp"


namespace herocore
{


class Tetron : public Enemy
{
public:

    Tetron(const Vec2<Fixnum>& pos);


    void step() override;

    void draw(Platform::Screen& s) const override;


    bool damage(Health dmg, Object& s) override;


private:
    int timeline_ = 0;
    u8 phase_ = 1;

    Vec2<Fixnum> dir_;
    Fixnum spd_;
};


} // namespace herocore
