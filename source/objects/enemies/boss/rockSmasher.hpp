#pragma once

#include "engine.hpp"
#include "objects/particles/explo.hpp"
#include "number/random.hpp"
#include "objects/solid.hpp"



namespace herocore
{



class RockSmasher : public Solid
{
public:

    RockSmasher(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y);


    void step() override;
    void draw(Platform::Screen& s) const override;


    bool overlapping_impl(Object& obj);


    bool overlapping(Object& obj) override;


    u8 hp_ = 8;
    u8 move_ = 0;

private:
    u8 movecyc_ = 0;

    u8 shotcyc_ = 0;
    u8 shotcyc2_ = 0;
    u8 shotcyc3_ = 0;
    u8 shotcyc4_ = 0;

    int timeline_ = 0;

    struct Orb
    {
        RockSmasher* owner_ = nullptr;
        Fixnum rot_ = 0;
        Fixnum d_ = 0;
        Fixnum d_max_ = 0;
        Fixnum rot_speed_;
        Fixnum x_;
        Fixnum y_;
        u8 mode_ = 0;
        u8 rfire_ = 0;
        u8 rfirecyc_ = 0;
        u8 dir_ = 0;

        Orb(RockSmasher* owner,
            Fixnum rot,
            Fixnum d,
            Fixnum d_max,
            Fixnum rot_speed,
            u8 mode) :
            owner_(owner),
            rot_(rot),
            d_(d),
            d_max_(d_max),
            rot_speed_(rot_speed),
            mode_(mode)
        {
        }

        void draw(Platform::Screen& s) const
        {
            if (y_ > 160 or y_ < 0 or x_ > 240 or x_ < 0) {
                return;
            }

            Sprite spr_;
            spr_.set_texture_index(125);
            spr_.set_position({x_, y_});
            s.draw(spr_);
        }


        void step()
        {
            if (d_ > d_max_) {
                d_ -= Fixnum(4);
            }

            rot_ += rot_speed_;

            if (rot_ > 360) {
                rot_ -= 360;
            }

            if (rot_ < 0) {
                rot_ += 360;
            }

            static constexpr auto rot_lut = make_rotation_lut(0);
            auto rvec = rot_lut[rot_.as_integer()];

            auto result = owner_->position() + rvec * d_;
            x_ = result.x;
            y_ = result.y;
        }
    };

    using OrbBuffer = Buffer<Orb, 16>;
    struct Components
    {
        OrbBuffer orbs_;
    };
    DynamicMemory<Components> c_;
};



}
