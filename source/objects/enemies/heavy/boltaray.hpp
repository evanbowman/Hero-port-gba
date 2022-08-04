#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/projectile/vortex.hpp"



namespace herocore
{



class Boltaray : public Enemy
{
public:

    Boltaray(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::drone, Health{8})
    {
        position_ = pos;

        sprite_index_ = 48;

        hitbox_.dimension_.size_ = {12, 10};
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            engine().add_object<BigExplo>(Vec2<Fixnum>{position_.x + 6,
                                                       position_.y + 7});
            return;
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
            if (not flash_) {
                if (x() > engine().hero()->x()) {
                    sprite_subimage_ = 2;
                } else {
                    sprite_subimage_ = 3;
                }
            }
            int temprand = 0;
            if (y() > engine().hero()->y()) {
                temprand = rng::choice<3>(rng::utility_state);
            } else {
                temprand = rng::choice<3>(rng::utility_state) + 1;
            }
            if (temprand < 2) {
                move_ = 1;
            } else {
                move_ = 2;
            }
            shotcyc_ += 1;
            if (shotcyc_ == 4) {
                flash_ = true;
                if (engine().g_.difficulty_ == Difficulty::normal) {
                    freeze_ = true;
                }
            }
            if (shotcyc_ == 6) {
                flash_ = false;
                if (engine().g_.difficulty_ == Difficulty::normal) {
                    freeze_ = true;
                }

                if (sprite_subimage_ == 2) {
                    if (auto v = engine().add_object<Vortex>(position_)) {
                        v->set_left();
                    }
                } else {
                    engine().add_object<Vortex>(position_);
                }
            }
            if (shotcyc_ == 8) {
                shotcyc_ = 0;
                flash_ = false;
                freeze_ = false;
            }
            break;
        }

        case 59:
            timeline_ = 40;
            break;
        }

        if (y() < 0) {
            move_ = 2;
        }
        if (y() > 160) {
            move_ = 1;
        }

        if (not freeze_) {
            if (move_ == 1) {
                position_.y -= 1;
            }
            if (move_ == 2) {
                position_.y += 1;
            }
            if (not place_free({position_.x, position_.y + 1})) {
                move_ = 1;
            }
            if (not place_free({position_.x, position_.y - 1})) {
                move_ = 2;
            }
        }
    }


    void draw(Platform::Screen& screen) const override
    {
        if (flash_) {
            flashcyc_ += 1;
            if (flashcyc_ == 2) {
                flashcyc_ = 0;
                Sprite spr_;
                spr_.set_texture_index(52);
                if (sprite_subimage_ == 3) {
                    spr_.set_flip({true, false});
                    spr_.set_position({position_.x + 2, position_.y - 7});
                } else {
                    spr_.set_position({position_.x - 6, position_.y - 7});
                }

                screen.draw(spr_);

                spr_.set_texture_index(53);

                if (sprite_subimage_ == 3) {
                    spr_.set_position({position_.x + 2, position_.y + 9});
                } else {
                    spr_.set_position({position_.x - 6, position_.y + 9});
                }

                screen.draw(spr_);
            }

        }

        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(sprite_index_ + sprite_subimage_);
        spr_.set_position(position_);
        spr_.set_flip({hflip_, vflip_});
        screen.draw(spr_);
    }


    int collision_damage() const override
    {
        return 3;
    }


private:
    bool flash_ = false;
    bool freeze_ = false;
    int timeline_ = 0;
    int move_ = 0;
    int shotcyc_ = 0;

    // NOTE: the draw call in the gamemaker game updates flashcyc. Just doing
    // the same to mirror the original layout of the code.
    mutable int flashcyc_ = 0;
};



}
