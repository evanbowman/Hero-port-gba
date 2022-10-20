#pragma once

#include "scene.hpp"
#include "engine.hpp"
#include "objects/particles/bigExplo.hpp"
#include "objects/enemies/elite/barrier.hpp"



namespace herocore
{



bool is_boss_level(Difficulty d, Vec2<u8> room);



class DiffSelScene : public Scene
{
public:

};



class MapScene : public Scene
{
public:

    ScenePtr<Scene> step() override;


    void display() override;


    void enter(Scene& prev_scene) override;

private:

    void show_worldmap();
    void show_zoomedmap();

    int sel_ = 0;

    Buffer<Vec2<u8>, 16> warp_points_;

    Vec2<int> pt_;
    u8 flicker_cyc_ = 0;
    bool flicker_on_ = true;
};



class OverworldScene : public Scene
{
    int respawn_countdown_ = 0;

public:

    ScenePtr<Scene> step() override
    {
        if (engine().g_.hp_ <= 0) {
            if (respawn_countdown_ == 0) {
                platform().speaker().play_sound("snd_death", 10);
                platform().speaker().play_sound("snd_explo3", 1);
                respawn_countdown_ = 80;
                engine().add_object<ExploSpewer>(engine().hero()->position());
            } else if (respawn_countdown_ == 1) {
                platform().speaker().play_sound("snd_herospawn", 11);
                engine().respawn_to_checkpoint();
            }
            --respawn_countdown_;
        }

        auto step_list =
                [&](auto& objects, auto on_destroy) {
                    for (auto it = objects.begin(); it not_eq objects.end();) {
                        if ((*it)->dead()) {
                            on_destroy(*it);
                            it = objects.erase(it);
                        } else {
                            (*it)->step();
                            ++it;
                        }
                    }
                };

        auto& e = engine();

        bool enemies_destroyed = false;

        step_list(e.enemies_, [&](auto&) { enemies_destroyed = true; });
        step_list(e.enemy_projectiles_, [](auto&) {});
        step_list(e.generic_objects_, [](auto&) {});
        step_list(e.generic_solids_, [](auto&) {});
        step_list(e.player_projectiles_, [&](auto&) {--e.g_.shot_count_;});

        if (enemies_destroyed) {
            if (e.enemies_.empty()) {
                e.unlock_doors();
            } else {
                // NOTE: I treated the barrier as an enemy within the engine,
                // but the presence of a barrier should not keep doors from
                // opening.
                bool all_barriers = true;
                for (auto& e : e.enemies_) {
                    if (not dynamic_cast<Barrier*>(e.get())) {
                        all_barriers = false;
                        break;
                    }
                }
                if (all_barriers) {
                    e.unlock_doors();
                }
            }
        }

        if (key_down<Key::start>()) {
            engine().paused_ = true;
            return scene_pool::alloc<MapScene>();
        }

        return null_scene();
    }


    void display() override
    {
        auto draw_list =
            [&](auto& objects) {
                for (auto& obj : objects) {

                    if (not obj->dead()) {
                        obj->draw(platform().screen());
                    }
                }
            };

        auto& e = engine();

        e.hero_->draw(platform().screen());

        draw_list(e.enemy_projectiles_);
        draw_list(e.player_projectiles_);
        draw_list(e.enemies_);
        draw_list(e.generic_solids_);
        draw_list(e.generic_objects_);
    }

};




}
