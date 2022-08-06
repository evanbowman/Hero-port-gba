#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "number/random.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/particles/bigExplo.hpp"



namespace herocore
{



class Phaze : public Enemy
{
private:
    bool move_ = false;
    bool shoot_ = false;

public:

    Phaze(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::phase, Health{8})
    {
        position_ = pos;

        sprite_index_ = 101;

        hitbox_.dimension_.size_ = {12, 12};
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            engine().add_object<BigExplo>(Vec2<Fixnum>{
                    position_.x + 6, position_.y + 6});
            return;
        }

        const bool hard = engine().g_.difficulty_ == Difficulty::hard;

        if (TaggedObject::collision_) {
            speed_.x += Fixnum(-0.1) + Fixnum(0.1) * rng::choice<2>(rng::utility_state);
            speed_.x += Fixnum(-0.05) + Fixnum(0.1) * rng::choice<1>(rng::utility_state);
            TaggedObject::collision_ = false;
        }

        if (move_) {
            if (x() < engine().hero_->x() - 4) {
                speed_.x += Fixnum(0.025f);
            }
            if (x() > engine().hero_->x() + 4) {
                speed_.x -= Fixnum(0.025f);
            }
            if (y() < engine().hero()->y() - 4) {
                speed_.y += Fixnum(0.025f);
            }
            if (y() > engine().hero()->y() + 4) {
                speed_.y += Fixnum(0.025f);
            }

            if (hard) {
                shoot_ = false;
                if (speed_.x > Fixnum(3.2f / 2)) {
                    speed_.x = 0;
                    shoot_ = true;
                }
                if (speed_.x < Fixnum(-3.2f / 2)) {
                    speed_.x = 0;
                    shoot_ = true;
                }
                if (speed_.y > Fixnum(2.2f / 2)) {
                    speed_.y = 0;
                    shoot_ = true;
                }
                if (speed_.y < Fixnum(-2.2f / 2)) {
                    speed_.y = 0;
                    shoot_ = true;
                }
                if (shoot_) {
                    if (auto e = engine().add_object<EnemyShot>(position_)) {
                        auto d = rotate({1, 0}, rng::choice<360>(rng::utility_state));
                        d = d * (1.3f / 2);
                        e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                    }
                }
            } else {
                if (speed_.x > Fixnum(1.4f)) {
                    speed_.x = Fixnum(-1.5f / 2);
                }
                if (speed_.x < Fixnum(-1.4f)) {
                    speed_.x = Fixnum(-1.5f / 2);
                }
                if (speed_.y > Fixnum(1.8f / 2)) {
                    speed_.y = Fixnum(-0.5f / 2);
                }
                if (speed_.y < Fixnum(-1.8f / 2)) {
                    speed_.y = Fixnum(-0.5f / 2);
                }
            }


            Enemy::step();

            if (x() < 40) {
                x() += 160;
            } else if (x() > 160 + 40) {
                x() -= 160;
            }
            if (y() < 0) {
                y() += 160;
            } else if (y() > 160) {
                y() -= 160;
            }
        }


        switch (timeline_++) {
        case 0:
            sprite_subimage_ = 0;
            break;

        case 10:
            sprite_subimage_ = 1;
            break;

        case 20:
            sprite_subimage_ = 0;
            break;

        case 30:
            sprite_subimage_ = 1;
            break;

        case 40: {
            move_ = true;
            break;
        }
        }

        if (move_) {
            if (sprite_subimage_ == 1) {
                sprite_subimage_ = 2;
            } else {
                sprite_subimage_ = 1;
            }
        }
    }


    int collision_damage() const override
    {
        return 3;
    }


private:
    int timeline_ = 0;
};



}
