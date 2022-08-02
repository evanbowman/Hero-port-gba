#pragma once

#include "taggedObject.hpp"



namespace herocore
{



class EnemyProjectile : public Object
{
private:

    u8 force_;
    u8 heat_;

public:

    EnemyProjectile(const Vec2<Fixnum>& pos,
                    u8 force,
                    u8 heat) :
        force_(force),
        heat_(heat)
    {
        position_ = pos;
    }


    u8 force() const
    {
        return force_;
    }


    u8 heat() const
    {
        return heat_;
    }


    void set_speed(const Vec2<Fixnum>& speed)
    {
        speed_ = speed;
    }


    virtual void on_hero_collision()
    {
        kill();
    }


};



}
