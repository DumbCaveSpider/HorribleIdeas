#pragma once

#include <HorribleIdeas.hpp>

#include <Geode/Geode.hpp>

#include <classes/Jumpscares.hpp>
#include <classes/Options.hpp>
#include <classes/Randng.hpp>

#include <classes/ui/MathQuiz.hpp>
#include <classes/ui/RandomAdPopup.hpp>

using namespace geode::prelude;
using namespace horribleideas;

// Namespace for utility methods for Horrible Ideas
namespace horrible {
    // Pointer reference to this Geode mod
    inline Mod* horribleMod = Mod::get();

    // Utilities for SillyTier
    namespace silly {
        /**
         * Get the int for the tier of silly
         *
         * @param silly Silly tier enum
         *
         * @returns The integer from the silly tier
         */
        static int getInt(SillyTier silly) {
            switch (silly) {
            case SillyTier::None:
                return 0; // No silliness

            case SillyTier::Low:
                return 1;

            case SillyTier::Medium:
                return 2;

            case SillyTier::High:
                return 3;

            default:
                return 0;
            };

            return 0;
        };
    };

    // Default horrible categories
    class Category {
    public:
        static inline const std::string playerlife = "Player Life";
        static inline const std::string jumpscares = "Jumpscares";
        static inline const std::string randoms = "Randoms";
        static inline const std::string chances = "Chances";
        static inline const std::string obstructive = "Obstructive";
        static inline const std::string misc = "Misc";
    };

    // Default horrible options
    static inline std::vector<Option> allOptions = {
        {"oxygen",
         "Oxygen Level",
         "Limited oxygen level. You gain oxygen as a flying gamemode. If your oxygen runs out, your player dies.\n<cy>Credit: ArcticWoof</c>",
         Category::playerlife,
         SillyTier::High},
        {"health",
         "Player Health",
         "Add a health bar and decreases everytime you collide with a hazard. If your health reaches zero, your player dies.\n<cy>Credit: Cheeseworks</c>",
         Category::playerlife,
         SillyTier::Medium},
        {"grief",
         "Get Back on Grief",
         "A chance at death of forcing you to play Grief.\n<cy>Credit: Sweep</c>",
         Category::jumpscares,
         SillyTier::High},
        {"congregation",
         "Congregation Jumpscare",
         "A chance at death of forcing you to play Congregation.\n<cy>Credit: StaticGD</c>",
         Category::jumpscares,
         SillyTier::High},
        {"mock",
         "Mock your 90%+ Fail",
         "Shows a screenshot of one of your 90%-99% fails in the main menu.\n<cy>Credit: Wuffin</c>\n\n<cr>Note: This will not work on macOS and iOS</c>",
         Category::misc,
         SillyTier::Medium,
         false,
         {PlatformID::Windows,
          PlatformID::Android}},
        {"freeze",
         "Random 90%+ FPS Drop",
         "Your visual framerate starts randomly dropping between 90-99% while playing.\n<cy>Credit: Hexfire</c>",
         Category::randoms,
         SillyTier::Medium},
        {"achieve",
         "Random Achievements",
         "Play the achievement sound when doing random things.\n<cy>Credit: Cheeseworks</c>",
         Category::randoms,
         SillyTier::Low},
        {"crash_death",
         "Crash Chance on Death",
         "When you die in the level, there's a small chance your game will die too. Don't worry your progress is saved when crashed :)\n<cy>Credit: DragonixGD</c>",
         Category::chances,
         SillyTier::High},
        {"math_quiz",
         "Richard's Math Quiz!",
         "When playing a level in practice mode, there's a chance Richard will pop out and give you a quick math quiz. Answer correctly to continue, or restart the level from the beginning.\n<cy>Credit: CyanBoi</c>",
         Category::obstructive,
         SillyTier::High},
        {"no_jump",
         "Randomly Don't Jump",
         "When making an input in a level, there will be a chance your character does not jump.\n<cy>Credit: GilanyKing12</c>",
         Category::randoms,
         SillyTier::Low},
        {"gravity",
         "Randomize Gravity",
         "Every time you jump in the level, gravity force will increase or decrease randomly.\n<cy>Credit: NJAgain</c>",
         Category::randoms,
         SillyTier::Low},
        {"death",
         "Fake Death",
         "The player's death effect will play but will not die.\n<cy>Credit: DragonixGD</c>",
         Category::obstructive,
         SillyTier::Medium},
        {"upside_down",
         "Upside-Down Chance",
         "When navigating the game, there's a chance it'll be upside-down and probably break everything.\n<cy>Credit: Cheeseworks</c>",
         Category::chances,
         SillyTier::Medium},
        {"ads",
         "Level Ads",
         "While playing a level in normal mode, an ad for a random level will pop up on the screen from time to time.\n<cy>Credit: staticGD</c>",
         Category::obstructive,
         SillyTier::Medium},
        {"black_screen",
         "Black Screen Blink",
         "The screen can suddenly turn black for a split second while playing a level.\n<cy>Credit: elite_smiler_ispro</c>",
         Category::obstructive,
         SillyTier::Low},
        {"parry",
         "Parry Obstacles",
         "Whenever your hitbox is inside of a hazard hitbox, you will not die if you time your click right.\n<cy>Credit: Wuffin</c>",
         Category::misc,
         SillyTier::Low},
        {"double_jump",
         "Double-Jump",
         "Allow your character to double-jump in a level.\n<cy>Credit: Cheeseworks</c>",
         Category::misc,
         SillyTier::Low},
        {"sleepy",
         "Sleepy Player",
         "Your character will occasionally fall asleep while playing.\n<cy>Credit: this_guy_yt</c>",
         Category::misc,
         SillyTier::Low},
        {"pauses",
         "Random Pauses",
         "While playing a level, the game will randomly pause itself.\n<cy>Credit: DragonixGD</c>",
         Category::randoms,
         SillyTier::Low},
        {"ice_level",
         "Ice Level",
         "Make every surface icy. Slip and slide!\n<cy>Credit: TimeRed</c>",
         Category::misc,
         SillyTier::Medium},
        {"random_mirror",
         "Random Mirror Portal",
         "Randomly activates a mirror portal in the level.\n<cy>Credit: TimeRed</c>",
         Category::randoms,
         SillyTier::Low},
        {"random_speed",
         "Random Speed Change",
         "Randomly changes your speed while playing a level.\n<cy>Credit: imdissapearinghelp</c>",
         Category::randoms,
         SillyTier::Medium},
        {"random_icon",
         "Random Icon Change",
         "Randomly change your icon every time you jump.\n<cy>Credit: JompyDoJump</c>",
         Category::randoms,
         SillyTier::Low},
        {"blinking_icon",
         "Blinking Icon",
         "Your icon will start to randomly blink.\n<cy>Credit: DragonixGD</c>",
         Category::obstructive,
         SillyTier::Low},
        {"dementia",
         "Dementia",
         "Chance of your player randomly teleports back. This is more like player lagging to be honest!\n<cy>Credit: imdissapearinghelp</c>",
         Category::misc,
         SillyTier::Medium},
        {"earthquake",
         "Earthquake",
         "Constantly shakes the camera while playing a level.\n<cy>Credit: ArcticWoof</c>",
         Category::obstructive,
         SillyTier::Medium},
        {"fake_crash",
         "Random Fake Crash",
         "While playing a level, there's a chance your game will fake crash.\n<cy>Credit: Timered</c>",
         Category::randoms,
         SillyTier::Medium},
        {"gambler",
         "Gambler",
         "While playing a level and reach at the 95%, there's a 50/50 chance you either complete the level or blast into the space backwards.\n<cy>Credit: Timered</c>",
         Category::misc,
         SillyTier::Medium},
        {"placebo",
         "Placebo",
         "A small chance that when you start a level, all the horrible mods you have enabled are disabled, or all the horrible mods you have disabled are enabled.\n<cy>Credit: tmdXD</c>",
         Category::misc,
         SillyTier::High},
        {"click_speed",
         "Click Player Speed",
         "Randomly increases or decreases your player speed everytime you jump.\n<cy>Credit: KGplayerA</c>",
         Category::randoms,
         SillyTier::Medium } };
};