#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"
#include "objects/projectile/megashot.hpp"


namespace herocore
{



class Eliminator : public Enemy
{
public:


    Eliminator(const Vec2<Fixnum>& pos);
    ~Eliminator();


    void step() override;


    void draw(Platform::Screen& screen) const override;


private:
    u8 flamecyc_ = 1;
    u8 pain_ = 0;
    u8 gotit_ = 0;
    u8 shotcyc_ = 0;
    u8 expel_ = 0;
    u8 rfire_ = 0;
    u8 rfirecyc_ = 0;
    u8 target_x_ = 0;
    u8 target_y_ = 0;
    u8 escapecyc_ = 0;
    u8 heroup_ = 7;
    bool escape_ = false;
    bool move_ = false;
    int timeline_ = 0;
    Fixnum max_speed_;
};



}
