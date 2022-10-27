#pragma once

#include "enemyProjectile.hpp"
#include "engine.hpp"
#include "objects/particles/bigExplo.hpp"


namespace herocore
{


class Vortex2 : public EnemyProjectile
{
private:
    int count_ = 0;
    int cyc_ = 0;

public:
    Vortex2(const Vec2<Fixnum>& pos) : EnemyProjectile(pos, 4, engine().g_.maxheat_)
    {
        sprite_index_ = 54;

        force_ = 4;
        hitbox_.dimension_.size_ = {8, 6};
    }


    void step() override
    {
        count_ += 1;
        cyc_ += 1;
        if (cyc_ == 4) {
            sprite_subimage_ += 1;
            cyc_ = 0;
        }
        if (count_ == 4) {
            force_ = 0;
            heat_ = 0;
        }
        if (count_ >= 16) {
            kill();
        }
    }

    void on_hero_collision() override
    {
        // Passes through fliphero.
    }
};


class Vortex : public EnemyProjectile
{
private:
    int cyc_ = 0;

public:
    Vortex(const Vec2<Fixnum>& pos) : EnemyProjectile(pos, 0, 0)
    {
        sprite_index_ = 54;

        hitbox_.dimension_.size_ = {8, 8};

        // TODO: screenshake
        if (engine().g_.difficulty_ == Difficulty::hard) {
            speed_.x = 8;
        } else {
            speed_.x = 4;
        }
    }


    void set_left()
    {
        if (speed_.x > 0) {
            speed_.x = speed_.x * -1;
        }
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            kill();
            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 8 - 4, position_.y + 4});
            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 8 + 4, position_.y + 4});
            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 8 - 8, position_.y + 4});
            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 8 + 8, position_.y + 4});
            return;
        }

        Object::step();

        cyc_ += 1;
        if (cyc_ == 1) {
            cyc_ = 0;
            engine().add_object<Vortex2>(position_);
        }
    }
};


} // namespace herocore
