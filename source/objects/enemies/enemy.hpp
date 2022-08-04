#pragma once

#include "taggedObject.hpp"
#include "objects/projectile/shot.hpp"



namespace herocore
{



class Enemy : public TaggedObject
{
public:

    using Health = s16;

    Enemy(Tag tag, Health health) :
        TaggedObject(tag),
        health_(health)
    {
    }


    virtual void damage(Health dmg, Shot& s)
    {
        health_ = std::max(0, health_ - dmg);
    }


    virtual int collision_damage() const
    {
        return 2;
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


protected:
    Health health_;
};



}
