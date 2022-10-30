#include "scenes.hpp"
#include "maps.hpp"
#include "graphics/overlay.hpp"


namespace herocore
{


static const std::array<Vec2<u8>, 10> hint_order_normal = {
    {{11, 4}, {11, 0}, {9, 6}, {0, 3}, {9, 9}, // {8, 1},
     {1, 6}, // {6, 9},
     {5, 13}, {7, 7}}};


static const std::array<Vec2<u8>, 10> hint_order_hard = {{{13, 8},
                                                          {11, 6},
                                                          {1, 4},
                                                          {8, 13},
                                                          {7, 4},
                                                          {5, 14},
                                                          {14, 11},
                                                          {0, 11},
                                                          {9, 1},
                                                          {7, 7}}};


bool is_boss_level(Difficulty d, Vec2<u8> room)
{
    if (d == Difficulty::normal) {
        for (auto& r : hint_order_normal) {
            if (r == room) {
                return true;
            }
        }
    } else {
        for (auto& r : hint_order_hard) {
            if (r == room) {
                return true;
            }
        }
    }
    return false;
}


void english__to_string(int num, char* buffer, int base)
{
    int i = 0;
    bool is_negative = false;

    if (num == 0) {
        buffer[i++] = '0';
        buffer[i] = '\0';
        return;
    }

    // Based on the behavior of itoa()
    if (num < 0 && base == 10) {
        is_negative = true;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        buffer[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    str_reverse(buffer, i);

    return;
}


void MapScene::show_worldmap()
{
    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 20; ++y) {
            platform().set_tile(Layer::overlay, x + 5, y, 95);
        }
    }

    const bool hard = engine().g_.difficulty_ == Difficulty::hard;

    auto loadrm = [&](int x, int y) {
        auto data = load_room_normal(x, y);
        if (hard) {
            data = load_room_hard(x, y);
        }
        return data;
    };

    // This warrants some explanation. Daniel created the map as one single room
    // in gamemaker. I wrote a script to extract the map and split it into
    // chunks. During runtime, we need to look into the map chunks and determine
    // which ones are linked, by checking edges.

    u8 margin = 1;
    u8 pad_x = 1;

    for (int x = 0; x < 15; ++x) {
        for (int y = 0; y < 15; ++y) {

            if ((x < 3 and y < 3) or (x > 11 and y < 3) or (x < 3 and y > 11) or
                (x > 11 and y > 11)) {
                continue;
            }

            if (not engine().p_->visited_.get(x, y)) {
                platform().set_tile(
                    Layer::overlay, x + 5 + margin + pad_x, y + margin, 125);
                continue;
            }

            auto data = loadrm(x, y);

            bool up = false;
            bool down = false;
            bool left = false;
            bool right = false;

            if (y > 0 and data) {
                auto above = loadrm(x, y - 1);
                if (above) {
                    for (int xx = 1; xx < 19; ++xx) {
                        if (data->tiles_[0][xx] not_eq 1 and
                            above->tiles_[19][xx] not_eq 1) {
                            up = true;
                            break;
                        }
                    }
                }
            }

            if (y < 14 and data) {
                auto below = loadrm(x, y + 1);
                if (below) {
                    for (int xx = 1; xx < 19; ++xx) {
                        if (data->tiles_[19][xx] not_eq 1 and
                            below->tiles_[0][xx] not_eq 1) {
                            down = true;
                            break;
                        }
                    }
                }
            }

            if (x > 0 and data) {
                auto rleft = loadrm(x - 1, y);
                if (rleft) {
                    for (int yy = 1; yy < 19; ++yy) {
                        if (data->tiles_[yy][0] not_eq 1 and
                            rleft->tiles_[yy][19] not_eq 1) {
                            left = true;
                            break;
                        }
                    }
                }
            }

            if (x < 14 and data) {
                auto rright = loadrm(x + 1, y);
                if (rright) {
                    for (int yy = 1; yy < 19; ++yy) {
                        if (data->tiles_[yy][19] not_eq 1 and
                            rright->tiles_[yy][0] not_eq 1) {
                            right = true;
                            break;
                        }
                    }
                }
            }


            int t = 0;

            // copy-pasted from the original game's map rendering code. I
            // wouldn't have done it this way. But I'm also to lazy to change
            // it. Fyi: in this situation, you can make your life way easier
            // with bitmasking.
            if (right && !up && !left && !down)
                t = 1;
            else if (!right && up && !left && !down)
                t = 2;
            else if (!right && !up && left && !down)
                t = 3;
            else if (!right && !up && !left && down)
                t = 4;
            else if (right && up && !left && !down)
                t = 5;
            else if (!right && up && left && !down)
                t = 6;
            else if (!right && !up && left && down)
                t = 7;
            else if (right && !up && !left && down)
                t = 8;
            else if (right && !up && left && !down)
                t = 9;
            else if (!right && up && !left && down)
                t = 10;
            else if (right && up && !left && down)
                t = 11;
            else if (right && up && left && !down)
                t = 12;
            else if (!right && up && left && down)
                t = 13;
            else if (right && !up && left && down)
                t = 14;
            else if (right && up && left && down)
                t = 15;
            else if (!right && !up && !left && !down)
                t = 16;

            if (engine().room_.coord_.x == x and engine().room_.coord_.y == y) {
                t += 16;
                pt_ = {x + 5 + margin + pad_x, y + margin};
            } else {
                for (auto& obj : data->objects_) {
                    if (obj.type_ == 24) { // savepoint
                        t += 90;
                        warp_points_.push_back({(u8)x, (u8)y});
                        break;
                    }
                }
            }

            if (left or right or up or down) {

                platform().set_tile(
                    Layer::overlay, x + 5 + margin + pad_x, y + margin, 126 + t);
            }
        }
    }

    if (hard) {
        for (auto& h : hint_order_hard) {
            bool found = false;
            for (auto& b : engine().p_->completed_bosses_) {
                if (b == h) {
                    found = true;
                    break;
                }
            }
            if (not found) {
                platform().set_tile(
                    Layer::overlay, h.x + 5 + margin + pad_x, h.y + margin, 158);
                break;
            }
        }
    } else {
        for (auto& h : hint_order_normal) {
            bool found = false;
            for (auto& b : engine().p_->completed_bosses_) {
                if (b == h) {
                    found = true;
                    break;
                }
            }
            if (not found) {
                platform().set_tile(
                    Layer::overlay, h.x + 5 + margin + pad_x, h.y + margin, 158);
                break;
            }
        }
    }


    draw_image(platform(), 159, 6, 17, 3, 2, Layer::overlay);

    platform().set_tile(Layer::overlay, 5 + pad_x, 0, 165);
    platform().set_tile(Layer::overlay, 5 + pad_x, 16, 167);
    platform().set_tile(Layer::overlay, 5 + pad_x + 16, 16, 169);
    platform().set_tile(Layer::overlay, 5 + pad_x + 16, 0, 171);

    for (int y = 1; y < 16; ++y) {
        platform().set_tile(Layer::overlay, 5 + pad_x, y, 166);
        platform().set_tile(Layer::overlay, 5 + pad_x + 16, y, 170);
        platform().set_tile(Layer::overlay, 5 + pad_x + y, 16, 168);
        platform().set_tile(Layer::overlay, 5 + pad_x + y, 0, 172);
    }
}


void MapScene::show_zoomedmap()
{
    for (int x = 0; x < 20; ++x) {
        for (int y = 0; y < 20; ++y) {
            platform().set_tile(Layer::overlay, x + 5, y, 95);
        }
    }

    const bool hard = engine().g_.difficulty_ == Difficulty::hard;

    auto loadrm = [&](int x, int y) {
        auto data = load_room_normal(x, y);
        if (hard) {
            data = load_room_hard(x, y);
        }
        return data;
    };

    // This warrants some explanation. Daniel created the map as one single room
    // in gamemaker. I wrote a script to extract the map and split it into
    // chunks. During runtime, we need to look into the map chunks and determine
    // which ones are linked, by checking edges.

    u8 margin = 0;

    for (int x = 0; x < 15; ++x) {
        for (int y = 0; y < 15; ++y) {

            if ((x < 3 and y < 3) or (x > 11 and y < 3) or (x < 3 and y > 11) or
                (x > 11 and y > 11)) {
                continue;
            }

            if (not engine().p_->visited_.get(x, y)) {
                platform().set_tile(Layer::overlay, x + 5 + margin, y + margin, 125);
                continue;
            }

            auto data = loadrm(x, y);

            bool up = false;
            bool down = false;
            bool left = false;
            bool right = false;

            if (y > 0 and data) {
                auto above = loadrm(x, y - 1);
                if (above) {
                    for (int xx = 1; xx < 19; ++xx) {
                        if (data->tiles_[0][xx] not_eq 1 and
                            above->tiles_[19][xx] not_eq 1) {
                            up = true;
                            break;
                        }
                    }
                }
            }

            if (y < 14 and data) {
                auto below = loadrm(x, y + 1);
                if (below) {
                    for (int xx = 1; xx < 19; ++xx) {
                        if (data->tiles_[19][xx] not_eq 1 and
                            below->tiles_[0][xx] not_eq 1) {
                            down = true;
                            break;
                        }
                    }
                }
            }

            if (x > 0 and data) {
                auto rleft = loadrm(x - 1, y);
                if (rleft) {
                    for (int yy = 1; yy < 19; ++yy) {
                        if (data->tiles_[yy][0] not_eq 1 and
                            rleft->tiles_[yy][19] not_eq 1) {
                            left = true;
                            break;
                        }
                    }
                }
            }

            if (x < 14 and data) {
                auto rright = loadrm(x + 1, y);
                if (rright) {
                    for (int yy = 1; yy < 19; ++yy) {
                        if (data->tiles_[yy][19] not_eq 1 and
                            rright->tiles_[yy][0] not_eq 1) {
                            right = true;
                            break;
                        }
                    }
                }
            }


            int t = 0;

            // copy-pasted from the original game's map rendering code. I
            // wouldn't have done it this way. But I'm also to lazy to change
            // it. Fyi: in this situation, you can make your life way easier
            // with bitmasking.
            if (right && !up && !left && !down)
                t = 1;
            else if (!right && up && !left && !down)
                t = 2;
            else if (!right && !up && left && !down)
                t = 3;
            else if (!right && !up && !left && down)
                t = 4;
            else if (right && up && !left && !down)
                t = 5;
            else if (!right && up && left && !down)
                t = 6;
            else if (!right && !up && left && down)
                t = 7;
            else if (right && !up && !left && down)
                t = 8;
            else if (right && !up && left && !down)
                t = 9;
            else if (!right && up && !left && down)
                t = 10;
            else if (right && up && !left && down)
                t = 11;
            else if (right && up && left && !down)
                t = 12;
            else if (!right && up && left && down)
                t = 13;
            else if (right && !up && left && down)
                t = 14;
            else if (right && up && left && down)
                t = 15;
            else if (!right && !up && !left && !down)
                t = 16;

            if (engine().room_.coord_.x == x and engine().room_.coord_.y == y) {
                t += 16;
            }


            if (left or right or up or down) {
                platform().set_tile(Layer::overlay, x + 5 + margin, y + margin, 126 + t);
            }
        }
    }
}


void MapScene::enter(Scene& prev_scene)
{
    show_worldmap();

    zone_text_.emplace(platform(),
                       OverlayCoord{9, 17});
    zone_text_->assign([] {
                           switch (engine().room_.zone_) {
                           case 1:
                               return "natural caves";

                           case 2:
                               return "army storage";

                           case 3:
                               return "control";

                           case 4:
                               return "light factory";

                           case 5:
                               return "new caves";

                           case 6:
                               return "war factory";

                           case 7:
                               return "guardian zone";

                           case 8:
                               return "annihltr factry";

                           case 9:
                               return "old base";

                           case 10:
                               return "core";

                           default:
                               return "?";
                           }
                       }());
}


ScenePtr<Scene> MapScene::step()
{
    if (key_down<Key::start>()) {
        for (int x = 0; x < 20; ++x) {
            for (int y = 0; y < 20; ++y) {
                platform().set_tile(Layer::overlay, x + 5, y, 0);
            }
        }
        engine().paused_ = false;
        return scene_pool::alloc<OverworldScene>();
    }

    flicker_cyc_ += 1;
    if (flicker_cyc_ == 5) {
        flicker_cyc_ = 0;

        auto t = platform().get_tile(Layer::overlay, pt_.x, pt_.y);
        if (flicker_on_) {
            t -= 16;
        } else {
            t += 16;
        }

        flicker_on_ = not flicker_on_;

        platform().set_tile(Layer::overlay, pt_.x, pt_.y, t);
    }

    return null_scene();
}


void MapScene::display()
{
}



ScenePtr<Scene> OverworldScene::step()
{
    if (engine().g_.hp_ <= 0) {
        if (respawn_countdown_ == 0) {
            platform().speaker().play_sound("snd_death", 21);
            platform().speaker().play_sound("snd_explo3", 21);
            respawn_countdown_ = 80;
            engine().add_object<ExploSpewer>(engine().hero()->position());
        } else if (respawn_countdown_ == 1) {
            platform().speaker().play_sound("snd_herospawn", 11);
            engine().respawn_to_checkpoint();
        }
        --respawn_countdown_;
    }

    auto step_list = [&](auto& objects, auto on_destroy) {
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
    step_list(e.player_projectiles_, [&](auto&) { --e.g_.shot_count_; });

    int shake = 0;
    if (engine().g_.screenshake_ > 0) {
        engine().g_.screenshake_ -= 1;

        if (engine().g_.screenshake_) {
            shake = (engine().g_.screenshake_ % 2) * 2;
        }
    }
    platform().spr_scroll(shake);
    platform().scroll(Layer::map_0, 0, -shake);

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

    if (key_down<Key::start>() and not engine().g_.tetron_destroyed_) {
        engine().paused_ = true;
        return scene_pool::alloc<MapScene>();
    }

    if (engine().g_.warpready_) {
        engine().g_.warpready_ = false;
        auto next = scene_pool::alloc<MapScene>();
        next->warpmode_ = true;
        return next;
    }

    if (engine().g_.tetron_destroyed_) {
        return scene_pool::alloc<FadeOutScene>();
    }

    return null_scene();
}



void EndgameStatsScene::enter(Scene& prev_scene)
{
    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            platform().set_tile(Layer::map_1, i, j, 0);
            platform().set_tile(Layer::map_0, i, j, 0);
        }
    }

    platform().sleep(1);
    platform().screen().fade(0);

    platform().fill_overlay(0);


    text_lines_.emplace_back(platform(),
                             "Thank you for playing!",
                             OverlayCoord{4, 4});

    text_lines_.emplace_back(platform(),
                             "Computers: ",
                             OverlayCoord{7, 6});
    text_lines_.back().append(engine().p_->computer_);
    text_lines_.back().append("/");
    text_lines_.back().append(10);

    text_lines_.emplace_back(platform(),
                             "Bosses: ",
                             OverlayCoord{9, 8});
    text_lines_.back().append(engine().p_->completed_bosses_.size() + 1);
    text_lines_.back().append("/");
    text_lines_.back().append(8);

    static const float screen_hz = 59.72f;
    int seconds = engine().p_->frames_spent_ / screen_hz;


    int h = seconds / 3600;
    int m = (seconds -(3600*h))/60;
    int s = (seconds -(3600*h)-(m*60));

    text_lines_.emplace_back(platform(),
                             "Clear Time: ",
                             OverlayCoord{5, 10});
    if (h < 10) {
        text_lines_.back().append("0");
    }
    text_lines_.back().append(h);
    text_lines_.back().append(":");

    if (m < 10) {
        text_lines_.back().append("0");
    }
    text_lines_.back().append(m);
    text_lines_.back().append(":");

    if (s < 10) {
        text_lines_.back().append("0");
    }
    text_lines_.back().append(s);
}



void CreditsScene::enter(Scene& prev_scene)
{
    platform().screen().fade(1);

    scroll_ = -70;
    platform().scroll(Layer::overlay, 0, scroll_);


    text_lines_.emplace_back(platform(),
                             "Game by Daniel Remar",
                             OverlayCoord{4, 4});

    text_lines_.emplace_back(platform(),
                             "Music by Brother Android",
                             OverlayCoord{3, 7});

    text_lines_.emplace_back(platform(),
                             "GBA Port by Evan Bowman",
                             OverlayCoord{3, 10});
}



void CreditsScene::exit(Scene& prev_scene)
{
    text_lines_.clear();
    platform().scroll(Layer::overlay, 0, 0);
}



ScenePtr<Scene> CreditsScene::step()
{
    scroll_ *= 0.85f;
    platform().scroll(Layer::overlay, 0, scroll_);

    if (++cnt_ == 200 or
        (cnt_ > 20 and (key_pressed<Key::action_2>() or
                        key_pressed<Key::action_1>()))) {
        return scene_pool::alloc<TitleScreenScene>();
    }

    return null_scene();
}



ScenePtr<Scene> TitleScreenScene::step()
{
    if (cnt_ < 70) {
        cnt_++;
        if (cnt_ == 10) {
            platform().sleep(1);
            platform().load_overlay_texture("overlay_title_screen");
            draw_image(platform(), 253, 6, 1, 19, 9, Layer::overlay);
        }
        if (cnt_ == 20) {
            platform().sleep(1);
            platform().load_overlay_texture("overlay_title_screen_2");
        }
        if (cnt_ == 30) {
            platform().sleep(1);
            platform().load_overlay_texture("overlay_title_screen_3");
        }
        if (cnt_ == 40) {
            platform().sleep(1);
            platform().load_overlay_texture("overlay_title_screen_4");
        }

        if (cnt_ == 70) {
            text_lines_.emplace_back(platform(), "continue", OverlayCoord{10, 13});
            text_lines_.emplace_back(platform(), "newgame", OverlayCoord{10, 15});
            platform().set_tile(Layer::overlay, 8, 13, 121);
            platform().set_tile(Layer::overlay, 8, 15, 0);
        }
    } else {
        if (key_down<Key::down>()) {
            sel_ = 1;
            platform().set_tile(Layer::overlay, 8, 13, 0);
            platform().set_tile(Layer::overlay, 8, 15, 121);
        }
        if (key_down<Key::up>()) {
            sel_ = 0;
            platform().set_tile(Layer::overlay, 8, 13, 121);
            platform().set_tile(Layer::overlay, 8, 15, 0);
        }

        if (key_down<Key::start>() or key_down<Key::action_1>() or key_down<Key::action_2>()) {
            return scene_pool::alloc<OverworldScene>();
        }
    }
    return null_scene();
}



void TitleScreenScene::enter(Scene&)
{
    platform().speaker().play_music("title", 0);
    platform().screen().fade(1);
}



void TitleScreenScene::exit(Scene&)
{
    platform().fill_overlay(0);
    text_lines_.clear();
    platform().screen().clear();
    platform().screen().display();
    if (sel_ == 0 and engine().has_save()) {
        platform().speaker().play_sound("snd_herospawn", 11);
    } else {
        platform().speaker().stop_music();
        platform().sleep(60);
    }
    engine().begin_game(engine().g_.difficulty_, sel_ == 0);
    platform().load_overlay_texture("overlay");
    platform().screen().fade(0);
}



} // namespace herocore
