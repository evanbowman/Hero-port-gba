#pragma once

#include "taggedObject.hpp"



namespace herocore
{



class Enemy : public TaggedObject
{
public:

    using Health = u16;

    Enemy(Tag tag, Health health) :
        TaggedObject(tag),
        health_(health)
    {
    }


    Health health() const
    {
        return health_;
    }


    void move(const Vec2<Fixnum>& pos)
    {
        // auto prev = position_;
        position_ = pos;
        // if (not place_free(position_)) {
        //     position_ = prev;
        // }
    }


private:
    Health health_;
};



}
