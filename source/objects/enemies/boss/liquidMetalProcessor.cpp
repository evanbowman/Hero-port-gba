#include "liquidMetalProcessor.hpp"
#include "objects/misc/pickup.hpp"



namespace herocore
{



LiquidMetalProcessor::LiquidMetalProcessor(const Vec2<Fixnum>& pos,
                                           u8 spawn_x,
                                           u8 spawn_y) :
    Enemy(TaggedObject::Tag::ignored, Health{128}),
    spawn_x_(spawn_x),
    spawn_y_(spawn_y)
{

}



void LiquidMetalProcessor::step()
{
    engine().p_->objects_removed_.push_back({
                (u8)engine().room_.coord_.x,
                (u8)engine().room_.coord_.y,
                spawn_x_,
                spawn_y_
            });

    engine().boss_completed();

    kill();

    engine().add_object<Pickup>(position_, Pickup::blade);
}



}
