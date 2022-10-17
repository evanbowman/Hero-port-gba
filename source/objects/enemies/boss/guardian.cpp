#include "guardian.hpp"



namespace herocore
{



// obj_guardianneck == GuardianUpper and GuardianLower
// obj_guardian == GuardianCore



class GuardianUpper : public Enemy
{
public:
    GuardianUpper(GuardianCore* core) :
        Enemy(TaggedObject::Tag::ignored, Health{9999}),
        core_(core)
    {
    }


    void step() override
    {
        position_ = core_->position();
        position_.y -= 23;
        position_.x -= 16;
    }


    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(178);
        spr_.set_position(position_);
        spr_.set_flip({hflip_, vflip_});
        screen.draw(spr_);

        auto pos = position_;
        pos.x += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(179);
        screen.draw(spr_);

        pos = position_;
        pos.x += 16;
        pos.y += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(181);
        screen.draw(spr_);

        pos = position_;
        pos.y += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(180);
        screen.draw(spr_);
    }

private:
    GuardianCore* core_;
};



class GuardianLower : public Enemy
{
public:
    GuardianLower(GuardianCore* core) :
        Enemy(TaggedObject::Tag::ignored, Health{9999}),
        core_(core)
    {
    }


    void step() override
    {
        position_ = core_->position();
        // position_.y += 23;
        position_.x -= 16;
    }


    void draw(Platform::Screen& screen) const override
    {
        Sprite spr_;
        spr_.set_origin({origin_.x, origin_.y});
        spr_.set_texture_index(184);
        spr_.set_position(position_);
        spr_.set_flip({hflip_, vflip_});
        screen.draw(spr_);

        auto pos = position_;
        pos.x += 16;
        spr_.set_position(pos);
        spr_.set_texture_index(185);
        screen.draw(spr_);
    }


private:
    GuardianCore* core_;
};




GuardianCore::GuardianCore(const Vec2<Fixnum>& pos, u8 spawn_x, u8 spawn_y) :
    Enemy(TaggedObject::Tag::ignored, Health{128}),
    spawn_x_(spawn_x),
    spawn_y_(spawn_y)
{
    hitbox_.dimension_.size_ = {4, 4};
    hitbox_.dimension_.origin_ = {3, 3};
    origin_ = {3, 3};

    sprite_index_ = 164;

    position_ = pos;
    position_.y -= 16;

    if (not (lower_ = engine().add_object<GuardianLower>(this))) {
        platform().fatal("failed to alloc guardian segment");
    }
    if (not (upper_ = engine().add_object<GuardianUpper>(this))) {
        platform().fatal("failed to alloc guardian segment");
    }
}



void GuardianCore::step()
{
    Enemy::step();

    animcyc_ += 1;
    if (animcyc_ == 4) {
        animcyc_ = 0;
        sprite_subimage_ += 1;
        if (sprite_subimage_ == 4) {
            sprite_subimage_ = 0;
        }
    }
}



}
