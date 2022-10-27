#pragma once

#include "enemyProjectile.hpp"
#include "engine.hpp"
#include "objects/particles/explo.hpp"


namespace herocore
{


class Missile : public EnemyProjectile
{
private:
    Vec2<Fixnum> dir_;
    int anim_ = 0;
    int count_ = 0;
    int dircyc_ = 0;
    int ttl_ = 0;

public:
    Missile(const Vec2<Fixnum>& pos) : EnemyProjectile(pos, 3, 0)
    {
        sprite_index_ = 117;

        hitbox_.dimension_.size_ = {8, 8};
        hitbox_.dimension_.origin_ = {4, 4};
        origin_ = {8, 8};
    }


    void set_dir(Vec2<float> d)
    {
        dir_.x = Fixnum(d.x);
        dir_.y = Fixnum(d.y);
        speed_ = dir_ * Fixnum(1.5f / 2);
    }


    void step() override
    {
        if (not place_free({position_.x + speed_.x, position_.y + speed_.y})) {
            engine().add_object<Explo>(Vec2<Fixnum>{x() - 4, y() - 4});
            kill();
            return;
        }

        for (auto& s : engine().player_projectiles_) {
            if (s->hitbox().overlapping(hitbox_)) {
                kill();
                s->kill();
                platform().speaker().play_sound("snd_explo1", 6);
                engine().add_object<Explo>(Vec2<Fixnum>{x() - 4, y() - 4});
            }
        }


        if (++dircyc_ == 20) {
            dircyc_ = 0;
            // direction() is expensive, we can't call it every frame like the
            // original game does.
            auto dir =
                direction(fvec({x() + 4, y() + 4}), fvec(engine().hero()->position()));
            Vec2<Fixnum> fd;
            fd.x = Fixnum(dir.x);
            fd.y = Fixnum(dir.y);

            auto diff = dir_ - fd;
            diff.x *= Fixnum(0.4f);
            diff.y *= Fixnum(0.4f);
            dir_ = dir_ - diff;

            speed_ = dir_ * Fixnum(1.5f / 2);

            if (abs(dir_.y) < Fixnum(0.5f)) {
                sprite_subimage_ = 0;
                hflip_ = dir_.x < 0;
            } else if (abs(dir_.y) < Fixnum(0.7f)) {
                sprite_subimage_ = 1;
                vflip_ = dir_.y > 0;
                hflip_ = dir_.x < 0;
            } else {
                sprite_subimage_ = 2;
                vflip_ = dir_.y > 0;
                hflip_ = false;
            }
        }

        if (++ttl_ == 400) {
            kill();
            engine().add_object<Explo>(Vec2<Fixnum>{x() - 4, y() - 4});
        }

        Object::step();

        anim_ += 1;
        if (anim_ == 3) {
            sprite_index_ = 120;
        }
        if (anim_ >= 6) {
            anim_ = 0;
            sprite_index_ = 117;
        }
    }
};


} // namespace herocore
