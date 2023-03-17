#include "dialogScene.hpp"
#include "engine.hpp"
#include "graphics/overlay.hpp"
#include "scenes.hpp"



Platform::TextureCpMapper locale_texture_map();



namespace herocore
{



void DialogScene::clear_textbox(Platform& pfrm)
{
    const auto st = calc_screen_tiles(pfrm);

    for (int x = 1; x < st.x - 1; ++x) {
        pfrm.set_tile(Layer::overlay, x, anchor_ - 5, 234);
        pfrm.set_tile(Layer::overlay, x, anchor_ - 4, 95);
        pfrm.set_tile(Layer::overlay, x, anchor_ - 3, 95);
        pfrm.set_tile(Layer::overlay, x, anchor_ - 2, 95);
        pfrm.set_tile(Layer::overlay, x, anchor_ - 1, 235);
    }

    pfrm.set_tile(Layer::overlay, 0, anchor_ - 4, 236);
    pfrm.set_tile(Layer::overlay, 0, anchor_ - 3, 236);
    pfrm.set_tile(Layer::overlay, 0, anchor_ - 2, 236);

    pfrm.set_tile(Layer::overlay, st.x - 1, anchor_ - 4, 237);
    pfrm.set_tile(Layer::overlay, st.x - 1, anchor_ - 3, 237);
    pfrm.set_tile(Layer::overlay, st.x - 1, anchor_ - 2, 237);

    pfrm.set_tile(Layer::overlay, 0, anchor_ - 5, 238);
    pfrm.set_tile(Layer::overlay, 0, anchor_ - 1, 240);
    pfrm.set_tile(Layer::overlay, st.x - 1, anchor_ - 5, 239);
    pfrm.set_tile(Layer::overlay, st.x - 1, anchor_ - 1, 241);

    text_state_.line_ = 0;
    text_state_.pos_ = 0;
}



void DialogScene::init_text(Platform& pfrm, const char* str)
{
    clear_textbox(pfrm);

    text_state_.current_word_remaining_ = 0;
    text_state_.current_word_ = (*text_state_.text_)->c_str();
    text_state_.timer_ = 0;
    text_state_.line_ = 0;
    text_state_.pos_ = 0;
}



bool DialogScene::advance_text()
{
    const auto delay = 5;

    text_state_.timer_ += 2;

    const auto st = calc_screen_tiles(platform());

    if (text_state_.timer_ > delay) {
        text_state_.timer_ = 0;

        platform().speaker().play_sound("msg", 5);

        if (text_state_.current_word_remaining_ == 0) {
            while (*text_state_.current_word_ == ' ') {
                text_state_.current_word_++;
                if (text_state_.pos_ < st.x - 2) {
                    text_state_.pos_ += 1;
                }
            }
            bool done = false;
            utf8::scan(
                [&](const utf8::Codepoint& cp, const char*, int) {
                    if (done) {
                        return;
                    }
                    if (cp == ' ') {
                        done = true;
                    } else {
                        text_state_.current_word_remaining_++;
                    }
                },
                text_state_.current_word_,
                str_len(text_state_.current_word_));
        }

        // At this point, we know the length of the next space-delimited word in
        // the string. Now we can print stuff...

        const auto st = calc_screen_tiles(platform());
        static const auto margin_sum = 2;
        const auto text_box_width = st.x - margin_sum;
        const auto remaining = (text_box_width - text_state_.pos_) -
                               (text_state_.line_ == 0 ? 0 : 1);

        if (remaining < text_state_.current_word_remaining_) {
            if (text_state_.line_ == 0) {
                text_state_.line_++;
                text_state_.pos_ = 0;
                return true;
            } else {
                return false;
            }
        }

        int bytes_consumed = 0;
        const auto cp = utf8::getc(text_state_.current_word_, &bytes_consumed);


        u16 t = 495; // bad glyph, FIXME: add a constant

        t = platform().map_glyph(cp, locale_texture_map());

        const int y_offset = text_state_.line_ == 0 ? 4 : 2;
        const int x_offset = text_state_.pos_ + 1;

        platform().set_tile(Layer::overlay, x_offset, anchor_ - y_offset, t);

        text_state_.current_word_remaining_--;
        text_state_.current_word_ += bytes_consumed;
        text_state_.pos_++;

        if (*text_state_.current_word_ == '\0') {
            display_mode_ = DisplayMode::key_released_check2;
        }
    }

    return true;
}



void DialogScene::enter(Scene& prev)
{
    // Don't cover the hero!
    if (engine().hero()->y() > 120) {
        anchor_ = 5;
    }

    if (not disable_hud_) {
        engine().draw_hud();
    }
    init_text(platform(), (*text_state_.text_)->c_str());
    engine().paused_ = true;
}



void DialogScene::exit(Scene& next)
{
    engine().paused_ = false;
}



ScenePtr<Scene> DialogScene::step()
{
    auto& pfrm = platform();

    auto animate_moretext_icon = [&] {
        static const auto duration = 10;
        text_state_.timer_ += 1;
        if (text_state_.timer_ > duration) {
            text_state_.timer_ = 0;
            const auto st = calc_screen_tiles(platform());
            if (pfrm.get_tile(Layer::overlay, st.x - 2, anchor_ - 2) == 242) {
                pfrm.set_tile(Layer::overlay, st.x - 2, anchor_ - 2, 243);
            } else {
                pfrm.set_tile(Layer::overlay, st.x - 2, anchor_ - 2, 242);
            }
        }
    };

    switch (display_mode_) {
    case DisplayMode::animate_in:
        display_mode_ = DisplayMode::busy;
        break;

    case DisplayMode::busy: {

        const bool text_busy = advance_text();

        if (not text_busy) {
            display_mode_ = DisplayMode::key_released_check1;
        } else {
            if (platform().keyboard().down_transition(Key::action_1) or
                platform().keyboard().down_transition(Key::action_2)) {

                while (advance_text()) {
                    if (display_mode_ not_eq DisplayMode::busy) {
                        break;
                    }
                }

                if (display_mode_ == DisplayMode::busy) {
                    display_mode_ = DisplayMode::key_released_check1;
                }
            }
        }
    } break;

    case DisplayMode::wait: {
        animate_moretext_icon();

        if (platform().keyboard().down_transition(Key::action_1) or
            platform().keyboard().down_transition(Key::action_2)) {

            text_state_.timer_ = 0;

            clear_textbox(platform());
            display_mode_ = DisplayMode::busy;
        }
        break;
    }

    case DisplayMode::key_released_check1:
        if (not platform().keyboard().down_transition(Key::action_1) and
            not platform().keyboard().down_transition(Key::action_2)) {

            text_state_.timer_ = seconds(1);
            display_mode_ = DisplayMode::wait;
        }
        break;

    case DisplayMode::key_released_check2:
        if (not platform().keyboard().down_transition(Key::action_1) and
            not platform().keyboard().down_transition(Key::action_2)) {

            text_state_.timer_ = seconds(1);
            display_mode_ = DisplayMode::done;
        }
        break;

    case DisplayMode::done:
        animate_moretext_icon();
        if (platform().keyboard().down_transition(Key::action_1) or
            platform().keyboard().down_transition(Key::action_2)) {

            display_mode_ = DisplayMode::animate_out;
        }
        break;

    case DisplayMode::animate_out:
        display_mode_ = DisplayMode::clear;
        platform().fill_overlay(0);
        if (not disable_hud_) {
            engine().draw_hud();
        }
        break;

    case DisplayMode::clear:
        return scene_pool::alloc<OverworldScene>();
    }

    return null_scene();
}



}
