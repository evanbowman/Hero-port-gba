#include "engine.hpp"
#include "graphics/overlay.hpp"
#include "localization.hpp"
#include "number/endian.hpp"
#include "string.hpp"
#include "version.hpp"
#include "objects/hero.hpp"
#include "objects/enemies/light/drone.hpp"
#include "objects/enemies/light/reaver.hpp"
#include "objects/enemies/light/crusher.hpp"
#include "objects/enemies/light/spew.hpp"
#include "objects/enemies/light/bolt.hpp"



static Platform* _platform;



Platform& platform()
{
    return *_platform;
}



namespace herocore
{



static Engine* bound_engine;



Engine& engine()
{
    return *bound_engine;
}



Engine::Engine(Platform& pf) : hero_(null_object())
{
    pf.load_overlay_texture("overlay");
    pf.load_sprite_texture("spritesheet");
    pf.load_tile0_texture("tile0");
    bound_engine = this;

    _platform = &pf;
    {
        _platform->screen().clear();
        _platform->enable_glyph_mode(true);
        _platform->screen().fade(1.f);
        _platform->screen().display();
    }
    _platform->screen().fade(0.f);
    _platform->screen().display();

    hero_ = alloc_object<Hero>(Vec2<Fixnum>{80, 80});
    add_object<Drone>(Vec2<Fixnum>{70, 50});
    add_object<Reaver>(Vec2<Fixnum>{70, 80});
    add_object<Crusher>(Vec2<Fixnum>{100, 80});
    add_object<Spew>(Vec2<Fixnum>{170, 80});
    add_object<Bolt>(Vec2<Fixnum>{80, 40});

    room_.clear();
    room_.load(0, 0);

    pf.speaker().play_music("zone5", 0);

    draw_hud();
}



void Engine::collision_check()
{
    for (auto& p : enemy_projectiles_) {
        if (p->hitbox().overlapping(hero_->hitbox())) {
            p->kill();
            // TODO: damage!
        }
    }

    for (auto& p : player_projectiles_) {
        for (auto& e : enemies_) {
            if (p->hitbox().overlapping(e->hitbox())) {
                p->kill();
                // TODO: damage!
                break;
            }
        }
    }

    for (auto& e : enemies_) {
        if (e->hitbox().overlapping(hero_->hitbox())) {
            // TODO: damage!
        }
    }
}



void Engine::run()
{
    while (true) {

        if (frame_count_ == 2) {
            frame_count_ = 0;
        } else {

            _platform->keyboard().poll();

            hero_->step();

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

            step_list(enemies_, [](auto&) {});
            step_list(enemy_projectiles_, [](auto&) {});
            step_list(generic_objects_, [](auto&) {});
            step_list(player_projectiles_, [&](auto&) {--g_.shot_count_;});

            ++frame_count_;
        }

        collision_check();

        _platform->screen().clear();

        auto draw_list =
            [&](auto& objects) {
                for (auto& obj : objects) {

                    if (not obj->dead()) {
                        obj->draw(_platform->screen());
                    }
                }
            };

        hero_->draw(_platform->screen());

        draw_list(enemies_);
        draw_list(enemy_projectiles_);
        draw_list(player_projectiles_);
        draw_list(generic_objects_);

        _platform->screen().display();
    }

}



void Engine::draw_hud()
{
    auto& pf = platform();

    // TODO: actually draw the hud correctly.
    // NOTE: technically, what I'm drawing is too wide, the health and heat bars
    // are actually 20 pixels wide, not 24.

    for (int y = 1; y < 19; ++y) {
        pf.set_tile(Layer::overlay, 2, y, 83);
        pf.set_tile(Layer::overlay, 3, y, 84);
        pf.set_tile(Layer::overlay, 4, y, 85);

        pf.set_tile(Layer::overlay, 30 - 5, y, 83);
        pf.set_tile(Layer::overlay, 30 - 4, y, 84);
        pf.set_tile(Layer::overlay, 30 - 3, y, 85);
    }

    pf.set_tile(Layer::overlay, 2, 19, 86);
    pf.set_tile(Layer::overlay, 3, 19, 87);
    pf.set_tile(Layer::overlay, 4, 19, 88);

    pf.set_tile(Layer::overlay, 30 - 5, 19, 86);
    pf.set_tile(Layer::overlay, 30 - 4, 19, 87);
    pf.set_tile(Layer::overlay, 30 - 3, 19, 88);
}



void Engine::Room::load(int chunk_x, int chunk_y)
{
    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 20; ++y) {
            if (x == 0 or y == 0 or x == 19 or y == 19) {
                walls_[x][y] = true;
                platform().set_tile(Layer::map_0, x + 5, y, 1);
            }
        }
    }
}



}
