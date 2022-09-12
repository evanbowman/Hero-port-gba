#pragma once


#include "bulkAllocator.hpp"
#include "filesystem.hpp"
#include "platform/platform.hpp"
#include "object.hpp"
#include "list.hpp"
#include "objects/enemies/enemy.hpp"
#include "objects/projectile/enemyProjectile.hpp"
#include "objects/projectile/shot.hpp"
#include "objects/fluids/liquidMetal.hpp"
#include "objects/solid.hpp"
#include "objects/hero.hpp"
#include "scene.hpp"



Platform& platform();



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
            } else if constexpr (std::is_base_of<LiquidMetal, T>() or
                                 std::is_same<LiquidMetal, T>()) {
                lm_.push(std::move(obj));
            } else if constexpr (std::is_base_of<Solid, T>()) {
                generic_solids_.push(std::move(obj));
            } else {
                generic_objects_.push(std::move(obj));
            }

            return result;
        }
        return nullptr;
    }


    void respawn_to_checkpoint();
    void begin_game(Difficulty d);


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

        void load(int chunk_x, int chunk_y, bool restore);

        Vec2<int> coord_;

        void render_entrances();

        void clear_adjacent_barriers();

        bool has_exit_up() const;
        bool has_exit_down() const;
        bool has_exit_left() const;
        bool has_exit_right() const;

        u8 zone_ = 0;
    } room_;


    struct Globalstate
    {
        Difficulty difficulty_ = Difficulty::normal;
        bool autofire_ = false;
        bool hero_jetpack_flames_ = true;
        int hp_ = 10;
        int max_hp_ = hp_;
        int invulnerable_ = 0;
        int shot_count_ = 0;
        int heat_ = 0;
        int maxheat_ = 120;
        int suit_ = 0;
        u8 flicker_ = 0;

        Vec2<u8> checkpoint_room_;
        Vec2<u8> checkpoint_coords_;

        void damage(int amount, int extra_invulnerable_time)
        {
            if (invulnerable_) {
                return;
            }

            flicker_ = 1;

            hp_ = std::max(0, (int)(hp_ - amount));
            invulnerable_ = 30 + extra_invulnerable_time;
        }
    } g_;


    Object* hero() const
    {
        return hero_.get();
    }


    void draw_hud_heat();
    void draw_hud();


    void collision_check();


    void load(int chunk_x, int chunk_y, bool restore);


    ObjectList<Enemy> enemies_;
    ObjectList<EnemyProjectile> enemy_projectiles_;
    ObjectList<Shot> player_projectiles_;
    ObjectList<Object> generic_objects_;
    ObjectList<Object> generic_solids_;
    ObjectList<LiquidMetal> lm_;

    ObjectRef<Hero> hero_;


    struct Persistence
    {
        struct TileModify
        {
            u8 tx_;
            u8 ty_;
            u8 room_x_ : 4;
            u8 room_y_ : 4;
            u8 new_value_;
        };

        struct ObjectRemove
        {
            u8 room_x_ : 4;
            u8 room_y_ : 4;
            u8 x_;
            u8 y_;
        };

        Buffer<TileModify, 200> tile_modifications_;
        Buffer<ObjectRemove, 40> objects_removed_;
    };

    DynamicMemory<Persistence> p_;

    void unlock_doors();

private:

    int animcyc_;
    void animate_tiles();

    ScenePtr<Scene> current_scene_;
    ScenePtr<Scene> next_scene_;

    // The original game processed updates at framerate of 40 fps. Our platform
    // hardware implementation targets screens with 60fps, so we need to drop
    // 1/3 of update steps, to bring 60fps down to 40fps.
    int frame_count_ = 0;
};



Engine& engine();



}



template <Key k> bool key_down()
{
    return platform().keyboard().down_transition<k>();
}



template <Key k> bool key_pressed()
{
    return platform().keyboard().pressed<k>();
}
