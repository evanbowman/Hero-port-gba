#include "liquidMetalProcessor.hpp"



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
    kill();
}



}
