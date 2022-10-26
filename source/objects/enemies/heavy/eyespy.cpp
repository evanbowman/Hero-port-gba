#include "eyespy.hpp"



namespace herocore
{



class EyespyRadius : public Object
{
public:

    Eyespy* parent_;
    u8 cyc_ = 0;
    int timeline_ = 0;
    Fixnum spd_ = 0;


    EyespyRadius(Eyespy* parent, const Vec2<Fixnum>& position) :
        parent_(parent)
    {
        position_ = position;
        position_.x -= 32;
        position_.y -= 32;
    }


    void step() override
    {
        cyc_ += 1;
        if (cyc_ == 4) {
            cyc_ = 0;
        }

        if (parent_->wake_) {
            this->kill();
        }

        switch (timeline_++) {
        case 15:
            spd_ = 0;
            break;

        case 40:
            spd_ = 1;
            break;

        case 65:
            spd_ = 0;
            break;

        case 90:
            spd_ = -1;
            break;

        case 115:
            spd_ = 0;
            break;

        case 140:
            spd_ = -1;
            break;

        case 165:
            spd_ = 0;
            break;

        case 190:
            spd_ = 1;
            break;

        case 214:
            timeline_ = 15;
            break;
        }

        // FIXME: how does original codebase determine look dir?
        if (engine().room_.coord_.y == 6) {
            speed_.x = spd_;
        }
        if (engine().room_.coord_.y == 7) {
            speed_.y = spd_;
        }
        if (engine().room_.coord_.y == 8) {
            if (engine().room_.coord_.x == 8) {
                speed_.y = spd_;
            } else {
                speed_.x = spd_;
            }
        }


        Object::step();

        Vec2<Fixnum> hb_pos = position_;
        hb_pos.x += 22;

        Hitbox hb;
        hb.dimension_.size_ = {20, 64};
        hb.position_ = &hb_pos;

        auto parent_wake = [&] {
                               kill();
                               parent_->wake_ = true;
                               platform().speaker().play_sound("snd_spot", 20);
                           };

        // Because I'm lazy, approximating the eyespy radius with a few bounding
        // boxes. Two long rectangular bounding boxes represent the radius' max
        // extents in the x/y direction. Another bounding box represents the
        // area enclosed by drawing a squre box by connecting the shape's inner
        // corners. This represents the octagonal shape reasonably well, but in
        // hindsite, maybe I could have used a circle. But then I'd have to
        // write code for testing intersection between a rectange and a
        // circle... If anyone wants to fix this code, feel free...

        if (hb.overlapping(engine().hero()->hitbox())) {
            parent_wake();
            return;
        }

        hb.dimension_.size_ = {64, 20};
        hb_pos.x = position_.x;
        hb_pos.y = position_.y + 22;

        if (hb.overlapping(engine().hero()->hitbox())) {
            parent_wake();
            return;
        }

        hb.dimension_.size_ = {64 - 14, 64 - 14};
        hb_pos.x = position_.x + 7;
        hb_pos.y = position_.y + 7;

        if (hb.overlapping(engine().hero()->hitbox())) {
            parent_wake();
            return;
        }
    }


    void draw(Platform::Screen& screen) const override
    {
        if (cyc_ < 3) {
            return;
        }

        Sprite spr;

        auto chunk =
            [&](int xo, int yo, int t, bool xflip, bool yflip) {
                auto pos = position_;
                pos.x += xo;
                pos.y += yo;
                spr.set_position(pos);
                spr.set_flip({xflip, yflip});
                spr.set_texture_index(t);
                screen.draw(spr);
            };

        chunk(0, 0, 203, false, false);
        chunk(0, 16, 202, false, false);
        chunk(16, 0, 204, false, false);

        chunk(32, 0, 204, true, false);
        chunk(48, 0, 203, true, false);
        chunk(48, 16, 202, true, false);

        chunk(0, 32, 202, false, true);
        chunk(0, 48, 203, false, true);
        chunk(16, 48, 204, false, true);

        chunk(32, 48, 204, true, true);
        chunk(48, 48, 203, true, true);
        chunk(48, 32, 202, true, true);
    }
};



Eyespy::Eyespy(const Vec2<Fixnum>& pos) :
    Enemy(TaggedObject::Tag::ignored, Health{64})
{
    position_ = pos;

    sprite_index_ = 200;
    origin_ = {8, 8};
    hitbox_.dimension_.origin_ = {4, 4};
    hitbox_.dimension_.size_ = {8, 8};

    engine().add_object<EyespyRadius>(this, pos);
}




void Eyespy::step()
{
    if (health_ == 0) {
        kill();
        platform().speaker().play_sound("snd_explo2", 6);
        engine().add_object<BigExplo>(Vec2<Fixnum>{
                position_.x - 4, position_.y - 4});
        return;
    }

    if (wake_) {
        gigacyc_ += 2;
        if (gigacyc_ > 360) {
            gigacyc_ -= 360;
        }
        gigacyc2_ += 1;
        if (gigacyc2_ == 16) {
            gigacyc2_ = 0;

            auto o = position_;
            o.x -= 6;
            o.y -= 6;
            if (auto e = engine().add_object<Gigashot>(o)) {
                auto d = rotate({1, 0}, gigacyc_);
                d = d * (3.f / 2);
                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
            }

            if (auto e = engine().add_object<Gigashot>(o)) {
                auto d = rotate({1, 0}, gigacyc_ + 180);
                d = d * (3.f / 2);
                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
            }

            if (auto e = engine().add_object<Gigashot>(o)) {
                auto d = rotate({1, 0}, gigacyc_ + 90);
                d = d * (3.f / 2);
                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
            }

            if (auto e = engine().add_object<Gigashot>(o)) {
                auto d = rotate({1, 0}, gigacyc_ + 270);
                d = d * (3.f / 2);
                e->set_speed({Fixnum(d.x), Fixnum(d.y)});
            }
        }
    }

    if (health_ not_eq 64) {
        if (not wake_) {
            wake_ = true;
            platform().speaker().play_sound("snd_spot", 20);
        }
    }

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

    case 40: {
        break;
    }

    case 90: {

        break;
    }

    case 139:

        break;
    }
}




}
