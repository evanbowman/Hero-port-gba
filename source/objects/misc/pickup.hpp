#pragma once

#include "engine.hpp"
#include "objects/particles/explo.hpp"
#include "objects/solid.hpp"


namespace herocore
{


class Pickup : public Solid
{
public:
    enum Type { blaster, blade, suit };


    Pickup(const Vec2<Fixnum>& pos, Type type) : type_(type)
    {
        position_ = pos;

        origin_ = {8, 8};

        switch (type) {
        case blaster:
            sprite_index_ = 191;
            break;

        case blade:
            sprite_index_ = 192;
            break;

        case suit:
            sprite_index_ = 193;
            break;
        }

        hitbox_.dimension_.size_ = {14, 14};
        hitbox_.dimension_.origin_ = {8, 8};
    }


    bool overlapping(Object& other) override
    {
        if (dead()) {
            return false;
        }

        if (&other == engine().hero()) {
            bool intersect = hitbox().overlapping(other.hitbox());

            if (intersect) {
                kill();
                switch (type_) {
                case blaster:
                    ++engine().p_->blaster_;
                    break;

                case blade:
                    ++engine().p_->blade_;
                    break;

                case suit:
                    ++engine().p_->suit_;
                    break;
                }
                engine().levelup();
                platform().speaker().play_sound("snd_pickup", 10);
            }

            return intersect;
        }
        return false;
    }


    void draw(Platform::Screen& s) const override
    {
        cyc_ += 1;
        if (cyc_ == 8) {
            cyc_ = 0;
        }

        if (cyc_ > 4) {
            Solid::draw(s);
        }
    }


private:
    Type type_;
    mutable u8 cyc_ = 0;
};


} // namespace herocore
