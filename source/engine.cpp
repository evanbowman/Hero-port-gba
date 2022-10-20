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
#include "objects/enemies/heavy/phaze.hpp"
#include "objects/enemies/heavy/battleDoor.hpp"
#include "objects/enemies/heavy/chainsnake.hpp"
#include "objects/enemies/elite/generator.hpp"
#include "objects/enemies/elite/barrier.hpp"
#include "objects/enemies/elite/mode.hpp"
#include "objects/enemies/elite/hunter.hpp"
#include "objects/enemies/elite/eidolon.hpp"
#include "objects/enemies/elite/direviper.hpp"
#include "objects/enemies/elite/annihilator.hpp"
#include "objects/enemies/boss/reaperDrone.hpp"
#include "objects/enemies/boss/guardian.hpp"
#include "objects/enemies/boss/silencer.hpp"
#include "objects/enemies/boss/hydra.hpp"
#include "objects/enemies/boss/rockSmasher.hpp"
#include "objects/enemies/boss/elite.hpp"
#include "objects/enemies/boss/eliminator.hpp"
#include "objects/misc/savepoint.hpp"
#include "objects/particles/weed.hpp"
#include "objects/particles/star.hpp"
#include "maps.hpp"
#include "scene.hpp"
#include "scenes.hpp"
#include "objects/particles/flame.hpp"



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
    hero_(alloc_object<Hero>(Vec2<Fixnum>{110, 70})),
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
    if (g_.current_music_ not_eq g_.checkpoint_music_) {
        g_.current_music_ = g_.checkpoint_music_;
        platform().speaker().play_music(g_.current_music_, 0);
    }

    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            // NOTE: I'm using layer 1 for large bosses, clear out the layer.
            platform().set_tile(Layer::map_1, x, y, 0);
        }
    }


    room_.clear();
    hero_->set_position(Vec2<Fixnum>{40 + g_.checkpoint_coords_.x,
                                         g_.checkpoint_coords_.y});

    g_.hp_ = g_.max_hp_;
    g_.invulnerable_ = 0;
    g_.shot_count_ = 0;
    g_.autofire_ = false;

    load(g_.checkpoint_room_.x, g_.checkpoint_room_.y, true);
    draw_hud();
}



void Engine::begin_game(Difficulty d)
{
    room_.clear();

    hero_->set_position(Vec2<Fixnum>{110, 80});
    g_.checkpoint_coords_.x = 50;
    g_.checkpoint_coords_.y = 70;

    g_.hp_ = g_.max_hp_;
    g_.invulnerable_ = 0;
    g_.shot_count_ = 0;
    g_.autofire_ = false;

    if (d == Difficulty::hard) {
        g_.checkpoint_room_ = {6, 0};
        load(6, 0, false);
        // load(11, 6, false); // hydra
        // load(13, 8, false); // silencer
        // load(0, 6, false); // annihilator
        // load(9, 8, false); // chain snake
        // load(9, 12, false); // eidolon
        // load(10, 2, false); // hunter
        // load(11, 1, false); // mode
        // load(9, 6, false); // battle door
        // load(1, 4, false); // reaper drone
    } else {
        g_.checkpoint_room_ = {11, 14};
        load(11, 14, false);
        // load(11, 4, false); // silencer
        // load(9, 6, false); // reaper drone
        // load(11, 0, false); // hydra
        // load(9, 9, false); // rock smasher
        // load(5, 13, false); // elite
        // load(1, 6, false); // annihilator
    }
}



void Engine::collision_check()
{
    if (paused_) {
        return;
    }

    for (auto& p : enemy_projectiles_) {
        if (p->hitbox().overlapping(hero_->hitbox())) {
            if (not g_.invulnerable_) {
                if (p->force()) {
                    p->on_hero_collision();
                    g_.damage(p->force(), 30);
                    g_.heat_ += p->heat();
                    g_.heat_ = std::min(g_.heat_, g_.maxheat_);
                    draw_hud();
                }

            }
        }
    }

    for (auto& p : player_projectiles_) {
        for (auto& e : enemies_) {
            if (p->hitbox().overlapping(e->hitbox())) {
                if (e->damage(p->damage(), *p)) {
                    add_object<Explo>(Vec2<Fixnum>{
                            p->x() - 2, p->y() - 2
                        });
                }
                if (p->dead()) {
                    break;
                }
            }
        }
    }

    for (auto& e : enemies_) {
        if (e->hitbox().overlapping(hero_->hitbox())) {
            if (not g_.invulnerable_) {
                auto dmg = e->collision_damage();
                if (dmg) {
                    g_.damage(dmg,
                              e->collision_damage_extra_invulnerable_time());
                    draw_hud();
                }
            }
        }
    }

    // Check internal collisions between objects, record a collision flag in the
    // objects themselves. Some objects check collisions with same-type objects
    // to prevent bunching up.
    for (int i = 0; i < (int)TaggedObject::Tag::count; ++i) {
        auto tag = (TaggedObject::Tag)i;
        auto l1 = TaggedObject::get_taglist(tag);
        while (l1) {
            if (not l1->collision()) {
                auto l2 = TaggedObject::get_taglist(tag);
                while (l2) {
                    if (l1->hitbox().overlapping(l2->hitbox())) {
                        l1->set_collision();
                        break;
                    }
                    l2 = l2->next();
                }
            }
            l1 = l1->next();
        }
    }
}



void Engine::run()
{
    while (true) {

        if (frame_count_ == 2) {
            frame_count_ = 0;

            // NOTE: The original game runs at 40fps. I'm using the downtime
            // between frames to run collision checking.
            collision_check();

        } else {

            _platform->keyboard().poll();

            if (next_scene_) {
                next_scene_->enter(*current_scene_);
                current_scene_ = std::move(next_scene_);
            }

            if (key_down<Key::alt_2>()) {
                g_.autofire_ = not g_.autofire_;
            }

            hero_->step();

            if (g_.summon_eliminator_tics_) {
                if (g_.summon_eliminator_tics_-- == 1) {
                    summon_eliminator();
                }
            }

            const auto hpos = hero_->position();
            if (hpos.x > Fixnum(196) and room_.has_exit_right()) {
                load(room_.coord_.x + 1, room_.coord_.y, false);
                hero_->set_position({50, hero_->position().y});
                continue;
            } else if (hpos.x < Fixnum(41) and room_.has_exit_left()) {
                load(room_.coord_.x - 1, room_.coord_.y, false);
                hero_->set_position({186, hero_->position().y});
                continue;
            } else if (hpos.y > Fixnum(154) and room_.has_exit_down()) {
                load(room_.coord_.x, room_.coord_.y + 1, false);
                hero_->set_position({hero_->position().x, 10});
                continue;
            } else if (hpos.y < 2 and room_.has_exit_up()) {
                load(room_.coord_.x, room_.coord_.y - 1, false);
                hero_->set_position({hero_->position().x, 144});
                continue;
            }

            next_scene_ = current_scene_->step();

            if (next_scene_) {
                current_scene_->exit(*next_scene_);
            }

            ++frame_count_;

            if (not paused_) {
                for (auto& l : lm_) {
                    if (l->hitbox().overlapping(hero_->hitbox())) {
                        if (g_.heat_ < g_.maxheat_) {
                            g_.heat_ += 3;
                            g_.heat_ = std::min(g_.heat_, g_.maxheat_);
                            draw_hud_heat();
                            break;
                        }
                    }
                }
            }
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



void Engine::summon_eliminator()
{
    bool can_summon_eliminator = true;
    for (auto& e : enemies_) {
        if (dynamic_cast<Generator*>(e.get()) or
            dynamic_cast<Barrier*>(e.get())) {
            can_summon_eliminator = false;
            break;
        }
    }
    for (auto& s : generic_solids_) {
        if (dynamic_cast<Savepoint*>(s.get())) {
            can_summon_eliminator = false;
            break;
        }
    }

    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 20; ++y) {
            auto t = platform().get_tile(Layer::map_0, x + 5, y);
            if (t == 33 or t == 34 or t == 35 or t == 36 or
                t == 37 or t == 38 or t == 39 or t == 40) {
                // Doors in room
                can_summon_eliminator = false;
            }
        }
    }

    if (p_->eliminator_defeated_) {
        can_summon_eliminator = false;
    }

    int temprand = 0;
    for (int i = 0; i < 10; ++i) {
        temprand += rng::choice<100>(rng::critical_state);
    }
    if (temprand > 20 + 5 * p_->level_) {
        can_summon_eliminator = false;
    }

    if (can_summon_eliminator) {
        enemies_.clear();
        platform().speaker().play_sound("snd_explo2", 1);
        engine().add_object<Eliminator>(Vec2<Fixnum>{80 + 5 * 8 - 1, 80});
        engine().add_object<GigaExplo>(Vec2<Fixnum>{80 + 5 * 8 - 8, 80 - 5});

        for (int i = 0; i < 20; ++i) {
            auto mkobj =
                [&](int xo, int yo) {
                    Vec2<Fixnum> p{5 * 8 + 80 + xo - 1,
                                   80 + yo};
                    if (auto f = engine().add_object<Flame>(p)) {
                        f->priority_ = 0;
                        f->counter_ = 10 + rng::choice(2 * (20 - i),
                                                       rng::critical_state);
                    }
                };
            mkobj(-i * 2.6f, -i * 2.6f);
            mkobj(i * 2.6f, i * 2.6f);
            mkobj(i * 2.6f, -i * 2.6f);
            mkobj(-i * 2.6f, i * 2.6f);
        }
    }
}



void Engine::unlock_doors()
{
    bool changed = false;
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
                changed = true;
            }
        }
    }

    if (not changed) {
        return;
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
                if (t == 0 or t == 1) {
                    // Nothing (most tiles)
                } else if (t == 16) {
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
                if (t == 0 or t == 1) {
                    // Nothing (most tiles)
                } else if (t == 44) {
                    t = 41;
                } else if (t == 48) {
                    t = 45;
                } else if (t >= 41 and t < 48) {
                    t++;
                } else if (t == 35) {
                    t = 33;
                } else if (t == 36) {
                    t = 34;
                } else if (t == 33) {
                    t = 35;
                } else if (t == 34) {
                    t = 36;
                } else if (t == 37) {
                    platform().fatal("here");
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



void Engine::load(int chunk_x, int chunk_y, bool restore)
{
    enemies_.clear();
    enemy_projectiles_.clear();
    generic_objects_.clear();
    generic_solids_.clear();
    player_projectiles_.clear();
    lm_.clear();

    g_.shot_count_ = 0;

    platform().sleep(1);
    room_.load(chunk_x, chunk_y, restore);

    if (not restore and
        not is_boss_level(g_.difficulty_, {u8(chunk_x), u8(chunk_y)})) {
        g_.summon_eliminator_tics_ = 20;
    }

    p_->visited_.set(chunk_x, chunk_y, true);
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
    }

    pf.set_tile(Layer::overlay, 2, 0, 119);
    pf.set_tile(Layer::overlay, 3, 0, 120);
    pf.set_tile(Layer::overlay, 4, 0, 121);

    pf.set_tile(Layer::overlay, 25, 0, 122);
    pf.set_tile(Layer::overlay, 26, 0, 123);
    pf.set_tile(Layer::overlay, 27, 0, 124);

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

    draw_hud_heat();

    draw_image(platform(),
               173,
               0,
               0,
               2,
               2,
               Layer::overlay);
}



void Engine::draw_hud_heat()
{
    auto& pf = platform();

    for (int y = 1; y < 19; ++y) {
        pf.set_tile(Layer::overlay, 30 - 5, y, 89);
        pf.set_tile(Layer::overlay, 30 - 4, y, 90);
        pf.set_tile(Layer::overlay, 30 - 3, y, 91);
    }

    if (g_.heat_ == 0) {
        return;
    }

    Float heat_percent = g_.heat_ / Float(g_.maxheat_);

    platform().screen().fade(0.2f * heat_percent, ColorConstant::silver_white);

    int v_tiles = 18;
    int full_tiles = heat_percent * v_tiles;

    int remainder = (10 * heat_percent * v_tiles) - 10 * full_tiles;
    int rt = (remainder / 10.f) * 4;

    for (int y = 18; y > 18 - full_tiles; --y) {
        pf.set_tile(Layer::overlay, 30 - 5, y, 114);
        pf.set_tile(Layer::overlay, 30 - 4, y, 115);
    }

    if (rt) {
        pf.set_tile(Layer::overlay, 30 - 5, 18 - full_tiles, 108 + 2 * (rt - 1));
        pf.set_tile(Layer::overlay, 30 - 4, 18 - full_tiles, 109 + 2 * (rt - 1));
    }


}



void Engine::Room::render_entrances()
{
    // Redraw room entrances
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 20; ++x) {
            if (platform().get_tile(Layer::map_0, x + 5, y)) {

            } else if (x == 19 and has_exit_right()) {
                platform().set_tile(Layer::map_0, x + 5, y, 2);
            } else if (x == 0 and has_exit_left()) {
                platform().set_tile(Layer::map_0, x + 5, y, 3);
            } else if (y == 19 and has_exit_down()) {
                platform().set_tile(Layer::map_0, x + 5, y, 4);
            } else if (y == 0 and has_exit_up()) {
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
            if (not (x + x_off > 0 and x + x_off < 15 and
                     y + y_off > 0 and y + y_off < 15)) {
                return;
            }
            if (auto ch = get_chunk(x + x_off, y + y_off)) {
                for (auto& obj : ch->objects_) {
                    if (obj.type_ == 18 or obj.type_ == 19) {
                        engine().p_->objects_removed_.push_back({
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



void Engine::Room::load(int chunk_x, int chunk_y, bool restore)
{
    clear();

    const RoomData* rd;
    if (engine().g_.difficulty_ == Difficulty::hard) {
        rd = load_room_hard(chunk_x, chunk_y);
    } else {
        rd = load_room_normal(chunk_x, chunk_y);
    }

    if (rd) {

        if (rd->zone_ and rd->zone_ not_eq zone_) {
            zone_ = rd->zone_;
            const char* current_music = engine().g_.current_music_;
            switch (zone_) {
            case 1:
                current_music = "zone1";
                platform().speaker().play_music(current_music, 0);
                break;

            case 2:
                current_music = "zone2";
                platform().speaker().play_music(current_music, 0);
                break;

            case 3:
                current_music = "zone3";
                platform().speaker().play_music(current_music, 0);
                break;

            case 4:
                current_music = "zone4";
                platform().speaker().play_music(current_music, 0);
                break;

            case 5:
                current_music = "zone5";
                platform().speaker().play_music(current_music, 0);
                break;

            case 6:
                current_music = "zone6";
                platform().speaker().play_music(current_music, 0);
                break;

            case 7:
                current_music = "zone7";
                platform().speaker().play_music(current_music, 0);
                break;

            case 8:
                current_music = "zone8";
                platform().speaker().play_music(current_music, 0);
                break;

            case 9:
                current_music = "zone9";
                platform().speaker().play_music(current_music, 0);
                break;

            case 10:
                current_music = "zone10";
                platform().speaker().play_music(current_music, 0);
                break;
            }
            engine().g_.current_music_ = current_music;
        }

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
            for (auto& rem : engine().p_->objects_removed_) {
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

            // NOTE: see encode.py for object type integers. I wrote a python
            // script to scrape objects from the game map, and I assigned
            // integers to each type of object. If I had more time, I might try
            // to clean this code up.
            switch (obj.type_) {
            case 0: // null
                break;

            case 1:
                engine().add_object<Drone>(Vec2<Fixnum>{40 + obj.x_ - 4, obj.y_ - 4});
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
                engine().add_object<Weed>(Vec2<Fixnum>{40 + obj.x_, obj.y_}, false);
                break;

            case 16:
                engine().add_object<Weed>(Vec2<Fixnum>{40 + obj.x_, obj.y_}, true);
                break;

            case 17:
                engine().add_object<Generator>(Vec2<Fixnum>{40 + obj.x_ * 8 - 5,
                                                                obj.y_ * 8 - 5},
                    obj.x_,
                    obj.y_);
                break;

            case 18:
                engine().add_object<Barrier>(Vec2<Fixnum>{40 + obj.x_ * 8, obj.y_ * 8}, false);
                break;

            case 19:
                engine().add_object<Barrier>(Vec2<Fixnum>{40 + obj.x_ * 8, obj.y_ * 8}, true);
                break;

            case 20:
                engine().add_object<LiquidMetal>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                 Hitbox::Dimension{8, 32, 0, 0});
                break;

            case 21:
                engine().add_object<LiquidMetal>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                 Hitbox::Dimension{32, 8, 0, 0});
                break;

            case 22:
                engine().add_object<LiquidMetal>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                 Hitbox::Dimension{24, 24, 0, 0});
                break;

            case 23:
                engine().add_object<LiquidMetal>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                 Hitbox::Dimension{8, 8, 0, 0});
                break;

            case 24:
                engine().add_object<Savepoint>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                               not restore);
                break;

            case 25:
                engine().add_object<ReaperDrone>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                 obj.x_,
                                                 obj.y_);
                break;

            case 26:
                engine().add_object<Star>(Vec2<Fixnum>{40 + obj.x_, obj.y_});
                break;

            case 27:
                engine().add_object<Phaze>(Vec2<Fixnum>{40 + obj.x_, obj.y_});
                break;

            case 28:
                engine().add_object<BattleDoor>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                obj.x_,
                                                obj.y_);
                break;

            case 29:
                engine().add_object<Mode>(Vec2<Fixnum>{40 + obj.x_ - 8, obj.y_ - 8});
                break;

            case 30:
                engine().add_object<Hunter>(Vec2<Fixnum>{40 + obj.x_ - 8, obj.y_ - 8});
                break;

            case 31:
                engine().add_object<Eidolon>(Vec2<Fixnum>{40 + obj.x_, obj.y_});
                break;

            case 32:
                engine().add_object<Chainsnake>(Vec2<Fixnum>{40 + obj.x_, obj.y_});
                break;

            case 33:
                engine().add_object<Direviper>(Vec2<Fixnum>{40 + obj.x_, obj.y_});
                break;

            case 34:
                engine().add_object<Annihilator>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                 obj.x_,
                                                 obj.y_);
                break;

            case 35:
                engine().add_object<Silencer>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                              obj.x_,
                                              obj.y_);
                break;

            case 36:
                engine().add_object<Hydra>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                           obj.x_,
                                           obj.y_);
                break;

            case 37:
                engine().add_object<RockSmasher>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                 obj.x_,
                                                 obj.y_);
                break;

            case 38:
                engine().add_object<Elite>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                           obj.x_,
                                           obj.y_);
                break;

            case 39:
                engine().add_object<GuardianCore>(Vec2<Fixnum>{40 + obj.x_, obj.y_},
                                                  obj.x_,
                                                  obj.y_);
                break;

            case 40:
                engine().add_object<HunterDormant>(Vec2<Fixnum>{40 + obj.x_, obj.y_});
                break;

            case 41:
                engine().add_object<SoldierDormant>(Vec2<Fixnum>{40 + obj.x_, obj.y_});
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



bool Engine::Room::has_exit_up() const
{
    if (coord_.y == 0) {
        return false;
    }


    if (engine().g_.difficulty_ == Difficulty::normal) {
        if (coord_ == Vec2<int>{11, 14}) {
            return false;
        }
    }


    return true;
}



bool Engine::Room::has_exit_down() const
{
    if (coord_.y == 14) {
        return false;
    }

    return true;
}



bool Engine::Room::has_exit_left() const
{
    if (coord_.x == 0) {
        return false;
    }

    if (engine().g_.difficulty_ == Difficulty::hard) {
        // FIXME!!! Find some other way to detect whether the player is
        // outdoors? I don't know yet how the gamemaker code does this. For now,
        // there are only a few rooms where you're outside of the asteroid, so
        // I'm just hard-coding them. The gamemaker code puts the whole map in a
        // single room, and it's unclear to me yet how the game determines
        // whether a room is "outdoors".
        if (coord_ == Vec2<int>{6, 0}) {
            return false;
        }
    }

    return true;
}



bool Engine::Room::has_exit_right() const
{
    if (coord_.x == 14) {
        return false;
    }

    if (engine().g_.difficulty_ == Difficulty::normal) {
        if (coord_ == Vec2<int>{11, 14}) {
            return false;
        }
    }

    return true;
}



}
