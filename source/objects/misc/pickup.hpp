#pragma once

#include "engine.hpp"
#include "objects/particles/explo.hpp"
#include "objects/solid.hpp"


namespace herocore
{


class Pickup : public Solid
{
public:
    enum Type { blaster, blade, suit, computer };


    Pickup(const Vec2<Fixnum>& pos, Type type) : type_(type)
    {
        position_ = pos;

        origin_ = {8, 8};

        switch (type) {
        case blaster:
            sprite_index_ = 191;
            break;

        case blade:
            sprite_index_ = 192;
            break;

        case suit:
            sprite_index_ = 193;
            break;

        case computer:
            sprite_index_ = 237;
            break;
        }

        hitbox_.dimension_.size_ = {14, 14};
        hitbox_.dimension_.origin_ = {8, 8};
    }


    bool overlapping(Object& other) override
    {
        if (dead()) {
            return false;
        }

        if (&other == engine().hero()) {
            bool intersect = hitbox().overlapping(other.hitbox());

            if (intersect) {
                kill();
                switch (type_) {
                case blaster:
                    ++engine().p_->blaster_;
                    switch (engine().p_->blaster_) {
                    case 2:
                        engine().show_dialog("Blaster level 2! "
                                             "Fires heavy blaster pellets. "
                                             "Max 6 pellets on screen at "
                                             "once. I can't fire during "
                                             "overheat.");
                        break;

                    case 3:
                        engine().show_dialog("Blaster level 3! "
                                             "Fires super blaster pellets. "
                                             "Max 6 pellets on screen at "
                                             "once. I can't fire during "
                                             "overheat.");
                        break;
                    }
                    engine().levelup();
                    break;

                case blade:
                    ++engine().p_->blade_;
                    switch (engine().p_->blade_) {
                    case 1:
                        engine().show_dialog("Blade level 1! Hold any "
                                             "fire key for a short while and"
                                             " release. Cuts dirt and deflects"
                                             " small enemy shots.");
                        break;

                    case 2:
                        engine().show_dialog("Blade level 2! Hold any "
                                             "fire key for a short while and"
                                             " release. Cuts dirt and pipes and"
                                             " deflects"
                                             " medium shots.");
                        break;

                    case 3:
                        engine().show_dialog("Blade level 3! Hold any "
                                             "fire key for a short while and"
                                             " release. Cuts dirt and pipes and"
                                             " deflects"
                                             " big shots.");
                        break;
                    }
                    engine().levelup();
                    break;

                case suit:
                    ++engine().p_->suit_;
                    switch (engine().p_->suit_) {
                    case 1:
                        engine().show_dialog("Suit level 1! My suit's cooling "
                                             "system is now twice as effective "
                                             "in reducing overheat.");
                        break;

                    case 2:
                        engine().show_dialog("Suit level 2! My suit's cooling "
                                             "now prevents all overheat, "
                                             "letting me use my weapons at any "
                                             "time.");
                        break;

                    case 3:
                        engine().show_dialog("Suit level 2! My suit now doubles "
                                             "my defense against all attacks "
                                             "and prevents overheat.");
                        break;
                    }
                    engine().levelup();
                    break;

                case computer:
                    ++engine().p_->computer_;
                    switch (engine().p_->computer_) {
                    case 1:
                        engine().show_dialog("This is a special computer "
                                             "connected to the base core. "
                                             "There seem to be 10 of them "
                                             "in total. My suit will keep "
                                             "track of how many I have found.");
                        break;

                    case 2:
                        engine().show_dialog("These core computers seem to "
                                             "control Tetron's subsystems. "
                                             "the more of them I find, the "
                                             "more I start to understand their "
                                             "function...");
                        break;

                    case 3:
                        engine().show_dialog("I see now. If I activate all ten "
                                             "core computers and defeat Tetron,"
                                             " the explosion will destroy him, "
                                             "the base, and the entire machine "
                                             "army!");
                        break;

                    case 4:
                        engine().show_dialog("I have found several of the "
                                             "computers. Something strange stirs"
                                             " within me as I realize that I may"
                                             " be on my way to destroying Tetron"
                                             " forever.");
                        break;

                    case 5:
                        engine().show_dialog("My entire life has revolved around"
                                             " Tetron, his bases and machines. "
                                             "We have clashed many times in"
                                             "the past, with no victor.");
                        break;

                    case 6:
                        engine().show_dialog("Four computers left. If I find "
                                             "them all I can finally end the "
                                             "eternal fight against tetron and "
                                             "the threat he poses to earth.");
                        break;

                    case 7:
                        engine().show_dialog("Tetron is a terrifying machine, "
                                             "able to build factories that in "
                                             "turn build his armies. His goal "
                                             "is not to rule the earth but to "
                                             "destroy it.");
                        break;

                    case 8:
                        engine().show_dialog("Tetron was the result of a war "
                                             "long forgotten, a self-sustaining"
                                             " machine built to last forever."
                                             "He will eventually be able to "
                                             "replicate himself.");
                        break;

                    case 9:
                        engine().show_dialog("The war Tetron was built for no "
                                             "longer exists, the civilizations "
                                             "gone. Beyond mortal lifetimes, "
                                             "he still believes the battle must"
                                             " be won.");
                        break;

                    case 10:
                        engine().show_dialog("All core computers have been "
                                             "activated. My victory over Tetron"
                                             " now will free us both. I must "
                                             "save the earth... and save Tetron"
                                             " from himself.");
                        break;
                    }
                    engine().remove_computer();
                    engine().draw_hud();
                    break;
                }

                platform().speaker().play_sound("snd_pickup", 10);
            }

            return intersect;
        }
        return false;
    }


    void draw(Platform::Screen& s) const override
    {
        cyc_ += 1;
        if (cyc_ == 8) {
            cyc_ = 0;
        }

        if (cyc_ > 4) {
            Solid::draw(s);
        }
    }


private:
    Type type_;
    mutable u8 cyc_ = 0;
};


} // namespace herocore
