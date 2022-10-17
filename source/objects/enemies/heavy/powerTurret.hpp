#pragma once

#include "objects/enemies/enemy.hpp"
#include "objects/projectile/enemyShot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/megashot.hpp"
#include "engine.hpp"
#include "objects/particles/bigExplo.hpp"



namespace herocore
{



class PowerTurret : public Enemy
{
private:
    int shotcyc_ = 0;
    bool left_ = false;

public:

    PowerTurret(const Vec2<Fixnum>& pos) :
        Enemy(TaggedObject::Tag::spew, Health{16})
    {
        position_ = pos;

        sprite_index_ = 64;
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



    int collision_damage() const override
    {
        return 3;
    }



    void step() override
    {
        if (health_ == 0) {
            kill();
            platform().speaker().play_sound("snd_explo2", 1);
            int dx = -4;
            if (left_) {
                dx = 4;
            }
            engine().add_object<BigExplo>(Vec2<Fixnum>{position_.x - dx, position_.y + 0});
            engine().add_object<BigExplo>(Vec2<Fixnum>{position_.x - dx, position_.y + 16});
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
                        origin.x -= 6;
                    }
                    Vec2<Fixnum> fo;
                    fo.x = Fixnum(origin.x);
                    fo.y = Fixnum(origin.y);
                    auto s = engine().add_object<Supershot>(fo);
                    auto s1 = engine().add_object<Supershot>(fo);
                    auto s2 = engine().add_object<Megashot>(fo);
                    if (not s or not s1 or not s2) {
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
                        s->set_speed(fv * Fixnum(1.4f / 2));
                        s1->set_speed(fv * Fixnum(1.8f / 2));
                        s2->set_speed(fv * Fixnum(2.2f / 2));
                    }
                } else {
                    shotcyc_ = 0;
                    auto origin = fvec(position_);
                    origin.x += 0;
                    origin.y += 6;
                    if (left_) {
                        origin.x -= 6;
                    }
                    Vec2<Fixnum> fo;
                    fo.x = Fixnum(origin.x);
                    fo.y = Fixnum(origin.y);
                    auto s = engine().add_object<Supershot>(fo);
                    auto s2 = engine().add_object<Supershot>(fo);
                    auto s3 = engine().add_object<Supershot>(fo);
                    auto s4 = engine().add_object<Supershot>(fo);
                    auto s5 = engine().add_object<Supershot>(fo);
                    if (not s or not s2 or not s3 or not s4) {
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
                        auto d = rotate(dir, 15);
                        auto d2 = rotate(dir, 345);
                        auto d3 = rotate(dir, 40);
                        auto d4 = rotate(dir, 320);
                        Vec2<Fixnum> fv;
                        fv.x = Fixnum(d.x);
                        fv.y = Fixnum(d.y);
                        s->set_speed(fv);
                        fv.x = Fixnum(d2.x);
                        fv.y = Fixnum(d2.y);
                        s2->set_speed(fv);
                        fv.x = Fixnum(d3.x);
                        fv.y = Fixnum(d3.y);
                        s3->set_speed(fv);
                        fv.x = Fixnum(d4.x);
                        fv.y = Fixnum(d4.y);
                        s4->set_speed(fv);
                        fv.x = Fixnum(dir.x);
                        fv.y = Fixnum(dir.y);
                        s5->set_speed(fv);
                    }
                }
            } else {
                if (shotcyc_ == 1) {
                    auto origin = fvec(position_);
                    origin.x += 0;
                    origin.y += 4;
                    if (left_) {
                        origin.x -= 6;
                    }
                    Vec2<Fixnum> fo;
                    fo.x = Fixnum(origin.x);
                    fo.y = Fixnum(origin.y);
                    auto s = engine().add_object<Megashot>(fo);
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
                        origin.x -= 6;
                    }
                    Vec2<Fixnum> fo;
                    fo.x = Fixnum(origin.x);
                    fo.y = Fixnum(origin.y);
                    auto s = engine().add_object<Supershot>(fo);
                    auto s2 = engine().add_object<Supershot>(fo);
                    auto s3 = engine().add_object<Supershot>(fo);
                    auto s4 = engine().add_object<Supershot>(fo);
                    if (not s or not s2 or not s3 or not s4) {
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
                        auto d = rotate(dir, 15);
                        auto d2 = rotate(dir, 345);
                        auto d3 = rotate(dir, 40);
                        auto d4 = rotate(dir, 320);
                        Vec2<Fixnum> fv;
                        fv.x = Fixnum(d.x);
                        fv.y = Fixnum(d.y);
                        s->set_speed(fv);
                        fv.x = Fixnum(d2.x);
                        fv.y = Fixnum(d2.y);
                        s2->set_speed(fv);
                        fv.x = Fixnum(d3.x);
                        fv.y = Fixnum(d3.y);
                        s3->set_speed(fv);
                        fv.x = Fixnum(d4.x);
                        fv.y = Fixnum(d4.y);
                        s4->set_speed(fv);
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



}
