#include "scenes.hpp"
#include "maps.hpp"



namespace herocore
{



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

    for (int x = 0; x < 15; ++x) {
        for (int y = 0; y < 15; ++y) {

            if ((x < 3 and y < 3) or
                (x > 11 and y < 3) or
                (x < 3 and y > 11) or
                (x > 11 and y > 11)) {
                continue;
            }

            if (not engine().visited_.get(x, y)) {
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

    draw_image(platform(),
               159,
               6,
               17,
               3,
               2,
               Layer::overlay);

    platform().set_tile(Layer::overlay, 5, 0, 165);
    platform().set_tile(Layer::overlay, 5, 16, 167);
    platform().set_tile(Layer::overlay, 5 + 16, 16, 169);
    platform().set_tile(Layer::overlay, 5 + 16, 0, 171);

    for (int y = 1; y < 16; ++y) {
        platform().set_tile(Layer::overlay, 5, y, 166);
        platform().set_tile(Layer::overlay, 5 + 16, y, 170);
        platform().set_tile(Layer::overlay, 5 + y, 16, 168);
        platform().set_tile(Layer::overlay, 5 + y, 0, 172);
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

            if (not engine().visited_.get(x, y)) {
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
    if (platform().keyboard().down_transition<Key::start>()) {
        for (int x = 0; x < 20; ++x) {
            for (int y = 0; y < 20; ++y) {
                platform().set_tile(Layer::overlay, x + 5, y, 0);
            }
        }
        engine().paused_ = false;
        return scene_pool::alloc<OverworldScene>();
    }

    return null_scene();
}



void MapScene::display()
{

}




}
