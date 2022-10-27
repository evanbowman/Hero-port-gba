#pragma once

#include "engine.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/supershot.hpp"


namespace herocore
{


class Turret : public Enemy
{
private:
    int shotcyc_ = 0;
    bool left_ = false;

public:
    Turret(const Vec2<Fixnum>& pos) : Enemy(TaggedObject::Tag::spew, Health{8})
    {
        position_ = pos;

        sprite_index_ = 37;
        sprite_subimage_ = 0;

        hitbox_.dimension_.size_ = {8, 16};
    }


    void set_left()
    {
        left_ = true;
        hflip_ = true;
        origin_.x = 16;
        hitbox_.dimension_.origin_.x = 8;
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
            platform().speaker().play_sound("snd_explo1", 6);
            int dx = -4;
            if (left_) {
                dx = 4;
            }
            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x - dx, position_.y + 0});
            engine().add_object<BigExplo>(
                Vec2<Fixnum>{position_.x - dx, position_.y + 16});
            ;
            return;
        }

        switch (timeline_++) {
        case 0:
            sprite_subimage_ = 0;
            break;

        case 10:
            sprite_subimage_ = 5;
            break;

        case 20:
            sprite_subimage_ = 0;
            break;

        case 30:
            sprite_subimage_ = 5;
            break;

        case 40:
            shotcyc_ += 1;
            if (engine().g_.difficulty_ == Difficulty::hard) {
                if (shotcyc_ == 1) {
                    auto origin = fvec(position_);
                    origin.x += 0;
                    origin.y += 4;
                    if (left_) {
                        origin.x -= 7;
                    }
                    Vec2<Fixnum> fo;
                    fo.x = Fixnum(origin.x);
                    fo.y = Fixnum(origin.y);
                    auto em = engine().add_object<Megashot>(fo);
                    auto s = engine().add_object<EnemyShot>(fo);
                    auto s2 = engine().add_object<EnemyShot>(fo);
                    if (not s or not em or not s2) {
                        break;
                    }
                    auto h = engine().hero()->position();
                    if (not left_ and h.x < position_.x) {
                        if (h.y < position_.y) {
                            s->set_speed({0, -1});
                        } else {
                            s->set_speed({0, 1});
                        }
                    } else if (left_ and h.x > position_.x) {
                        if (h.y < position_.y) {
                            s->set_speed({0, -1});
                        } else {
                            s->set_speed({0, 1});
                        }
                    } else {
                        auto dir = direction(origin, fvec(engine().hero()->position()));
                        auto d = (1.8f / 2) * rotate(dir, 30);
                        auto d2 = (1.8f / 2) * rotate(dir, 330);
                        Vec2<Fixnum> fv;
                        fv.x = Fixnum(dir.x);
                        fv.y = Fixnum(dir.y);
                        em->set_speed(fv);
                        fv.x = Fixnum(d.x);
                        fv.y = Fixnum(d.y);
                        s->set_speed(fv);
                        fv.x = Fixnum(d2.x);
                        fv.y = Fixnum(d2.y);
                        s2->set_speed(fv);
                    }
                } else {
                    shotcyc_ = 0;
                    auto origin = fvec(position_);
                    origin.x += 0;
                    origin.y += 6;
                    if (left_) {
                        origin.x -= 7;
                    }
                    Vec2<Fixnum> fo;
                    fo.x = Fixnum(origin.x);
                    fo.y = Fixnum(origin.y);
                    auto s = engine().add_object<Supershot>(fo);
                    auto s2 = engine().add_object<Supershot>(fo);
                    if (not s or not s2) {
                        break;
                    }
                    auto h = engine().hero()->position();
                    if (not left_ and h.x < position_.x) {
                        if (h.y < position_.y) {
                            s->set_speed({0, -1});
                        } else {
                            s->set_speed({0, 1});
                        }
                    } else if (left_ and h.x > position_.x) {
                        if (h.y < position_.y) {
                            s->set_speed({0, -1});
                        } else {
                            s->set_speed({0, 1});
                        }
                    } else {
                        auto dir = direction(origin, fvec(engine().hero()->position()));
                        auto d = rotate(dir, 30);
                        auto d2 = rotate(dir, 330);
                        Vec2<Fixnum> fv;
                        fv.x = Fixnum(d.x);
                        fv.y = Fixnum(d.y);
                        s->set_speed(fv);
                        fv.x = Fixnum(d2.x);
                        fv.y = Fixnum(d2.y);
                        s2->set_speed(fv);
                    }
                }
            } else {
                if (shotcyc_ == 1) {
                    auto origin = fvec(position_);
                    origin.x += 0;
                    origin.y += 4;
                    if (left_) {
                        origin.x -= 4;
                    }
                    Vec2<Fixnum> fo;
                    fo.x = Fixnum(origin.x);
                    fo.y = Fixnum(origin.y);
                    auto s = engine().add_object<Supershot>(fo);
                    if (not s) {
                        break;
                    }
                    auto h = engine().hero()->position();
                    if (not left_ and h.x < position_.x) {
                        if (h.y < position_.y) {
                            s->set_speed({0, -1});
                        } else {
                            s->set_speed({0, 1});
                        }
                    } else if (left_ and h.x > position_.x) {
                        if (h.y < position_.y) {
                            s->set_speed({0, -1});
                        } else {
                            s->set_speed({0, 1});
                        }
                    } else {
                        auto dir = direction(origin, fvec(engine().hero()->position()));
                        Vec2<Fixnum> fv;
                        fv.x = Fixnum(dir.x);
                        fv.y = Fixnum(dir.y);
                        s->set_speed(fv);
                    }
                } else {
                    shotcyc_ = 0;
                    auto origin = fvec(position_);
                    origin.x += 0;
                    origin.y += 6;
                    if (left_) {
                        origin.x -= 4;
                    }
                    Vec2<Fixnum> fo;
                    fo.x = Fixnum(origin.x);
                    fo.y = Fixnum(origin.y);
                    auto s = engine().add_object<EnemyShot>(fo);
                    auto s2 = engine().add_object<EnemyShot>(fo);
                    if (not s or not s2) {
                        break;
                    }
                    auto h = engine().hero()->position();
                    if (not left_ and h.x < position_.x) {
                        if (h.y < position_.y) {
                            s->set_speed({0, -1});
                        } else {
                            s->set_speed({0, 1});
                        }
                    } else if (left_ and h.x > position_.x) {
                        if (h.y < position_.y) {
                            s->set_speed({0, -1});
                        } else {
                            s->set_speed({0, 1});
                        }
                    } else {
                        auto dir = direction(origin, fvec(engine().hero()->position()));
                        auto d = rotate(dir, 30);
                        auto d2 = rotate(dir, 330);
                        Vec2<Fixnum> fv;
                        fv.x = Fixnum(d.x);
                        fv.y = Fixnum(d.y);
                        s->set_speed(fv);
                        fv.x = Fixnum(d2.x);
                        fv.y = Fixnum(d2.y);
                        s2->set_speed(fv);
                    }
                }
            }
            if (left_) {
                position_.x += 1;
            } else {
                position_.x -= 1;
            }
            break;

        case 44:
            if (left_) {
                position_.x -= 1;
            } else {
                position_.x += 1;
            }
            break;


        case 41:
        case 50:
        case 60:
        case 70: {
            auto origin = fvec(position_);
            origin.x += 8;
            origin.y += 16;
            auto dir = direction(origin, fvec(engine().hero()->position()));
            if (left_) {
                hflip_ = true;
            }
            // FIXME! Write a function to compute angles in degrees.
            if (dir.y < -0.95f) {
                sprite_subimage_ = 1;
                vflip_ = false;
            } else if (dir.y < -0.7f) {
                sprite_subimage_ = 2;
                vflip_ = false;
            } else if (dir.y < -0.5f) {
                sprite_subimage_ = 3;
                vflip_ = false;
            } else if (dir.y < -0.2f) {
                sprite_subimage_ = 4;
                vflip_ = false;
            } else if (dir.y > 0.95f) {
                sprite_subimage_ = 1;
                vflip_ = true;
            } else if (dir.y > 0.7f) {
                sprite_subimage_ = 2;
                vflip_ = true;
            } else if (dir.y > 0.5f) {
                sprite_subimage_ = 3;
                vflip_ = true;
            } else if (dir.y > 0.2f) {
                sprite_subimage_ = 4;
                vflip_ = true;
            } else {
                sprite_subimage_ = 5;
                vflip_ = false;
            }
            break;
        }

        case 79:
            timeline_ = 40;
            break;
        }
    }


private:
    int timeline_ = 0;
};


} // namespace herocore
