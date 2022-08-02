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


private:
    Health health_;
};



}
