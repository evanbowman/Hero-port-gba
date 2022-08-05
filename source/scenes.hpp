#pragma once

#include "scene.hpp"
#include "engine.hpp"
#include "objects/particles/bigExplo.hpp"



namespace herocore
{



class DiffSelScene : public Scene
{
public:

};



class OverworldScene : public Scene
{
    int respawn_countdown_ = 0;

public:

    ScenePtr<Scene> step() override
    {
        if (engine().g_.hp_ <= 0) {
            if (respawn_countdown_ == 0) {
                respawn_countdown_ = 60;
                engine().add_object<ExploSpewer>(engine().hero()->position());
            } else if (respawn_countdown_ == 1) {
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

        if (enemies_destroyed and e.enemies_.empty()) {
            e.unlock_doors();
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
