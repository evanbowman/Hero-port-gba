#pragma once

#include "taggedObject.hpp"



namespace herocore
{



class EnemyProjectile : public Object
{
public:

    EnemyProjectile(const Vec2<Fixnum>& pos)
    {
        position_ = pos;
    }


    void set_speed(const Vec2<Fixnum>& speed)
    {
        speed_ = speed;
    }


};



}
