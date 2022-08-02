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
    add_object<Spew>(Vec2<Fixnum>{200, 80});
    add_object<Bolt>(Vec2<Fixnum>{40, 40});

    draw_hud();
}



void Engine::run()
{
    while (true) {

        if (frame_count_ == 2) {
            frame_count_ = 0;
        } else {

            _platform->keyboard().poll();

            hero_->step();

            for (auto it = objects_.begin(); it not_eq objects_.end();) {
                if ((*it)->dead()) {
                    it = objects_.erase(it);
                } else {
                    (*it)->step();
                    ++it;
                }
            }
            ++frame_count_;
        }

        _platform->screen().clear();

        for (auto& obj : objects_) {

            hero_->draw(_platform->screen());

            if (not obj->dead()) {
                obj->draw(_platform->screen());
            }
        }

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



}
