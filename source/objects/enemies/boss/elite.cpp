#include "elite.hpp"
#include "objects/projectile/mimicShot.hpp"



namespace herocore
{



Elite::Elite(const Vec2<Fixnum>& pos,
             u8 spawn_x,
             u8 spawn_y) :
    Enemy(TaggedObject::Tag::ignored, Health{128}),
    spawn_x_(spawn_x),
    spawn_y_(spawn_y)
{
    position_ = {pos.x - 4, pos.y - 4};
    sprite_index_ = 60;
    hitbox_.dimension_.size_ = {6, 8};
}



void Elite::step()
{
    Enemy::step();

    if (health_ <= 0) {
        kill();
        platform().speaker().play_sound("snd_explo3", 1);
        return;
    }

    switch (timeline_++) {
    case 0:
        engine().swapsong("boss");
        sprite_index_ = 60;
        break;

    case 10:
        sprite_index_ = 61;
        break;

    case 20:
        sprite_index_ = 60;
        break;

    case 30:
        sprite_index_ = 61;
        break;

    case 40:
        sprite_index_ = 1;
        origin_ = {5, 0};

        if (x() > engine().hero()->x()) {
            hflip_ = false;
        } else {
            hflip_ = true;
        }

        break;

    case 80: {
        auto dir = rotate({1, 0}, rng::choice<8>(rng::utility_state) * 45);
        dir = dir * 1.f;
        speed_.x = Fixnum(dir.x);
        speed_.y = Fixnum(dir.y);
        dir_ = speed_;

        auto temprand = rng::choice<100>(rng::utility_state);
        if (temprand > 87) {
            speed_ = {};
        }

        if (temprand > 30) {
            auto dir = direction(fvec(position_),
                                 fvec(engine().hero()->position()));
            // FIXME!!! floor dir s.t. divisible by 45 degrees.
            speed_.x = Fixnum(dir.x);
            speed_.y = Fixnum(dir.y);
        }

        if (x() > engine().hero()->x()) {
            hflip_ = false;
        } else {
            hflip_ = true;
        }

        shotcyc_ += 1;
        if (shotcyc_ % 3 == 0) {
            if (x() > engine().hero()->x()) {
                play_sound("snd_mimicshot", 2);
                engine().add_object<MimicShot>(Vec2<Fixnum>{position_.x,
                                                            position_.y + 3},
                    Fixnum(-2.5f / 2));
            } else {
                play_sound("snd_mimicshot", 2);
                engine().add_object<MimicShot>(Vec2<Fixnum>{position_.x + 3,
                                                            position_.y + 3},
                    Fixnum(2.5f / 2));
            }
        }
        if (shotcyc_ == 32) {
            if (length(engine().enemies_) == 1) {
                speed_ = {};
                expel_ = 1;
            } else if (length(engine().enemies_) == 2) {

                Buffer<Elite*, 2> els;
                for (auto& e : engine().enemies_) {
                    if (auto el = dynamic_cast<Elite*>(e.get())) {
                        els.push_back(el);
                    }
                }

                bool assigned = false;
                for (auto& e : els) {
                    if (e->expel_) {
                        assigned = true;
                    }
                }

                if (not assigned) {
                    auto c = els[rng::choice<2>(rng::utility_state)];
                    c->expel_ = 1;
                    c->speed_ = {};
                }
            }

            shotcyc_ = 0;
        }
        break;
    }

    case 99:
        if (not expel_) {
            timeline_ = 80;
        }
        break;

    case 120:
        speed_.x = 0;
        speed_.y = -1;
        break;

    case 130:
        speed_.x = 0;
        speed_.y = 1;
        break;

    case 140:
        speed_.x = -1;
        speed_.y = 0;
        break;

    case 150:
        speed_.x = 1;
        speed_.y = 0;
        break;

    case 160:
        speed_ = {};
        for (int i = 0; i < 28; ++i) {
            if (auto e = engine().add_object<MimicShot>(Vec2<Fixnum>{
                        x(), y()
                            }, 0)) {
                auto dir = rotate({1, 0}, (Fixnum(i) * Fixnum(12.85)).as_integer());
                dir = dir * 1.5f;
                e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
            }
        }
        break;

    case 239:
        expel_ = 0;
        timeline_ = 80;
        break;
    }

    if (not place_free({x(), y()})) {

        if (not place_free({x() + 1, y()}) or not place_free({x() - 1, y()})) {
            speed_.x *= -1;
            gotit_ = 1;
        }

        if (not place_free({x(), y() + 1}) or not place_free({x(), y() - 1})) {
            speed_.y *= -1;
            gotit_ = 1;
        }

        if (not gotit_){
            speed_.x *= -1;
            speed_.y *= -1;
        }
    }

}



}
