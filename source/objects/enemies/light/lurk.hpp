#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"



namespace herocore
{



class Lurk : public Enemy
{
private:

    int timeline_ = 0;
    int animcyc_ = 0;
    int y_start_ = 0;
    bool wake_ = false;
    int height_ = 0;

public:

    Lurk(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::lurk, Health{8})
    {
        position_ = pos;

        sprite_index_ = 78;

        hitbox_.dimension_.size_ = {4, 3};
        y_start_ = pos.y.as_integer();
    }


    void draw(Platform::Screen& screen) const override
    {
        Enemy::draw(screen);

        Sprite spr;
        spr.set_texture_index(70 + sprite_subimage_);
        for (int y = 0; y < 1 + height_ / 8; ++y) {
            auto img = sprite_subimage_;
            img = sprite_subimage_ + 24 - (y + 1);
            while (img > 7) {
                img -= 8;
            }
            if (img < 0) {
                img += 8;
            }
            spr.set_texture_index(70 + img);
            spr.set_position({this->x(), 8 + this->y() + y * 8});
            screen.draw(spr);
        }
    }


    virtual void damage(Health dmg, Shot& s)
    {
        Hitbox hb = hitbox_;
        hb.dimension_.size_ = {4, 5};
        if (hb.overlapping(s.hitbox())) {
            health_ = std::max(0, health_ - dmg);
        }

    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            engine().add_object<Explo>(Vec2<Fixnum>{x() - 1, y()});
            for (int y = 0; y < 1 + height_ / 8; ++y) {
                engine().add_object<Explo>(Vec2<Fixnum>{x() - 1,
                                                        this->y() + y * 8});
            }
            return;
        }

        if (wake_) {
            animcyc_ += 1;
            if (animcyc_ == 4) {
                animcyc_ = 0;
                sprite_subimage_ += 1;
                if (sprite_subimage_ == 8) {
                    sprite_subimage_ = 0;
                }

                if (y() + 16 > engine().hero()->y()
                    and place_free({x(), y() - 1})
                    and y() > 0) {
                    position_.y -= 1;
                    height_ += 1;
                } else if (y() + 16 < engine().hero()->y()
                           and place_free({x(), y() + 1})
                           and height_ > 0) {
                    position_.y += 1;
                    height_ -= 1;
                }

                hitbox_.dimension_.size_.y = height_ - 1;
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

        case 40:
            wake_ = true;
            break;

        case 119:
            timeline_ = 40;
            break;
        }
    }

};



}
