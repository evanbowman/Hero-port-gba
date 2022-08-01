#include "hero.hpp"
#include "engine.hpp"
#include "objects/particles/flame.hpp"



namespace herocore
{



void Hero::step()
{
    if (key_pressed<Key::action_1>() and key_pressed<Key::action_2>()) {

    } else if (key_down<Key::action_1>()) {
        hflip_ = false;
    } else if (key_down<Key::action_2>()) {
        hflip_ = true;
    }

    auto x_prev = position_.x;
    auto y_prev = position_.y;

    if (key_pressed<Key::left>()) {
        if (place_free({position_.x - flyspeed_, position_.y})) {
            position_.x -= flyspeed_;
        }
    } else if (key_pressed<Key::right>()) {
        if (place_free({position_.x + flyspeed_, position_.y})) {
            position_.x += flyspeed_;
        }
    }
    if (key_pressed<Key::up>()) {
        if (place_free({position_.x, position_.y - flyspeed_})) {
            position_.y -= flyspeed_;
        }

    } else if (key_pressed<Key::down>()) {
        if (place_free({position_.x, position_.y + flyspeed_})) {
            position_.y += flyspeed_;
        }
    }

    if (position_.x not_eq x_prev or position_.y not_eq y_prev) {
        jetpack_counter_ += 1;
        if (jetpack_counter_ == 6) {
            jetpack_counter_ = 0;
            if (hflip_ == false) {
                engine().add_object<Flame>(Vec2<Fixnum>{position_.x + 4, position_.y + 4});
            } else {
                engine().add_object<Flame>(Vec2<Fixnum>{position_.x, position_.y + 4});
            }
        }
    }

    if (engine().autofire_) {
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
}



void Hero::fireleft()
{

}



void Hero::fireright()
{

}



}
