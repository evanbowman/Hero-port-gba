#pragma once

#include "objects/enemies/enemy.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"



namespace herocore
{



class Spew : public Enemy
{
private:
    Vec2<Fixnum> direction_;
    Fixnum speed_;
    Fixnum max_speed_;
    int shotcyc_ = 0;

public:

    Spew(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::spew, Health{1})
    {
        position_ = pos;

        sprite_index_ = 11;
        sprite_subimage_ = 0;

        hitbox_.dimension_.size_ = {8, 8};
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            engine().add_object<Explo>(position_);
            platform().speaker().play_sound("snd_explo1", 6);
            return;
        }

        move(position_ + direction_ * speed_);

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
            if (position_.x > engine().hero()->position().x) {
                hflip_ = false;
            } else {
                hflip_ = true;
            }

            sprite_subimage_ = 2;
            origin_.x = 4;

            if (engine().g_.hp_ > 0) {
                auto dir = direction(fvec(position_),
                                     fvec(engine().hero()->position()));

                speed_ = Fixnum(1.2f / 2);

                if (engine().g_.difficulty_ == Difficulty::hard) {

                    // auto xd = position_.x - engine().hero()->position().x;
                    // auto yd = position_.y - engine().hero()->position().y;
                    // Diagonal movement. FIXME!!!
                    if (dir.x > 0 and dir.y > 0) {
                        dir = rotate({1, 0}, 45);
                    } else if (dir.x < 0 and dir.y > 0) {
                        dir = rotate({1, 0}, 45 + 90);
                    } else if (dir.x < 0 and dir.y < 0) {
                        dir = rotate({1, 0}, 45 + 180);
                    } else {
                        dir = rotate({1, 0}, 45 + 270);
                    }

                    shotcyc_ += 1;
                    if (shotcyc_ == 2) {
                        shotcyc_ = 0;
                        auto s = engine().add_object<Supershot>(position_);
                        if (s) {

                            auto dir = direction(fvec(position_),
                                                 fvec(engine().hero()->position()));

                            // FIXME:
                            if (dir.x > 0 and abs(dir.y) < 0.3f) {
                                dir.x = 1.f;
                                dir.y = 0.f;
                            } else {
                                if (dir.x > 0 and dir.y > 0) {
                                    dir = rotate({1, 0}, 45);
                                } else if (dir.x < 0 and dir.y > 0) {
                                    dir = rotate({1, 0}, 45 + 90);
                                } else if (dir.x < 0 and dir.y < 0) {
                                    dir = rotate({1, 0}, 45 + 180);
                                } else {
                                    dir = rotate({1, 0}, 45 + 270);
                                }
                            }

                            if (engine().g_.difficulty_ == Difficulty::hard) {
                                dir = dir * (2.5f / 2);
                            }

                            Vec2<Fixnum> speed;
                            speed.x = Fixnum(dir.x);
                            speed.y = Fixnum(dir.y);
                            s->set_speed(speed);
                        }
                    }
                } else {
                    auto temprand = rng::choice<8>(rng::utility_state);
                    if (temprand < 4) {
                        dir = rotate({1.f, 0.f}, temprand * 90);
                    } else {
                        auto xd = position_.x - engine().hero()->position().x;
                        auto yd = position_.y - engine().hero()->position().y;
                        // Orthogonal movement.
                        if (abs(xd) > abs(yd)) {
                            if (xd < 0) {
                                dir = {1, 0};
                            } else {
                                dir = {-1, 0};
                            }
                        } else {
                            if (yd < 0) {
                                dir = {0, 1};
                            } else {
                                dir = {0, -1};
                            }
                        }
                    }


                    shotcyc_ += 1;
                    if (shotcyc_ == 2) {
                        shotcyc_ = 0;

                        auto s = engine().add_object<EnemyShot>(position_);
                        if (s) {

                            auto dir2 = direction(fvec(position_),
                                                  fvec(engine().hero()->position()));

                            // FIXME:
                            if (dir2.x > 0 and abs(dir2.y) < 0.3f) {
                                dir2.x = 1.f;
                                dir2.y = 0.f;
                            } else {
                                if (dir2.x > 0 and dir2.y > 0) {
                                    dir2 = rotate({1, 0}, 45);
                                } else if (dir2.x < 0 and dir2.y > 0) {
                                    dir2 = rotate({1, 0}, 45 + 90);
                                } else if (dir2.x < 0 and dir2.y < 0) {
                                    dir2 = rotate({1, 0}, 45 + 180);
                                } else {
                                    dir2 = rotate({1, 0}, 45 + 270);
                                }
                            }

                            Vec2<Fixnum> speed;
                            speed.x = Fixnum(dir2.x);
                            speed.y = Fixnum(dir2.y);
                            s->set_speed(speed);
                        }
                    }
                }

                direction_.x = Fixnum(dir.x);
                direction_.y = Fixnum(dir.y);
            }
            break;

        case 79:
            timeline_ = 40;
            break;
        }

        auto x = position_.x;
        auto y = position_.y;
        if (not place_free({position_.x + (direction_ * speed_).x,
                            position_.y + (direction_ * speed_).y})) {
            if (not place_free({x + 2, y}) or not place_free({x - 2, y})) {
                direction_.x = direction_.x * -1;
            }
            if (not place_free({x, y + 2}) or not place_free({x, y - 2})) {
                direction_.y = direction_.y * -1;
            }
        }
    }


private:

    int timeline_ = 0;

};



}
