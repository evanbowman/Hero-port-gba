#include "hero.hpp"
#include "engine.hpp"



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
