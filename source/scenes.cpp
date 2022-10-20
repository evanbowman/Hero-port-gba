#include "scenes.hpp"
#include "maps.hpp"



namespace herocore
{



static const std::array<Vec2<u8>, 10> hint_order_normal = {{
    {11, 4},
    {11, 0},
    {9, 6},
    {0, 3},
    {9, 9},
    {8, 1},
    {1, 6},
    {6, 9},
    {5, 13},
    {7, 7}
}};



static const std::array<Vec2<u8>, 10> hint_order_hard = {{
    {13, 8},
    {11, 6},
    {1, 4},
    {8, 13},
    {7, 4},
    {5, 14},
    {14, 11},
    {0, 11},
    {9, 1},
    {7, 7}
}};



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

            if ((x < 3 and y < 3) or
                (x > 11 and y < 3) or
                (x < 3 and y > 11) or
                (x > 11 and y > 11)) {
                continue;
            }

            if (not engine().p_->visited_.get(x, y)) {
                platform().set_tile(Layer::overlay,
                                    x + 5 + margin + pad_x,
                                    y + margin,
                                    125);
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
            else if  (!right && up && !left && !down)
                t = 2;
            else if  (!right && !up && left && !down)
                t = 3;
            else if  (!right && !up && !left && down)
                t = 4;
            else if  (right && up && !left && !down)
                t = 5;
            else if  (!right && up && left && !down)
                t = 6;
            else if  (!right && !up && left && down)
                t = 7;
            else if  (right && !up && !left && down)
                t = 8;
            else if  (right && !up && left && !down)
                t = 9;
            else if  (!right && up && !left && down)
                t = 10;
            else if  (right && up && !left && down)
                t = 11;
            else if  (right && up && left && !down)
                t = 12;
            else if  (!right && up && left && down)
                t = 13;
            else if  (right && !up && left && down)
                t = 14;
            else if  (right && up && left && down)
                t = 15;
            else if  (!right && !up && !left && !down)
                t = 16;

            if (engine().room_.coord_.x == x and
                engine().room_.coord_.y == y) {
                t += 16;
                pt_ = {x + 5 + margin + pad_x, y + margin};
            }

            if (left or right or up or down) {
                platform().set_tile(Layer::overlay,
                                    x + 5 + margin + pad_x,
                                    y + margin,
                                    126 + t);
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
                platform().set_tile(Layer::overlay,
                                    h.x + 5 + margin + pad_x,
                                    h.y + margin,
                                    158);
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
                platform().set_tile(Layer::overlay,
                                    h.x + 5 + margin + pad_x,
                                    h.y + margin,
                                    158);
                break;
            }
        }
    }


    draw_image(platform(),
               159,
               6,
               17,
               3,
               2,
               Layer::overlay);

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

            if ((x < 3 and y < 3) or
                (x > 11 and y < 3) or
                (x < 3 and y > 11) or
                (x > 11 and y > 11)) {
                continue;
            }

            if (not engine().p_->visited_.get(x, y)) {
                platform().set_tile(Layer::overlay,
                                    x + 5 + margin,
                                    y + margin,
                                    125);
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
            else if  (!right && up && !left && !down)
                t = 2;
            else if  (!right && !up && left && !down)
                t = 3;
            else if  (!right && !up && !left && down)
                t = 4;
            else if  (right && up && !left && !down)
                t = 5;
            else if  (!right && up && left && !down)
                t = 6;
            else if  (!right && !up && left && down)
                t = 7;
            else if  (right && !up && !left && down)
                t = 8;
            else if  (right && !up && left && !down)
                t = 9;
            else if  (!right && up && !left && down)
                t = 10;
            else if  (right && up && !left && down)
                t = 11;
            else if  (right && up && left && !down)
                t = 12;
            else if  (!right && up && left && down)
                t = 13;
            else if  (right && !up && left && down)
                t = 14;
            else if  (right && up && left && down)
                t = 15;
            else if  (!right && !up && !left && !down)
                t = 16;

            if (engine().room_.coord_.x == x and
                engine().room_.coord_.y == y) {
                t += 16;
            }


            if (left or right or up or down) {
                platform().set_tile(Layer::overlay,
                                    x + 5 + margin,
                                    y + margin,
                                    126 + t);
            }
        }
    }
}



void MapScene::enter(Scene& prev_scene)
{
    show_worldmap();
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




}
