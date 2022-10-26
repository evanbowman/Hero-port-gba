#pragma once

#include "bitvector.hpp"
#include "color.hpp"
#include "number/fixnum.hpp"
#include "number/numeric.hpp"



using TextureIndex = u16;


class Sprite
{
public:
    enum Alpha : u8 {
        opaque,
        translucent,
        transparent, // invisible
        count
    };

    enum Size : u8 { w16_h16 };

    enum Flags1 : u8 {};
    enum Flags2 : u8 {};


    Sprite();


    void set_position(const Vec2<Fixnum>& position);


    void set_origin(const Vec2<s16>& origin);


    void set_texture_index(TextureIndex texture_index);


    void set_flip(const Vec2<bool>& flip);


    void set_alpha(Alpha alpha);


    void set_mix(const ColorMix& mix);


    void set_size(Size size);


    const Vec2<Fixnum>& get_position() const;


    const Vec2<s16>& get_origin() const;


    TextureIndex get_texture_index() const;


    Vec2<bool> get_flip() const;


    Alpha get_alpha() const;


    const ColorMix& get_mix() const;


    Size get_size() const;


    void set_priority(u8 priority)
    {
        priority_ = priority;
    }


    u8 get_priority() const
    {
        return priority_;
    }


    void set_palette(u8 palette)
    {
        palette_ = palette;
    }


    u8 palette() const
    {
        return palette_;
    }


private:
    // For the gameboy advance edition of the game, all the data for the engine
    // is designed to fit within IWRAM, so we need to be careful about
    // memory. Packing the engine into 32kB has benefits for other platforms
    // too--this game is very cache-friendly.
    u8 alpha_ : 2;
    u8 size_ : 1;
    bool flip_x_ : 1;
    bool flip_y_ : 1;

    // Extra flags reserved for future use.
    u8 flags1_ : 3;
    u8 priority_ : 2;
    u8 palette_ : 1;
    u8 reserved_ : 2;

    // Because sprites are only 16x32 or 32x32, 16bits for the origin field is
    // quite generous...
    Vec2<s16> origin_;

public:
    Vec2<Fixnum> position_;

private:
    TextureIndex texture_index_ = 0;
    ColorMix mix_;
};



// Warning: The class requires the initial mix amount passed in to be a multiple
// of five. There is a reason for this, and it has to do with smooth animations
// when the game logic is run synchronously (dt is > 1000 microseconds per
// step).
template <Microseconds Interval> class FadeColorAnimation
{
public:
    inline void advance(Sprite& sprite, Microseconds dt)
    {
        const auto& cmix = sprite.get_mix();
        if (cmix.amount_ > 0) {
            timer_ += dt;
            if (timer_ > Interval) {
                timer_ -= Interval;
                sprite.set_mix({cmix.color_, u8(cmix.amount_ - 5)});
            }
        } else {
            timer_ = 0;
            sprite.set_mix({});
        }
    }

    inline void reverse(Sprite& sprite, Microseconds dt)
    {
        const auto& cmix = sprite.get_mix();
        if (cmix.color_ not_eq ColorConstant::null and cmix.amount_ < 255) {
            timer_ += dt;
            if (timer_ > Interval) {
                timer_ -= Interval;
                sprite.set_mix({cmix.color_, u8(cmix.amount_ + 5)});
            }
        } else {
            timer_ = 0;
            sprite.set_mix({});
        }
    }

private:
    Microseconds timer_ = 0;
};
