#pragma once

#include "object.hpp"


namespace herocore
{


class Hero : public Object
{
public:
    Hero(const Vec2<Fixnum>& position)
    {
        position_ = position;
        sprite_index_ = 1;
        origin_ = {6, 0};

        hitbox_.dimension_.size_ = {4, 8};
    }


    void damage()
    {
    }


    void step() override;
    void draw(Platform::Screen& screen) const override;


    void fireleft();
    void fireright();


    u8& blaster_level() const;
    u8& blade_level() const;
    u8& suit_level() const;
    u8& level() const;


private:
    Fixnum flyspeed_ = Fixnum(1);
    int autofire_index_ = 0;
    u8 jetpack_counter_ = 0;
    u8 heatcyc_ = 0;
    u8 chargeblade_ = 0;
    u8 chargebladeprev_ = 0;
    bool lv2_blaster_damage_inc_ = false;

    int blaster_damage();
};


} // namespace herocore
