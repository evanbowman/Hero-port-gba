#pragma once

#include "scene.hpp"
#include "bulkAllocator.hpp"
#include "engine.hpp"


namespace herocore
{



class DialogScene : public Scene {
public:
    DialogScene(const char* msg)
    {
        text_state_.text_ =
            allocate_dynamic<StringBuffer<1000>>(platform(), msg);
    }

    void enter(Scene& prev) override;
    void exit(Scene& next) override;

    ScenePtr<Scene> step() override;

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

protected:
    struct TextWriterState {
        std::optional<DynamicMemory<StringBuffer<1000>>> text_;
        const char* current_word_;
        int timer_;
        u8 line_;
        u8 pos_;
        u8 current_word_remaining_;
    };

    TextWriterState text_state_;

    void clear_textbox(Platform& pfrm);

    // Return false when the textbox has no more room to print additional
    // glyphs, otherwise, return true.
    bool advance_text();

    void init_text(Platform& pfrm, const char*);

    u8 anchor_ = 20;

    enum class DisplayMode {
        animate_in,
        busy,
        key_released_check1,
        key_released_check2,
        wait,
        done,
        animate_out,
        clear,
    } display_mode_ = DisplayMode::animate_in;

    bool disable_hud_ = false;

};




}
