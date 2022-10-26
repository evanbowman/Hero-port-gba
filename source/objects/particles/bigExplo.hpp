#pragma once

#include "object.hpp"
#include "engine.hpp"



namespace herocore
{



class BigExplo : public Object
{
private:

    int anim_cyc_ = 0;

public:

    BigExplo(const Vec2<Fixnum>& pos)
    {
        position_ = pos;

        sprite_index_ = 24;
        origin_ = {8, 8};
    }


    void step() override
    {
        Object::step();

        anim_cyc_ += 1;
        if (anim_cyc_ == 3) {
            anim_cyc_ = 0;
            sprite_subimage_ += 1;
            if (sprite_subimage_ == 6) {
                kill();
            }
        }
    }


private:

    int counter_ = 20;


};



class ExploSpewer : public Object
{
private:
    int cyc_ = 0;

public:

    ExploSpewer(const Vec2<Fixnum>& pos)
    {
        position_ = pos;
    }


    void step() override
    {
        if (cyc_ == 0) {
            engine().add_object<BigExplo>(Vec2<Fixnum>{x(), y()});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() - 16, y()});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() + 16, y()});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x(), y() - 16});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x(), y() + 16});
        }
        if (cyc_ == 4) {
            engine().add_object<BigExplo>(Vec2<Fixnum>{x(), y()});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() - 8, y() - 8});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() + 8, y() - 8});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() + 8, y() + 8});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() - 8, y() + 8});
        }
        if (cyc_ == 8) {
            engine().add_object<BigExplo>(Vec2<Fixnum>{x(), y()});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() - 16, y() - 16});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() + 16, y() - 16});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() + 16, y() + 16});
            engine().add_object<BigExplo>(Vec2<Fixnum>{x() - 16, y() + 16});
        }

        if (cyc_ == 9) {
            kill();
        }
        cyc_ += 1;
    }
};



}
