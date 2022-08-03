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
#include "objects/enemies/light/blomb.hpp"
#include "objects/enemies/light/turret.hpp"
#include "maps.hpp"
#include "scene.hpp"
#include "scenes.hpp"



namespace herocore::scene_pool
{
    _Pool pool_("scene-pool");
}




herocore::ScenePtr<herocore::Scene> herocore::null_scene()
{
    return {nullptr, scene_pool::deleter};
}




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



Engine::Engine(Platform& pf) :
    hero_(alloc_object<Hero>(Vec2<Fixnum>{80, 80})),
    current_scene_(scene_pool::alloc<OverworldScene>()),
    next_scene_(null_scene())
{
    pf.load_overlay_texture("overlay");
    pf.load_sprite_texture("spritesheet");
    pf.load_tile0_texture("tile0");
    pf.enable_glyph_mode(true);
    bound_engine = this;

    current_scene_->enter(*current_scene_);

    _platform = &pf;
    {
        _platform->screen().clear();
        _platform->enable_glyph_mode(true);
        _platform->screen().fade(1.f);
        _platform->screen().display();
    }
    _platform->screen().fade(0.f);
    _platform->screen().display();

    // add_object<Drone>(Vec2<Fixnum>{70, 50});
    // add_object<Reaver>(Vec2<Fixnum>{70, 80});
    // add_object<Crusher>(Vec2<Fixnum>{100, 80});
    // add_object<Spew>(Vec2<Fixnum>{170, 80});
    // add_object<Bolt>(Vec2<Fixnum>{80, 40});
    // add_object<Blomb>(Vec2<Fixnum>{64, 8});
    // if (auto t = add_object<Turret>(Vec2<Fixnum>{192, 48})) {
    //     t->set_left();
    // }

    begin_game(g_.difficulty_);

    draw_hud();
}



void Engine::begin_game(Difficulty d)
{
    room_.clear();

    if (d == Difficulty::hard) {
        room_.load(6, 0);
    } else {
        room_.load(11, 14);
    }
}



void Engine::collision_check()
{
    for (auto& p : enemy_projectiles_) {
        if (p->hitbox().overlapping(hero_->hitbox())) {
            p->on_hero_collision();
            g_.damage(p->force());
            draw_hud();
        }
    }

    for (auto& p : player_projectiles_) {
        for (auto& e : enemies_) {
            if (p->hitbox().overlapping(e->hitbox())) {
                p->kill();
                e->damage(1);
                break;
            }
        }
    }

    for (auto& e : enemies_) {
        if (e->hitbox().overlapping(hero_->hitbox())) {
            g_.damage(e->collision_damage());
            draw_hud();
        }
    }
}



void Engine::run()
{
    platform().speaker().play_music("zone1", 0);

    while (true) {

        if (frame_count_ == 2) {
            frame_count_ = 0;

            // collision_check();

        } else {

            _platform->keyboard().poll();

            if (next_scene_) {
                next_scene_->enter(*current_scene_);
                current_scene_ = std::move(next_scene_);
            }

            if (key_down<Key::alt_1>()) {
                g_.autofire_ = not g_.autofire_;
            }

            hero_->step();

            const auto hpos = hero_->position();
            if (hpos.x > Fixnum(196)) {
                load(room_.coord_.x + 1, room_.coord_.y);
                hero_->set_position({50, hero_->position().y});
                continue;
            } else if (hpos.x < Fixnum(41)) {
                load(room_.coord_.x - 1, room_.coord_.y);
                hero_->set_position({182, hero_->position().y});
                continue;
            } else if (hpos.y > Fixnum(154)) {
                load(room_.coord_.x, room_.coord_.y + 1);
                hero_->set_position({hero_->position().x, 10});
                continue;
            } else if (hpos.y < 2) {
                load(room_.coord_.x, room_.coord_.y - 1);
                hero_->set_position({hero_->position().x, 148});
                continue;
            }

            next_scene_ = current_scene_->step();

            if (next_scene_) {
                current_scene_->exit(*next_scene_);
            }

            ++frame_count_;

            collision_check();
        }

        _platform->screen().clear();

        if (g_.flicker_ == 1) {
            platform().screen().fade(1.f, custom_color(0xffffff));
            g_.flicker_ = 2;
        } else if (g_.flicker_ == 2) {
            platform().screen().fade(0);
            g_.flicker_ = 0;
        }

        current_scene_->display();

        _platform->screen().display();
    }

}



void Engine::load(int chunk_x, int chunk_y)
{
    enemies_.clear();
    enemy_projectiles_.clear();
    generic_objects_.clear();
    player_projectiles_.clear();

    g_.shot_count_ = 0;

    room_.load(chunk_x, chunk_y);
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

        pf.set_tile(Layer::overlay, 30 - 5, y, 89);
        pf.set_tile(Layer::overlay, 30 - 4, y, 90);
        pf.set_tile(Layer::overlay, 30 - 3, y, 91);
    }

    Float hp_percent = g_.hp_ / Float(g_.max_hp_);
    int v_tiles = 18;
    int full_tiles = hp_percent * v_tiles;

    int remainder = (10 * hp_percent * v_tiles) - 10 * full_tiles;
    int rt = (remainder / 10.f) * 4;

    for (int y = 18; y > 18 - full_tiles; --y) {
        pf.set_tile(Layer::overlay, 3, y, 104);
        pf.set_tile(Layer::overlay, 4, y, 105);
    }

    if (rt) {
        pf.set_tile(Layer::overlay, 3, 18 - full_tiles, 98 + 2 * (rt - 1));
        pf.set_tile(Layer::overlay, 4, 18 - full_tiles, 99 + 2 * (rt - 1));
    }


    pf.set_tile(Layer::overlay, 2, 19, 86);
    pf.set_tile(Layer::overlay, 3, 19, 87);
    pf.set_tile(Layer::overlay, 4, 19, 88);

    pf.set_tile(Layer::overlay, 30 - 5, 19, 92);
    pf.set_tile(Layer::overlay, 30 - 4, 19, 93);
    pf.set_tile(Layer::overlay, 30 - 3, 19, 94);

    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 20; ++y) {
            pf.set_tile(Layer::overlay, x, y, 95);
            pf.set_tile(Layer::overlay, 29 - x, y, 95);
        }
    }
}



void Engine::Room::load(int chunk_x, int chunk_y)
{
    clear();

    const RoomData* rd;
    if (engine().g_.difficulty_ == Difficulty::hard) {
        rd = load_room_hard(chunk_x, chunk_y);
    } else {
        rd = load_room_normal(chunk_x, chunk_y);
    }

    if (rd) {
        for (int y = 0; y < 20; ++y) {
            for (int x = 0; x < 20; ++x) {
                walls_[x][y] =
                    rd->tiles_[y][x] == 1 or
                    rd->tiles_[y][x] == 6 or
                    rd->tiles_[y][x] == 7 or
                    rd->tiles_[y][x] == 8;

                platform().set_tile(Layer::map_0, x + 5, y,
                                        rd->tiles_[y][x]);

                if (walls_[x][y]) {

                } else if (x == 19) {
                    platform().set_tile(Layer::map_0, x + 5, y, 2);
                } else if (x == 0) {
                    platform().set_tile(Layer::map_0, x + 5, y, 3);
                } else if (y == 19) {
                    platform().set_tile(Layer::map_0, x + 5, y, 4);
                } else if (y == 0) {
                    platform().set_tile(Layer::map_0, x + 5, y, 5);
                }
            }
        }

        for (auto& obj : rd->objects_) {
            switch (obj.type_) {
            case 0: // null
                break;

            case 1:
                engine().add_object<Drone>(Vec2<Fixnum>{40 + obj.x_ * 8 - 4, obj.y_ * 8 - 4});
                break;

            case 2:
                engine().add_object<Reaver>(Vec2<Fixnum>{40 + obj.x_ * 8 - 4, obj.y_ * 8 - 4});
                break;

            case 3:
                engine().add_object<Spew>(Vec2<Fixnum>{40 + obj.x_ * 8 - 4, obj.y_ * 8 - 4});
                break;

            case 4:
                engine().add_object<Crusher>(Vec2<Fixnum>{40 + obj.x_ * 8 - 4, obj.y_ * 8 - 4});
                break;

            case 5:
                engine().add_object<Blomb>(Vec2<Fixnum>{40 + obj.x_ * 8 - 4, obj.y_ * 8});
                break;

            case 6:
                engine().add_object<Bolt>(Vec2<Fixnum>{40 + obj.x_ * 8 - 4, obj.y_ * 8 - 4});
                break;

            default:
                break;
            }
        }
    }

    coord_ = {chunk_x, chunk_y};
}



}
