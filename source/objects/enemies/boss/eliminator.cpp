#include "eliminator.hpp"
#include "objects/particles/flame.hpp"



namespace herocore
{



Eliminator::Eliminator(const Vec2<Fixnum>& pos) :
    Enemy(TaggedObject::Tag::ignored, Health{64})
{
    position_ = pos;
    position_.x -= 2;
    position_.y -= 2;
    sprite_index_ = 188;
    origin_ = {5, 0};
    hitbox_.dimension_.size_ = {6, 8};
}



void Eliminator::step()
{
    if (health_ == 0) {
        kill();
        engine().p_->eliminator_defeated_ = true;
        platform().speaker().play_sound("snd_explo3", 14);
        engine().add_object<ExploSpewer>(Vec2<Fixnum>{x() + 2, y() + 4});
    }

    switch (timeline_++) {
    case 0:
        engine().swapsong("boss");
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
        sprite_subimage_ = 2;
        if (x() > engine().hero()->x()) {
            hflip_ = false;
        } else {
            hflip_ = true;
        }
        break;

    case 80:
        if (health_ > 0 and not escape_) {
            move_ = true;
            shotcyc_ += 1;
            if (shotcyc_ == 3) {
                shotcyc_ = 0;
                rfire_ = 4;
                max_speed_ = Fixnum(2.5f / 2);
                target_x_ = 5 * 8 + 25 + rng::choice<110>(rng::critical_state);
                target_y_ = 25 + rng::choice<110>(rng::critical_state);
            }

            escapecyc_ += 1;
            if (engine().p_->level_ < 9 and
                escapecyc_ == 50 - (int)engine().g_.difficulty_ * 10 +
                engine().p_->level_) {
                escape_ = true;
                target_x_ = 250;
                target_y_ = 250;
            }
        }
        break;

    case 99:
        timeline_ = 80;
        break;
    }

    if (y() > 240) {
        kill();
    }

    if (move_) {
        heroup_ += 1;
        if (heroup_ == 8) {
            heroup_ = 0;
            if (x() > engine().hero()->x()) {
                hflip_ = false;
            } else {
                hflip_ = true;
            }
        }
        if (x() < target_x_ - 4) {
            speed_.x += Fixnum(0.05f);
        }
        if (x() > target_x_ + 4) {
            speed_.x -= Fixnum(0.05f);
        }
        if (y() < target_y_ - 4) {
            speed_.y += Fixnum(0.05f);
        }
        if (y() > target_y_ + 4) {
            speed_.y -= Fixnum(0.05f);
        }
        speed_.x = clamp(speed_.x, -1 * max_speed_, max_speed_);
        speed_.y = clamp(speed_.y, -1 * max_speed_, max_speed_);
    }

    if (rfire_ > 0 and health_ > 0) {
        rfirecyc_ += 1;
        if (rfirecyc_ == 5) {
            rfirecyc_ = 0;
            rfire_ -= 1;
            if (rfire_ == 3) {
                int xo = -2;
                if (auto e = engine().add_object<Supershot>(Vec2<Fixnum>{x() + xo, y() - 1})) {
                    auto dir = direction(fvec(position_),
                                         fvec(engine().hero()->position()));
                    dir = dir * ((1.5f / 2) + rfire_ * 0.15f);
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            } else {
                int xo = -1;
                if (auto e = engine().add_object<EnemyShot>(Vec2<Fixnum>{x() + xo, y() - 1})) {
                    auto dir = direction(fvec(position_),
                                         fvec(engine().hero()->position()));
                    dir = dir * ((1.f / 2) + rfire_ * 0.15f);
                    e->set_speed({Fixnum(dir.x), Fixnum(dir.y)});
                }
            }
        }
    }

    if (timeline_ > 40 and
        --flamecyc_ == 0) {
        flamecyc_ = 1 + rng::choice<3>(rng::critical_state);
        int xo = 1;
        int yo = 2;
        if (hflip_) {
            xo = 4;
        }
        if (auto f = engine().add_object<Flame>(Vec2<Fixnum>{position_.x + xo,
                                                             position_.y + yo})) {
            f->priority_ = 0;
            f->counter_ = 16;
        }

    }

    Enemy::step();
}



Eliminator::~Eliminator()
{
    engine().prevsong();
}



void Eliminator::draw(Platform::Screen& screen) const
{
    if (timeline_ % 2 == 0) {
        Enemy::draw(screen);
    }
}



}
