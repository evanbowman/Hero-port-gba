#include "hero.hpp"
#include "engine.hpp"
#include "objects/particles/flame.hpp"
#include "objects/projectile/enemyProjectile.hpp"
#include "objects/projectile/gigashot.hpp"
#include "objects/projectile/megashot.hpp"
#include "objects/projectile/missile.hpp"
#include "objects/projectile/shot.hpp"
#include "objects/projectile/supershot.hpp"
#include "objects/projectile/vortex.hpp"
#include "objects/projectile/laser.hpp"



namespace herocore
{


class Blade : public Object
{
public:
    Blade(const Vec2<Fixnum>& pos, bool flip)
    {
        position_ = pos;

        sprite_index_ = 194;
        origin_ = {8, 8};
        hitbox_.dimension_.size_ = {8, 8};
        hitbox_.dimension_.origin_ = {4, 4};

        hflip_ = flip;

        if (flip) {
            position_.x += 9;
        } else {
            position_.x -= 3;
        }

        position_.y += 4;

        if (engine().p_->blade_ > 2) {
            sprite_index_ = 195;
            hitbox_.dimension_.size_ = {10, 16};
            hitbox_.dimension_.origin_ = {8, 8};
        }
    }


    bool test_wall_collision()
    {
        auto pos = position_;

        auto intx = pos.x.as_integer();
        auto inty = pos.y.as_integer();

        // Onscreen area starts at 0, 40.
        intx -= 40;

        const int tile_x = intx / 8;
        const int tile_y = inty / 8;

        Vec2<Fixnum> wall_pos;

        Hitbox wall_hb;
        wall_hb.dimension_.size_ = {8, 8};
        wall_hb.position_ = &wall_pos;

        auto cached_pos = hitbox_.position_;
        hitbox_.position_ = &pos;

        for (int x = -1; x < 2; ++x) {
            for (int y = -1; y < 2; ++y) {
                auto cx = tile_x + x;
                auto cy = tile_y + y;
                if (cx > -1 and cx < 20 and cy > -1 and cy < 20) {
                    auto t = platform().get_tile(Layer::map_0, cx + 5, cy);
                    if (engine().room_.walls_[cx][cy]) {
                        wall_pos.x = 40 + cx * 8;
                        wall_pos.y = cy * 8;

                        if (wall_hb.overlapping(hitbox_)) {
                            hitbox_.position_ = cached_pos;

                            if (t == 6) {
                                engine().room_.walls_[cx][cy] = false;
                                platform().set_tile(Layer::map_0, cx + 5, cy, 0);
                                return true;
                            }
                            if ((t == 7 or t == 8) and engine().p_->blade_ > 1) {
                                engine().room_.walls_[cx][cy] = false;
                                platform().set_tile(Layer::map_0, cx + 5, cy, 0);
                                return true;
                            }
                        }
                    }
                }
            }
        }

        return false;
    }


    void step()
    {
        cyc_ += 1;
        switch (cyc_) {
        case 1:
            for (auto& e : engine().enemies_) {
                if (e->hitbox().overlapping(hitbox_)) {
                    e->damage(2 + 2 * engine().p_->blade_, *this);
                    dead_ = false;
                }
            }
            for (auto& s : engine().enemy_projectiles_) {

                if (not s->hitbox().overlapping(hitbox_)) {
                    continue;
                }

                auto dir = hflip_ ? Fixnum(2) : Fixnum(-2);

                auto reflect_s = [&](int xo, int yo) {
                    engine().add_object<Shot>(
                        Vec2<Fixnum>{s->x() + xo, s->y() + yo}, dir, 1);
                };

                if (dynamic_cast<Missile*>(s.get())) {
                    reflect_s(0, 3);
                    reflect_s(0, -3);
                    reflect_s(hflip_ ? 3 : -3, 0);
                    engine().add_object<Explo>(Vec2<Fixnum>{s->x() - 4, s->y() - 4});
                    s->kill();
                } else if ((dynamic_cast<Laser*>(s.get()) or
                           dynamic_cast<Vortex*>(s.get())) and
                           engine().p_->blade_ == 3) {
                    reflect_s(0, 3);
                    reflect_s(0, -3);
                    reflect_s(hflip_ ? 3 : -3, 0);
                    reflect_s(hflip_ ? -3 : 3, 0);
                    engine().add_object<Explo>(Vec2<Fixnum>{s->x() - 4, s->y() - 4});
                    s->kill();
                } else if (dynamic_cast<Supershot*>(s.get()) and
                           engine().p_->blade_ >= 2) {
                    reflect_s(0, 2);
                    reflect_s(0, -2);
                    engine().add_object<Explo>(Vec2<Fixnum>{s->x() - 4, s->y() - 4});
                    s->kill();
                } else if (dynamic_cast<Megashot*>(s.get()) and
                           engine().p_->blade_ >= 2) {
                    reflect_s(0, 3);
                    reflect_s(0, -3);
                    reflect_s(hflip_ ? 3 : -3, 0);
                    engine().add_object<Explo>(Vec2<Fixnum>{s->x() - 4, s->y() - 4});
                    s->kill();
                } else if (dynamic_cast<Gigashot*>(s.get()) and
                           engine().p_->blade_ == 3) {
                    reflect_s(0, 3);
                    reflect_s(0, -3);
                    reflect_s(hflip_ ? 3 : -3, 0);
                    reflect_s(hflip_ ? -3 : 3, 0);
                    engine().add_object<Explo>(Vec2<Fixnum>{s->x() - 4, s->y() - 4});
                    s->kill();
                } else if (dynamic_cast<EnemyShot*>(s.get())) {
                    reflect_s(hflip_ ? 3 : -3, 0);
                    s->kill();
                }
            }
            break;

        case 2:
            break;

        case 3:
            this->kill();
            break;
        }

        if (test_wall_collision()) {
        }
    }

private:
    u8 dud_ = 1;
    u8 cyc_ = 0;
};


void Hero::step()
{
    if (engine().g_.tetron_dead_) {
        return;
    }

    if (engine().paused_) {
        return;
    }

    if (engine().g_.hp_ == 0) {
        return;
    }


    if (engine().g_.heat_) {
        switch (suit_level()) {
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


    if (chargeblade_ == 21) {
        chargeblade_ = 20;
    }

    if (blade_level() > 0) {
        if (key_pressed<Key::action_2>()) {
            if (not key_pressed<Key::action_1>()) {
                chargeblade_ += 1;
            }
        } else if (key_pressed<Key::action_1>()) {
            if (not key_pressed<Key::action_2>()) {
                chargeblade_ += 1;
            }
        } else if (not key_pressed<Key::action_1>() and
                   not key_pressed<Key::action_2>() and chargeblade_ == 20) {

            engine().add_object<Blade>(position_, hflip_);

            platform().speaker().play_sound("snd_blade", 8);

            chargeblade_ = 0;
        } else {
            chargeblade_ = 0;
        }
    }


    if (key_pressed<Key::action_2>() and key_pressed<Key::action_1>()) {
        chargewarp_ += 1;
        if (chargewarp_ >= 53) {
            engine().g_.warpready_ = true;
            chargewarp_ = 0;
        }
    } else {
        chargewarp_ = 0;
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
                engine().add_object<Flame>(
                    Vec2<Fixnum>{position_.x + 4, position_.y + 5});
            } else {
                engine().add_object<Flame>(
                    Vec2<Fixnum>{position_.x - 1, position_.y + 5});
            }
        }
    }

    if (not chargeblade_ and engine().g_.autofire_) {
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

    if (engine().g_.heat_ or chargeblade_) {
        heatcyc_ += 1;
        if (heatcyc_ == 6) {
            heatcyc_ = 0;
            if (not chargeblade_) {
                play_sound("snd_heat", 1);
            }
        }
    }
}


void Hero::draw(Platform::Screen& screen) const
{
    if (engine().g_.hp_ == 0) {
        return;
    }

    if (engine().g_.invulnerable_ > 0 and engine().g_.invulnerable_ % 4 < 2) {
        Object::draw(screen);
    }

    if (chargewarp_ >= 20) {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(sprite_index_ + sprite_subimage_);
        spr_.set_flip({hflip_, vflip_});
        spr_.set_position(position_);
        screen.draw(spr_);

        warpcyc_ += 1;
        if (warpcyc_ == 8) {
            warpcyc_ = 0;
        } else if (warpcyc_ == 2) {
            play_sound("snd_chargeblade", 1);
            spr_.set_position({x(), y() - 1});
            screen.draw(spr_);
            spr_.set_position({x(), y() + 1});
            screen.draw(spr_);
        } else if (warpcyc_ == 4) {
            spr_.set_position({x() - 1, y() - 1});
            screen.draw(spr_);
            spr_.set_position({x() + 1, y() + 1});
            screen.draw(spr_);
        } else if (warpcyc_ == 6) {
            spr_.set_position({x() - 1, y()});
            screen.draw(spr_);
            spr_.set_position({x() + 1, y()});
            screen.draw(spr_);
        } else if (warpcyc_ == 0) {
            spr_.set_position({x() - 1, y() + 1});
            screen.draw(spr_);
            spr_.set_position({x() + 1, y() - 1});
            screen.draw(spr_);
        }
        return;
    }

    if (not engine().g_.invulnerable_ or chargeblade_ >= 20) {
        Object::draw(screen);
        if (engine().g_.heat_ or chargeblade_ >= 20) {
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



int Hero::blaster_damage()
{
    int dmg = 1;
    if (blaster_level() == 2) {
        // NOTE: already months into this project, I realized that in the
        // original source code, the level 2 blaster deals 1.5 damage. This is
        // really inconvenient, as I'm not storing health as fixed point. Until
        // I can fix it, the level 2 blaster deals 2 damage half of the time.
        lv2_blaster_damage_inc_ = not lv2_blaster_damage_inc_;
        if (lv2_blaster_damage_inc_) {
            dmg = 2;
        }
    } else if (blaster_level() == 3) {
        dmg = 2;
    }
    return dmg;
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

        int dmg = blaster_damage();

        engine().add_object<Shot>(
            Vec2<Fixnum>{position_.x - 1, position_.y + 3}, Fixnum(-2), dmg);
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

        int dmg = blaster_damage();

        engine().add_object<Shot>(
            Vec2<Fixnum>{position_.x + 2, position_.y + 3}, Fixnum(2), dmg);
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


} // namespace herocore
