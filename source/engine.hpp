#pragma once


#include "filesystem.hpp"
#include "platform/platform.hpp"
#include "object.hpp"
#include "list.hpp"



namespace herocore
{



enum class Difficulty { normal, hard };



class Engine {
public:
    Engine(Platform& pf);

    void run();


    template <typename T, typename ...Args>
    T* add_object(Args&& ...args)
    {
        if (auto obj = alloc_object<T>(std::forward<Args>(args)...)) {
            auto result = obj.get();
            objects_.push(std::move(obj));
            return result;
        }
        return nullptr;
    }


    Difficulty difficulty_ = Difficulty::hard;
    bool autofire_ = false;
    int hp_ = 10;



    Object* hero() const
    {
        return hero_.get();
    }


    void draw_hud();


private:
    ObjectList<Object> objects_;
    ObjectRef<Object> hero_;

    // The original game processed updates at framerate of 40 fps. Our platform
    // hardware implementation targets screens with 60fps, so we need to drop
    // 1/3 of update steps, to bring 60fps down to 40fps.
    int frame_count_ = 0;
};



Engine& engine();



}



Platform& platform();



template <Key k> bool key_down()
{
    return platform().keyboard().down_transition<k>();
}



template <Key k> bool key_pressed()
{
    return platform().keyboard().pressed<k>();
}
