#include "hero.hpp"
#include "engine.hpp"
#include "objects/particles/flame.hpp"
#include "objects/projectile/shot.hpp"



namespace herocore
{



void Hero::step()
{
    if (engine().paused_) {
        return;
    }

    if (engine().g_.hp_ == 0) {
        return;
    }


    if (engine().g_.heat_) {
        switch (engine().g_.suit_) {
        case 0:
            engine().g_.heat_ -= 1;
            break;

        case 1:
            engine().g_.heat_ -= 2;
            break;

        default:
            engine().g_.heat_ = 0;
            break;
        }
        if (engine().g_.heat_ < 0) {
            engine().g_.heat_ = 0;
        }
        engine().draw_hud_heat();
    }


    if (key_down<Key::action_2>()) {
        hflip_ = false;

        fireleft();

    }
    if (key_down<Key::action_1>()) {
        hflip_ = true;

        fireright();

    }

    if (engine().g_.invulnerable_ > 0) {
        engine().g_.invulnerable_ -= 1;
    }


    auto x_prev = position_.x;
    auto y_prev = position_.y;

    if (key_pressed<Key::left>()) {
        if (place_free({position_.x - flyspeed_, position_.y})) {
            position_.x -= flyspeed_;

            if (not place_free(position_)) {
                position_.x = x_prev;
            }
        }
    } else if (key_pressed<Key::right>()) {
        if (place_free({position_.x + flyspeed_, position_.y})) {
            position_.x += flyspeed_;

            if (not place_free(position_)) {
                position_.x = x_prev;
            }
        }
    }
    if (key_pressed<Key::up>()) {
        if (place_free({position_.x, position_.y - flyspeed_})) {
            position_.y -= flyspeed_;

            if (not place_free(position_)) {
                position_.y = y_prev;
            }
        }

    } else if (key_pressed<Key::down>()) {
        if (place_free({position_.x, position_.y + flyspeed_})) {
            position_.y += flyspeed_;

            if (not place_free(position_)) {
                position_.y = y_prev;
            }
        }
    }

    if ((position_.x not_eq x_prev or position_.y not_eq y_prev) and
        engine().g_.hero_jetpack_flames_) {

        jetpack_counter_ += 1;
        if (jetpack_counter_ == 6) {
            jetpack_counter_ = 0;
            if (hflip_ == false) {
                engine().add_object<Flame>(Vec2<Fixnum>{position_.x + 5,
                                                        position_.y + 5});
            } else {
                engine().add_object<Flame>(Vec2<Fixnum>{position_.x + 0,
                                                        position_.y + 5});
            }
        }
    }

    if (engine().g_.autofire_) {
        autofire_index_ += 1;
        if (autofire_index_ >= 4) {
            autofire_index_ = 0;

            if (not hflip_) {
                fireleft();
            } else {
                fireright();
            }
        }
    }

    if (engine().g_.heat_) {
        heatcyc_ += 1;
        if (heatcyc_ == 6) {
            heatcyc_ = 0;
        }
    }
}



void Hero::draw(Platform::Screen& screen) const
{
    if (engine().g_.hp_ == 0) {
        return;
    }

    if (engine().g_.invulnerable_ > 0 and
        engine().g_.invulnerable_ % 4 < 2) {
        Object::draw(screen);
    } if (not engine().g_.invulnerable_) {
        Object::draw(screen);
        if (engine().g_.heat_) {
            if (heatcyc_ > 3) {
                Sprite spr_;
                spr_.set_origin({origin_.x, origin_.y});
                spr_.set_texture_index(sprite_index_ + sprite_subimage_);
                spr_.set_flip({hflip_, vflip_});
                spr_.set_position({x() - 1, y()});
                screen.draw(spr_);
                spr_.set_position({x() + 1, y()});
                screen.draw(spr_);
                spr_.set_position({x(), y() - 1});
                screen.draw(spr_);
                spr_.set_position({x(), y() + 1});
                screen.draw(spr_);
            }
        }
    }
}



void Hero::fireleft()
{
    if (engine().g_.heat_) {
        return;
    }
    if (engine().g_.shot_count_ < 6) {
        if (not platform().speaker().is_sound_playing("snd_fireshot")) {
            platform().speaker().play_sound("snd_fireshot", 1);
        }
        engine().add_object<Shot>(Vec2<Fixnum>{position_.x, position_.y + 3},
                                  Fixnum(-2),
                                  1);
    }
}



void Hero::fireright()
{
    if (engine().g_.heat_) {
        return;
    }
    if (engine().g_.shot_count_ < 6) {
        if (not platform().speaker().is_sound_playing("snd_fireshot")) {
            platform().speaker().play_sound("snd_fireshot", 1);
        }
        engine().add_object<Shot>(Vec2<Fixnum>{position_.x + 3, position_.y + 3},
                                  Fixnum(2),
                                  1);
    }
}



u8& Hero::blaster_level() const
{
    return engine().p_->blaster_;
}



u8& Hero::blade_level() const
{
    return engine().p_->blade_;
}



u8& Hero::suit_level() const
{
    return engine().p_->suit_;
}



u8& Hero::level() const
{
    return engine().p_->level_;
}



}
