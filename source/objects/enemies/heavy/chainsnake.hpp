#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "number/random.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/particles/bigExplo.hpp"
#include "bulkAllocator.hpp"



namespace herocore
{



class Chainsnake;



class ChainsnakeNeck : public Enemy
{
private:
    Chainsnake* owner_;
    int offset_;

public:

    ChainsnakeNeck(Chainsnake* owner, int offset) :
        Enemy(Tag::chainsnake_neck, Health{99}),
        owner_(owner), offset_(offset)
    {
        sprite_index_ = 141;
        origin_ = {3, 3};
        hitbox_.dimension_.origin_ = {3, 3};
        hitbox_.dimension_.size_ = {6, 6};
    }


    inline void step() override;


    Chainsnake* owner() const
    {
        return owner_;
    }

};



class Chainsnake : public Enemy
{
private:
    int timeline_ = 0;

    using HistoryBuffer = std::array<Vec2<s16>, 80>;

    bool neckinit_ = false;
    bool firsthome_ = false;

    int prevdir_ = 0;
    int dir_ = 0;
    int shotcyc_ = 0;

public:

    DynamicMemory<HistoryBuffer> hist_;


    Chainsnake(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::chainsnake_neck, Health{8}),
        hist_(allocate_dynamic<HistoryBuffer>(platform()))
    {
        position_ = pos;

        sprite_index_ = 137;

        hitbox_.dimension_.size_ = {10, 8};
        hitbox_.dimension_.origin_ = {5, 4};
        origin_ = {5, 4};
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            for (auto& e : engine().enemies_) {
                if (auto eg = dynamic_cast<ChainsnakeNeck*>(e.get())) {
                    if (eg->owner() == this) {
                        eg->kill();
                    }
                }
            }
            engine().add_object<BigExplo>(Vec2<Fixnum>{
                    position_.x, position_.y});
            return;
        }

        auto speed = Fixnum(1.3f / 2);

        const bool hard = engine().g_.difficulty_ == Difficulty::hard;
        if (hard) {
            speed = Fixnum(1.5f / 2);
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

            if (hard) {
                shotcyc_ += 1;
                if (shotcyc_ == 4) {
                    for (int i = 0; i < 4; ++i) {
                        if (auto e = engine().add_object<EnemyShot>(Vec2<Fixnum>{
                                    x() - 2, y() - 2
                                })) {
                            auto d = rotate({1, 0}, 45 + i * 90);
                            e->set_speed({Fixnum(d.x), Fixnum(d.y)});
                        }
                    }
                    shotcyc_ = 0;
                }
            }
            if (not neckinit_) {
                neckinit_ = true;
                for (int i = 5; i > 0; --i) {
                    int offset = i * 12;
                    engine().add_object<ChainsnakeNeck>(this, offset);
                }
            }
            {
                prevdir_ = dir_;

                int temprand = rng::choice<12>(rng::utility_state);
                if (not firsthome_) {
                    temprand = 5;
                }
                if (temprand < 4) {
                    dir_ = 90 * temprand;
                } else {
                    auto xd = x() - engine().hero()->x();
                    auto yd = y() - engine().hero()->y();

                    if (abs(xd.as_integer()) > abs(yd.as_integer())) {
                        if (xd < 0) {
                            dir_ = 0;
                        } else {
                            dir_ = 180;
                        }
                    } else {
                        if (yd < 0) {
                            dir_ = 270;
                        } else {
                            dir_ = 90;
                        }
                    }
                }
                if (not firsthome_) {
                    firsthome_ = true;
                    prevdir_ = dir_;
                }
                if ((dir_ == 0 and prevdir_ == 180) or
                    (dir_ == 180 and prevdir_ == 0) or
                    (dir_ == 90 and prevdir_ == 270) or
                    (dir_ == 270 and prevdir_ == 90)) {
                    dir_ = prevdir_;
                }
                auto d = rotate({1, 0}, dir_);
                speed_.x = speed * Fixnum(d.x);
                speed_.y = speed * Fixnum(d.y);
            }

            break;
        }

        case 59:
            timeline_ = 40;
            break;
        }

        if (neckinit_) {
            bool collision = false;
            if (dir_ == 0 or dir_ == 180) {
                if (not place_free({x(), y() + speed_.y}) and
                    not place_free({x(), y() - speed_.y})) {
                    dir_ += 180;
                    collision = true;
                } else if (not place_free({x(), y() - speed_.y})) {
                    dir_ = 270;
                    collision = true;
                } else if (not place_free({x(), y() + speed_.y})) {
                    dir_ = 90;
                    collision = true;
                }
            } else {
                if (not place_free({x() - speed_.x, y()}) and
                    not place_free({x() + speed_.x, y()})) {
                    dir_ += 180;
                    collision = true;
                } else if (not place_free({x() - speed_.x, y()})) {
                    dir_ = 0;
                    collision = true;
                } else if (not place_free({x() + speed_.x, y()})) {
                    dir_ = 180;
                    collision = true;
                }
            }

            if (dir_ >= 360) {
                dir_ -= 360;
            }

            if (collision) {
                auto d = rotate({1, 0}, dir_);
                speed_.x = speed * Fixnum(d.x);
                speed_.y = speed * Fixnum(d.y);
            }


            if (dir_ == 90 or dir_ == 270) {
                if (x() < engine().hero()->x()) {
                    sprite_subimage_ = 2;
                } else {
                    sprite_subimage_ = 3;
                }
            } else if (dir_ == 180) {
                sprite_subimage_ = 2;
            } else if (dir_ == 0) {
                sprite_subimage_ = 3;
            }
        }

        Enemy::step();

        (*hist_)[0].x = position_.x.as_integer();
        (*hist_)[0].y = position_.y.as_integer();

        for (int i = 80; i > 0; --i) {
            (*hist_)[i].x = (*hist_)[i - 1].x;
            (*hist_)[i].y = (*hist_)[i - 1].y;
        }
    }


    int collision_damage() const override
    {
        return 3;
    }
};



inline void ChainsnakeNeck::step()
{
    position_.x = (*owner()->hist_)[offset_].x;
    position_.y = (*owner()->hist_)[offset_].y;
}



}
