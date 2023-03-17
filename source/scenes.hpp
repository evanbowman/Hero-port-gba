#pragma once

#include "engine.hpp"
#include "objects/enemies/elite/barrier.hpp"
#include "objects/particles/bigExplo.hpp"
#include "graphics/overlay.hpp"
#include "scene.hpp"


namespace herocore
{


bool is_boss_level(Difficulty d, Vec2<u8> room);


class DiffSelScene : public Scene
{
public:
};



class CreditsScene : public Scene
{
public:

    void enter(Scene& prev_scene) override;
    void exit(Scene& prev_scene) override;


    ScenePtr<Scene> step() override;


private:
    Buffer<Text, 7> text_lines_;
    int cnt_ = 0;
    int scroll_ = 0;
};



class TitleScreenScene : public Scene
{
public:

    void enter(Scene& prev_scene) override;
    void exit(Scene& prev_scene) override;


    ScenePtr<Scene> step() override;


private:
    Buffer<Text, 7> text_lines_;
    int sel_ = 0;
    int cnt_ = 0;
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

    Buffer<Vec2<u8>, 20> warp_points_;

    Vec2<int> pt_;
    u8 flicker_cyc_ = 0;
    u8 warpcyc_ = 0;
    u8 warp_sprite_subimage_ = 0;
    bool flicker_on_ = true;

    std::optional<Text> zone_text_;

public:
    bool warpmode_ = false;
};



class OverworldScene : public Scene
{
    int respawn_countdown_ = 0;

public:

    ScenePtr<Scene> step() override;


    void enter(Scene&) override
    {
        engine().draw_hud();
    }


    void display() override
    {
        auto draw_list = [&](auto& objects) {
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



class EndgameStatsScene : public Scene
{
public:

    void enter(Scene& prev_scene) override;


private:
    Buffer<Text, 7> text_lines_;
};



class FadeOutScene : public OverworldScene
{
public:

    ScenePtr<Scene> step() override
    {
        OverworldScene::step();

        ++cyc_;
        if (cyc_ == 2) {
            cyc_ = 0;
            ++counter_;
        }

        platform().screen().fade(counter_ / 255.f, ColorConstant::silver_white);

        if (counter_ == 255) {
            platform().fill_overlay(95);
            platform().speaker().play_music("ending", 0);
            return scene_pool::alloc<EndgameStatsScene>();
        }

        return null_scene();
    }


private:
    int counter_ = 0;
    int cyc_ = 0;
};



ScenePtr<Scene> intro_sequence();



} // namespace herocore
