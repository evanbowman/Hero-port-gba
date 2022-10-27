#pragma once

#include "object.hpp"


namespace herocore
{


class Solid : public Object
{
public:
    virtual bool overlapping(Object& other)
    {
        return hitbox().overlapping(other.hitbox());
    }
};


} // namespace herocore
