#pragma once

#include "engine.hpp"
#include "objects/particles/explo.hpp"
#include "objects/solid.hpp"


namespace herocore
{


class Savepoint : public Solid
{
private:
    int cyc_ = 0;
    bool avail_;
    bool visible_ = false;

public:
    Savepoint(const Vec2<Fixnum>& pos, bool avail = true) : avail_(avail)
    {
        position_ = pos;
        position_.x += 2;
        position_.y += 4;

        sprite_index_ = 99;

        hitbox_.dimension_.size_ = {12, 12};
    }

    void step() override
    {
        Hitbox hb;
        Vec2<Fixnum> pos;
        pos.x = x() - 4;
        pos.y = y() - 12;
        hb.position_ = &pos;
        hb.dimension_.size_ = {20, 12};

        if (avail_ and hb.overlapping(engine().hero()->hitbox())) {
            engine().p_->checkpoint_music_ = platform().speaker().current_music();
            engine().p_->checkpoint_room_ = engine().room_.coord_.cast<u8>();
            engine().p_->checkpoint_coords_.x = (x() - 40 + 4).as_integer();
            engine().p_->checkpoint_coords_.y = (y() - 12).as_integer();
            engine().g_.hp_ = 10 + engine().p_->level_;
            engine().savegame();
            avail_ = false;
            platform().sleep(4);
            platform().speaker().play_sound("snd_save", 10);
            engine().draw_hud();
            engine().add_object<ExploSpewer2>(position_);
        }

        cyc_ += 1;
        if (cyc_ == 10) {
            cyc_ = true;
            visible_ = false;
        } else if (cyc_ >= 5) {
            visible_ = true;
        }
    }

    void draw(Platform::Screen& screen) const
    {
        Solid::draw(screen);

        if (avail_ and visible_) {
            Sprite spr_;
            spr_.set_texture_index(97);
            spr_.set_position({x() - 4, y() - 12});
            screen.draw(spr_);
            spr_.set_texture_index(98);
            spr_.set_position({x() + 12, y() - 12});
            screen.draw(spr_);
        }
    }
};


} // namespace herocore
