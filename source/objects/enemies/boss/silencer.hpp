#pragma once

#include "objects/enemies/enemy.hpp"
#include "engine.hpp"
#include "fmt.hpp"
#include "number/random.hpp"
#include "objects/particles/explo.hpp"



namespace herocore
{



class SilencerNeck : public Enemy
{
private:
    Enemy* owner_ = nullptr;
    Vec2<int> offset_;

public:

    SilencerNeck(Enemy* owner, Vec2<int> offset) :
        Enemy(TaggedObject::Tag::ignored, Health{2}),
        owner_(owner),
        offset_(offset)
    {
        hitbox_.dimension_.size_ = {2, 4};
        hitbox_.dimension_.origin_ = {0, 0};
        if (offset.x > 0) {
            hitbox_.dimension_.origin_ = {2, 0};
        }

        sprite_index_ = 163;
    }


    void step() override
    {
        if (health_ == 0) {
            kill();
        }
        position_.x = owner_->position().x + offset_.x;
        position_.y = owner_->position().y + offset_.y;
    }

};



class Silencer : public Enemy
{
public:

    Silencer(const Vec2<Fixnum>& pos,
             u8 spawn_x,
             u8 spawn_y) :
        Enemy(TaggedObject::Tag::ignored, Health{32})
    {
        position_ = pos;

        sprite_index_ = 162;

        hitbox_.dimension_.size_ = {32, 16};
        hitbox_.dimension_.origin_ = {16, 8};
        origin_ = {16, 8};

        engine().add_object<SilencerNeck>(this, Vec2<int>{-5, -2});
        engine().add_object<SilencerNeck>(this, Vec2<int>{-7, -2});
        engine().add_object<SilencerNeck>(this, Vec2<int>{-9, -2});
        engine().add_object<SilencerNeck>(this, Vec2<int>{-11, -2});
        engine().add_object<SilencerNeck>(this, Vec2<int>{-13, -2});

        engine().add_object<SilencerNeck>(this, Vec2<int>{4, -2});
        engine().add_object<SilencerNeck>(this, Vec2<int>{6, -2});
        engine().add_object<SilencerNeck>(this, Vec2<int>{8, -2});
        engine().add_object<SilencerNeck>(this, Vec2<int>{10, -2});
        engine().add_object<SilencerNeck>(this, Vec2<int>{12, -2});

        speed_.y = Fixnum(0.001f);
    }



    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(sprite_index_ + sprite_subimage_);
        spr_.set_position(position_);
        screen.draw(spr_);

        spr_.set_position({x() + 16, y()});
        spr_.set_flip({true, false});
        screen.draw(spr_);
    }



    bool damage(Health dmg, Shot& s)
    {
        Hitbox hb;
        hb.dimension_.size_ = {32, 5};
        hb.dimension_.origin_ = hitbox_.dimension_.origin_;
        auto p = position_;
        hb.position_ = &p;
        if (hb.overlapping(s.hitbox())) {
            s.kill();
            return false;
        }

        p.y += 11;
        if (hb.overlapping(s.hitbox())) {
            s.kill();
            return false;
        }

        return false;
    }


    void step() override
    {
        if (health_ <= 0) {
            kill();
            engine().add_object<Explo>(position_);
            return;
        }

        Enemy::step();

        switch (timeline_++) {
        case 0:
            platform().speaker().play_music("boss", 0);
            break;

        case 10:
            break;

        case 20:
            break;

        case 30:
            break;

        case 40:
            break;

        case 75:
            break;

        case 99:
            timeline_ = 40;
            break;
        }

        if (not place_free({position_.x + speed_.x,
                            position_.y + speed_.y})) {
            auto x = position_.x;
            auto y = position_.y;
            if (not place_free({x + 2, y}) or not place_free({x - 2, y})) {
                speed_.x = speed_.x * -1;
            }
            if (not place_free({x, y + 2}) or not place_free({x, y - 2})) {
                speed_.y = speed_.y * -1;
            }
        }
    }


private:

    int timeline_ = 0;

};



}
