#pragma once

#include "engine.hpp"
#include "number/random.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/enemies/heavy/phaze.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/vortex.hpp"


namespace herocore
{


class Eidolon;


class EidolonGun : public Enemy
{
private:
    Eidolon* owner_;
    int offset_;
    int firecyc_;
    bool flash_ = false;
    mutable u8 flashcyc_ = 0;

public:
    EidolonGun(Eidolon* owner, int offset)
        : Enemy(TaggedObject::Tag::ignored, Health{8}), owner_(owner), offset_(offset),
          firecyc_(offset)
    {
        sprite_index_ = 136;
    }


    inline void step() override;


    void draw(Platform::Screen& screen) const override
    {
        Enemy::draw(screen);

        if (flash_) {
            flashcyc_ += 1;
            if (flashcyc_ == 2) {
                flashcyc_ = 0;

                Sprite spr_;
                spr_.set_texture_index(52);
                if (x() < engine().hero()->x()) {
                    spr_.set_flip({true, false});
                    spr_.set_position({position_.x + 4, position_.y - 5});
                } else {
                    spr_.set_position({position_.x - 4, position_.y - 5});
                }

                screen.draw(spr_);

                spr_.set_texture_index(53);

                if (x() < engine().hero()->x()) {
                    spr_.set_position({position_.x + 4, position_.y + 11});
                } else {
                    spr_.set_position({position_.x - 4, position_.y + 11});
                }

                screen.draw(spr_);
            }
        }
    }


    Eidolon* owner() const
    {
        return owner_;
    }
};


class Eidolon : public Enemy
{
private:
    Fixnum maxspeed_;
    int timeline_ = 0;
    Vec2<int> target_;
    bool init_ = false;
    bool move_ = false;
    u8 heroup_ = 0;
    u8 dir_ = 1;

public:
    using HistoryBuffer = std::array<Vec2<s16>, 80>;

    DynamicMemory<HistoryBuffer> hist_;


    Vec2<int> target() const
    {
        return target_;
    }


    Eidolon(const Vec2<Fixnum>& pos)
        : Enemy(TaggedObject::Tag::ignored, Health{32}),
          hist_(allocate_dynamic<HistoryBuffer>(platform()))
    {
        position_ = pos;

        sprite_index_ = 130;

        hitbox_.dimension_.size_ = {24, 16};

        for (u32 i = 0; i < hist_->size(); ++i) {
            (*hist_)[i] =
                Vec2<s16>{(s16)position_.x.as_integer(), (s16)position_.y.as_integer()};
        }
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            platform().speaker().play_sound("snd_explo3", 6);
            if (engine().g_.screenshake_ < 6) {
                engine().g_.screenshake_ = 6;
            }
            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 12, position_.y + 8});

            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 12 - 7, position_.y + 8 - 4});

            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 12 - 7, position_.y + 8 + 4});

            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 12 + 7, position_.y + 8 + 4});

            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x + 12 + 7, position_.y + 8 - 4});

            for (auto& e : engine().enemies_) {
                if (auto eg = dynamic_cast<EidolonGun*>(e.get())) {
                    if (eg->owner() == this) {
                        eg->kill();
                    }
                }
            }
            return;
        }

        // const bool hard = engine().g_.difficulty_ == Difficulty::hard;

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
            if (TaggedObject::count(Tag::phaze) == 0) {
                for (auto& e : engine().enemies_) {
                    if (auto eg = dynamic_cast<EidolonGun*>(e.get())) {
                        if (eg->owner() == this) {
                            if (TaggedObject::count(Tag::phaze) < 1) {
                                engine().add_object<Phaze>(position_);
                            }
                        }
                    }
                }
            }
            if (not init_) {
                init_ = true;

                engine().add_object<EidolonGun>(this, 10);
                engine().add_object<EidolonGun>(this, 20);

                if (engine().g_.difficulty_ == Difficulty::hard) {
                    engine().add_object<EidolonGun>(this, 30);
                    engine().add_object<EidolonGun>(this, 40);
                }
            }

            if (engine().g_.difficulty_ == Difficulty::hard) {
                maxspeed_ = Fixnum(1.6f / 2);
            } else {
                maxspeed_ = Fixnum(1.4f / 2);
            }

            target_.x = engine().hero()->x().as_integer();
            target_.y = engine().hero()->y().as_integer();
            break;
        }

        case 60:
            maxspeed_ = Fixnum(1.8f / 2);
            break;

        case 109:
            timeline_ = 40;
            break;
        }

        if (move_) {
            // TODO: collision with other eidolon...
            heroup_ += 1;
            if (heroup_ == 8) {
                heroup_ = 0;
                if (x() > engine().hero()->x()) {
                    dir_ = 1;
                } else {
                    dir_ = 2;
                }
            }

            if (sprite_subimage_ == 1 or sprite_subimage_ == 2) {
                sprite_subimage_ = 0;
            } else {
                if (dir_ == 1) {
                    sprite_subimage_ = 1;
                } else {
                    sprite_subimage_ = 2;
                }
            }

            if (x() < target_.x - 4) {
                speed_.x = speed_.x + Fixnum(0.05f);
            }
            if (x() > target_.x + 4) {
                speed_.x = speed_.x - Fixnum(0.05f);
            }
            if (y() < target_.y - 4) {
                speed_.y = speed_.y + Fixnum(0.05f);
            }
            if (y() > target_.y + 4) {
                speed_.y = speed_.y - Fixnum(0.05f);
            }
            if (speed_.x > maxspeed_) {
                speed_.x = maxspeed_;
            }
            if (speed_.x < -1 * maxspeed_) {
                speed_.x = -1 * maxspeed_;
            }
            if (speed_.y > maxspeed_) {
                speed_.y = maxspeed_;
            }
            if (speed_.y < -1 * maxspeed_) {
                speed_.y = -1 * maxspeed_;
            }

            Enemy::step();

            (*hist_)[0].x = position_.x.as_integer();
            (*hist_)[0].y = position_.y.as_integer();

            for (int i = 80; i > 0; --i) {
                (*hist_)[i].x = (*hist_)[i - 1].x;
                (*hist_)[i].y = (*hist_)[i - 1].y;
            }

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
    }


    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 2);
        spr_.set_position(position_);
        spr_.set_flip({hflip_, vflip_});
        screen.draw(spr_);
        spr_.set_texture_index(sprite_index_ + sprite_subimage_ * 2 + 1);
        spr_.set_position({x() + 16, y()});
        screen.draw(spr_);
    }


    int collision_damage() const override
    {
        return 4;
    }
};


inline void EidolonGun::step()
{
    if (health_ <= 0) {
        kill();
        return;
    }

    position_.x = (*owner()->hist_)[offset_].x;
    position_.y = (*owner()->hist_)[offset_].y;

    hflip_ = x() < engine().hero_->x();

    firecyc_ += 1;
    if (firecyc_ == 80) {
        flash_ = true;
    }

    if (firecyc_ == 100) {
        firecyc_ = 0;
        flash_ = false;

        if (x() > engine().hero()->x()) {
            if (auto v = engine().add_object<Vortex>(Vec2<Fixnum>{x(), y() + 4})) {
                v->set_left();
            }
        } else {
            engine().add_object<Vortex>(Vec2<Fixnum>{x() - 4, y() + 4});
        }
    }
}


} // namespace herocore
