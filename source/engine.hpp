#pragma once


#include "filesystem.hpp"
#include "platform/platform.hpp"
#include "object.hpp"
#include "list.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/projectile/enemyProjectile.hpp"
#include "objects/projectile/shot.hpp"



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
            if constexpr (std::is_base_of<Enemy, T>()) {
                enemies_.push(std::move(obj));
            } else if constexpr(std::is_base_of<EnemyProjectile, T>()) {
                enemy_projectiles_.push(std::move(obj));
            } else if constexpr (std::is_base_of<Shot, T>()) {
                player_projectiles_.push(std::move(obj));
                ++g_.shot_count_;
            } else {
                generic_objects_.push(std::move(obj));
            }

            return result;
        }
        return nullptr;
    }


    struct Room
    {
        bool walls_[20][20];

        void clear()
        {
            for (auto& row : walls_) {
                for (auto& w : row) {
                    w = false;
                }
            }
        }

        void load(int chunk_x, int chunk_y);

    } room_;


    struct Globalstate
    {
        Difficulty difficulty_ = Difficulty::hard;
        bool autofire_ = false;
        bool hero_jetpack_flames_ = true;
        int hp_ = 10;
        int shot_count_ = 0;
    } g_;


    Object* hero() const
    {
        return hero_.get();
    }


    void draw_hud();


    void collision_check();


private:
    ObjectList<Enemy> enemies_;
    ObjectList<EnemyProjectile> enemy_projectiles_;
    ObjectList<Shot> player_projectiles_;
    ObjectList<Object> generic_objects_;

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
