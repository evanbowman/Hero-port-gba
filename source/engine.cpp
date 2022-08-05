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
#include "objects/enemies/light/lurk.hpp"
#include "objects/enemies/heavy/spawner.hpp"
#include "objects/enemies/heavy/powerTurret.hpp"
#include "objects/enemies/heavy/boltaray.hpp"
#include "objects/enemies/heavy/soldier.hpp"
#include "objects/enemies/elite/generator.hpp"
#include "objects/enemies/elite/barrier.hpp"
#include "objects/particles/weed.hpp"
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
    hero_(alloc_object<Hero>(Vec2<Fixnum>{90, 80})),
    p_(allocate_dynamic<Persistence>(pf)),
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

    begin_game(g_.difficulty_);

    draw_hud();
}



void Engine::respawn_to_checkpoint()
{
    // TODO: implement checkpoints!

    begin_game(g_.difficulty_);
    draw_hud();
}



void Engine::begin_game(Difficulty d)
{
    room_.clear();

    hero_->set_position(Vec2<Fixnum>{90, 80});

    g_.hp_ = g_.max_hp_;
    g_.invulnerable_ = 0;
    g_.shot_count_ = 0;
    g_.autofire_ = false;

    if (d == Difficulty::hard) {
        load(6, 0);
    } else {
        load(11, 14);
    }
}



void Engine::collision_check()
{
    for (auto& p : enemy_projectiles_) {
        if (p->hitbox().overlapping(hero_->hitbox())) {
            if (p->force()) {
                p->on_hero_collision();
                g_.damage(p->force(), 30);
                draw_hud();
            }
        }
    }

    for (auto& p : player_projectiles_) {
        for (auto& e : enemies_) {
            if (p->hitbox().overlapping(e->hitbox())) {
                p->kill();
                e->damage(1, *p);
                break;
            }
        }
    }

    for (auto& e : enemies_) {
        if (e->hitbox().overlapping(hero_->hitbox())) {
            g_.damage(e->collision_damage(),
                      e->collision_damage_extra_invulnerable_time());
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
                hero_->set_position({186, hero_->position().y});
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

        animate_tiles();

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



void Engine::unlock_doors()
{
    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 20; ++y) {
            auto t = platform().get_tile(Layer::map_0, x + 5, y);
            if (t == 33 or t == 34 or t == 35 or t == 36 or
                t == 37 or t == 38 or t == 39 or t == 40) {
                p_->tile_modifications_.push_back({(u8)x,
                                                   (u8)y,
                                                   (u8)room_.coord_.x,
                                                   (u8)room_.coord_.y,
                                                   0
                    });
                platform().set_tile(Layer::map_0, x + 5, y, 0);
                room_.walls_[x][y] = false;
            }
        }
    }

    room_.render_entrances();
}



void Engine::animate_tiles()
{
    animcyc_ = animcyc_ + 1;
    if (animcyc_ == 5) {
        animcyc_ = 0;
    }

    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 20; ++y) {
            auto t = platform().get_tile(Layer::map_0, x + 5, y);
            if (frame_count_ == 0) {
                if (t == 16) {
                    t = 9;
                } else if (t == 24) {
                    t = 17;
                } else if (t == 32) {
                    t = 25;
                } else if (t >= 9 and t < 32) {
                    t = t + 1;
                }
            }

            if (animcyc_ == 4) {
                if (t == 35) {
                    t = 33;
                } else if (t == 36) {
                    t = 34;
                } else if (t == 33) {
                    t = 35;
                } else if (t == 34) {
                    t = 36;
                } else if (t == 37) {
                    t = 39;
                } else if (t == 38) {
                    t = 40;
                } else if (t == 39) {
                    t = 37;
                } else if (t == 40) {
                    t = 38;
                }
            }
            platform().set_tile(Layer::map_0, x + 5, y, t);
        }
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



void Engine::Room::render_entrances()
{
    // Redraw room entrances
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 20; ++x) {
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
}



void Engine::Room::clear_adjacent_barriers()
{
    auto get_chunk = [&](int chunk_x, int chunk_y) {
                         const RoomData* rd;
                         if (engine().g_.difficulty_ == Difficulty::hard) {
                             rd = load_room_hard(chunk_x, chunk_y);
                         } else {
                             rd = load_room_normal(chunk_x, chunk_y);
                             return rd;
                         }
                         return rd;
                     };

    auto x = coord_.x;
    auto y = coord_.y;

    auto barrier_clear =
        [&](int x_off, int y_off) {
            if (not (x + x_off > 0 and x + x_off < 15 and y + y_off > 0 and y + y_off < 15)) {
                return;
            }
            if (auto ch = get_chunk(x + x_off, y + y_off)) {
                for (auto& obj : ch->objects_) {
                    if (obj.type_ == 18) {
                        engine().p_->object_modifications_.push_back({
                                (u8)(x + x_off),
                                (u8)(y + y_off),
                                obj.x_,
                                obj.y_
                            });
                    }
                }
            }
        };

    barrier_clear(0, 0);
    barrier_clear(-1, 0);
    barrier_clear(1, 0);
    barrier_clear(0, 1);
    barrier_clear(0, -1);
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
                    rd->tiles_[y][x] == 8 or
                    rd->tiles_[y][x] == 33 or
                    rd->tiles_[y][x] == 34 or
                    rd->tiles_[y][x] == 37 or
                    rd->tiles_[y][x] == 38;

                platform().set_tile(Layer::map_0, x + 5, y,
                                        rd->tiles_[y][x]);
            }
        }

        for (auto& obj : rd->objects_) {

            bool removed = false;
            for (auto& rem : engine().p_->object_modifications_) {
                if (rem.room_x_ == chunk_x and
                    rem.room_y_ == chunk_y and
                    rem.x_ == obj.x_ and
                    rem.y_ == obj.y_) {
                    removed = true;
                }
            }

            if (removed) {
                continue;
            }

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

            case 8:
                if (auto t = engine().add_object<Turret>(Vec2<Fixnum>{40 + obj.x_ * 8 + 8, obj.y_ * 8})) {
                    t->set_left();
                }
                break;

            case 7:
                engine().add_object<Turret>(Vec2<Fixnum>{40 + obj.x_ * 8, obj.y_ * 8});
                break;

            case 9:
                engine().add_object<Boltaray>(Vec2<Fixnum>{40 + obj.x_ * 8 - 4, obj.y_ * 8 - 4});
                break;

            case 10:
                engine().add_object<Spawner>(Vec2<Fixnum>{40 + obj.x_ * 8 - 8, obj.y_ * 8 - 4});
                break;

            case 11:
                engine().add_object<Soldier>(Vec2<Fixnum>{40 + obj.x_ * 8 - 4, obj.y_ * 8 - 4});
                break;

            case 13:
                if (auto t = engine().add_object<PowerTurret>(Vec2<Fixnum>{40 + obj.x_ * 8 + 8, obj.y_ * 8})) {
                    t->set_left();
                }
                break;

            case 12:
                engine().add_object<PowerTurret>(Vec2<Fixnum>{40 + obj.x_ * 8, obj.y_ * 8});
                break;

            case 14:
                engine().add_object<Lurk>(Vec2<Fixnum>{40 + obj.x_ * 8 + 1, obj.y_ * 8});
                break;

            case 15:
                engine().add_object<Weed>(Vec2<Fixnum>{40 + obj.x_, obj.y_}, true);
                break;

            case 16:
                engine().add_object<Weed>(Vec2<Fixnum>{40 + obj.x_, obj.y_}, false);
                break;

            case 17:
                engine().add_object<Generator>(Vec2<Fixnum>{40 + obj.x_ * 8 - 5, obj.y_ * 8 - 5});
                break;

            case 18:
                engine().add_object<Barrier>(Vec2<Fixnum>{40 + obj.x_ * 8, obj.y_ * 8}, false);
                break;

            default:
                break;
            }
        }
    }

    coord_ = {chunk_x, chunk_y};

    for (auto& m : engine().p_->tile_modifications_) {
        if (m.room_x_ == chunk_x and m.room_y_ == chunk_y) {
            platform().set_tile(Layer::map_0, m.tx_ + 5, m.ty_, m.new_value_);

            auto v = m.new_value_;

            walls_[m.tx_][m.ty_] = v == 1 or v == 6 or v == 7 or v == 8;
        }
    }

    render_entrances();
}



}
