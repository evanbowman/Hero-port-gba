#pragma once

#include "object.hpp"
#include "engine.hpp"



namespace herocore
{



class Explo : public Object
{
private:

    int anim_cyc_ = 0;

public:

    Explo(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 34;
    }


    void set_speed(const Vec2<Fixnum>& speed)
    {
        speed_ = speed;
    }


    void step() override
    {
        Object::step();

        anim_cyc_ += 1;
        if (anim_cyc_ == 3) {
            anim_cyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 3) {
                kill();
            }
        }
    }


private:

    int counter_ = 20;


};



class ExploSpewer2 : public Object
{
private:
    int cyc_ = 0;

public:

    ExploSpewer2(const Vec2<Fixnum>& pos)
    {
        position_ = pos;
    }


    void step() override
    {
        cyc_ += 1;

        for (int i = 0; i < 8; ++i) {
            int dir = 22.5f + i * 45 - cyc_ * 11;
            int dir2 = 22.5f + i * 45 + cyc_ * 11;
            int cyc2 = cyc_ * 12;

            if (dir >= 360) {
                dir -= 360;
            }
            if (dir2 >= 360) {
                dir2 -= 360;
            }

            static constexpr auto rot_lut = make_rotation_lut(0);
            auto rvec = rot_lut[dir];
            auto rvec2 = rot_lut[dir2];

            Vec2<Fixnum> pos;
            if (i % 2 == 1) {
                pos.x = x() + Fixnum(rvec.x * cyc2);
                pos.y = y() + Fixnum(rvec.y * cyc2);
            } else {
                pos.x = x() + Fixnum(rvec2.x * cyc2);
                pos.y = y() + Fixnum(rvec2.y * cyc2);
            }

            if (pos.x > 40 and pos.y > 0 and pos.x < 200 and pos.y < 160) {
                engine().add_object<Explo>(pos);
            }

        }

        if (cyc_ == 25) {
            kill();
        }
    }
};



}
