#pragma once

#include <Horrible.hpp>

#include <fmt/core.h>

#include <Geode/Geode.hpp>

#include <classes/Jumpscares.hpp>
#include <classes/Menu.hpp>
#include <classes/Options.hpp>
#include <classes/Randng.hpp>

#include <classes/ui/MathQuiz.hpp>
#include <classes/ui/RandomAdPopup.hpp>
#include <classes/ui/SpamJumps.hpp>

using namespace geode::prelude;

// Additional utility methods for Horrible Ideas
namespace horrible {
    // Pointer reference to this Geode mod
    inline Mod* horribleMod = Mod::get();

    /**
     * Convert a chance setting number to a cooldown percentage decimal
     *
     * @param chance The chance setting number
     */
    inline float chanceToDelayPct(int chance) {
        if (chance <= 0) chance = 1;
        if (chance > 100) chance = 100;

        return ((100.f - static_cast<float>(chance)) + 1.f) / 100.f;
    };

    namespace fs = std::filesystem; // Shortcut for std::filesystem

    namespace str = utils::string; // Shortcut for geode::utils::string

    // Default horrible categories
    namespace category {
        constexpr std::string_view playerlife = "Player Life";
        constexpr std::string_view jumpscares = "Jumpscares";
        constexpr std::string_view randoms = "Randoms";
        constexpr std::string_view chances = "Chances";
        constexpr std::string_view obstructive = "Obstructive";
        constexpr std::string_view misc = "Misc";
    };

    // Default horrible options
    inline std::vector<Option> const allOptions = {
        {"oxygen",
         "Oxygen Level",
         "Limited oxygen level. You gain oxygen as a flying gamemode. If your oxygen runs out, the player dies.\n<cy>Credit: ArcticWoof</c>",
         category::playerlife,
         SillyTier::High},
        {"health",
         "Player Health",
         "Add a health bar and decreases everytime you collide with a hazard. If your health reaches zero, the player dies.\n<cy>Credit: Cheeseworks</c>",
         category::playerlife,
         SillyTier::Medium},
        {"grief",
         "Get Back on Grief",
         "A chance at death of forcing you to play Grief.\n<cy>Credit: Sweep</c>",
         category::jumpscares,
         SillyTier::High},
        {"congregation",
         "Congregation Jumpscare",
         "A chance at death of forcing you to play Congregation.\n<cy>Credit: StaticGD</c>",
         category::jumpscares,
         SillyTier::High},
        {"math_quiz",
         "Richard's Math Quiz!",
         "When playing a level in practice mode, there's a chance Richard will pop out and give you a quick math quiz. Answer correctly to continue, or restart the level from the beginning.\n<cy>Credit: CyanBoi</c>",
         category::obstructive,
         SillyTier::High},
        {"mock",
         "Mock your 90%+ Fail",
         "Taunts you in the main me with a screenshot of one of your 90%-99% fails.\n<cy>Credit: Wuffin</c>",
         category::misc,
         SillyTier::Medium,
         false,
         {PlatformID::Windows,
          PlatformID::Android}},
        {"freeze",
         "Random 90%+ FPS Drop",
         "Your visual framerate starts randomly dropping during 90-99% in a level.\n<cy>Credit: Hexfire</c>",
         category::randoms,
         SillyTier::Medium},
        {"spam",
         "Spam Jumps!",
         "Forces you to spam an input sometimes while playing a level.\n<cy>Credit: Cheeseworks</c>",
         category::obstructive,
         SillyTier::High},
        {"achieve",
         "Random Achievements",
         "Randomly play the achievement sound when clicking buttons.\n<cy>Credit: Cheeseworks</c>",
         category::randoms,
         SillyTier::Low},
        {"crash_death",
         "Crash Chance on Death",
         "When you die in a level, there's a chance your game will die too.\n<cg>Don't worry, your progress will save when crashing!</c> :)\n<cy>Credit: DragonixGD</c>",
         category::chances,
         SillyTier::High},
        {"confetti",
         "Confetti Explosion",
         "While playing a level, the screen will sometimes cause an explosion of random textures.\n<cy>Credit: Cheeseworks</c>",
         category::obstructive,
         SillyTier::Medium},
        {"no_jump",
         "Randomly Don't Jump",
         "When making an input in a level, there's a chance the player does not respond to it.\n<cy>Credit: GilanyKing12</c>",
         category::randoms,
         SillyTier::Low},
        {"gravity",
         "Randomize Gravity",
         "Every time you jump in a level, gravity force will increase or decrease randomly.\n<cy>Credit: NJAgain</c>",
         category::randoms,
         SillyTier::Low},
        {"death",
         "Fake Death",
         "The player's death effect will show without dying.\n<cy>Credit: DragonixGD</c>",
         category::obstructive,
         SillyTier::Medium},
        {"upside_down",
         "Upside-Down Chance",
         "While navigating the game, there's a chance a layer will be upside-down, and probably break everything...\n<cy>Credit: Cheeseworks</c>",
         category::chances,
         SillyTier::Medium},
        {"ads",
         "Level Ads",
         "While playing a level in normal mode, an ad for a random level will pop up on your screen from time to time.\n<cy>Credit: staticGD</c>",
         category::obstructive,
         SillyTier::Medium},
        {"black_screen",
         "Black Screen Blink",
         "The screen can suddenly blink while playing a level.\n<cy>Credit: elite_smiler_ispro</c>",
         category::obstructive,
         SillyTier::Low},
        {"parry",
         "Parry Obstacles",
         "Whenever your hitbox is inside of a hazard hitbox, you will instead destroy it if you time your input right.\n<cy>Credit: Wuffin</c>",
         category::misc,
         SillyTier::Low,
         true,
         {}},
        {"double_jump",
         "Double-Jump",
         "Allows your character to double-jump in a level.\n<cy>Credit: Cheeseworks</c>",
         category::misc,
         SillyTier::Low},
        {"sleepy",
         "Sleepy Player",
         "Your character will occasionally fall asleep while playing.\n<cy>Credit: this_guy_yt</c>",
         category::misc,
         SillyTier::Low},
        {"pauses",
         "Random Pauses",
         "While playing a level, it will randomly pause itself.\n<cy>Credit: DragonixGD</c>",
         category::randoms,
         SillyTier::Low},
        {"ice_level",
         "Ice Level",
         "Makes every surface icy. Slip n' slide!\n<cy>Credit: TimeRed</c>",
         category::misc,
         SillyTier::Medium},
        {"random_mirror",
         "Random Mirror Portal",
         "Randomly activates a mirror portal while playing.\n<cy>Credit: TimeRed</c>",
         category::randoms,
         SillyTier::Low},
        {"random_speed",
         "Random Speed Change",
         "Randomly changes your speed while playing a level.\n<cy>Credit: imdissapearinghelp</c>",
         category::randoms,
         SillyTier::Medium},
        {"random_icon",
         "Random Icon Change",
         "Randomly change your icon every time you jump.\n<cy>Credit: JompyDoJump</c>",
         category::randoms,
         SillyTier::Low},
        {"blinking_icon",
         "Blinking Icon",
         "Your icon will start to randomly blink.\n<cy>Credit: DragonixGD</c>",
         category::obstructive,
         SillyTier::Low},
        {"dementia",
         "Dementia",
         "Chance of the player randomly teleports back. This is more like player lagging to be honest!\n<cy>Credit: imdissapearinghelp</c>",
         category::misc,
         SillyTier::Medium},
        {"earthquake",
         "Earthquake",
         "Constantly shakes the camera while playing a level.\n<cy>Credit: ArcticWoof</c>",
         category::obstructive,
         SillyTier::Medium},
        {"fake_crash",
         "Random Fake Crash",
         "While playing a level, there's a chance your game will fake a crash.\n<cy>Credit: Timered</c>",
         category::randoms,
         SillyTier::Medium},
        {"gambler",
         "Gambler",
         "While playing a level and reach at the 95%, there's a 50/50 chance you either complete a level or blast into the space backwards.\n<cy>Credit: Timered</c>",
         category::misc,
         SillyTier::Medium},
        {"placebo",
         "Placebo",
         "A random chance that when you start a level, all the options you have enabled are disabled, or all the options you have disabled are enabled.\n<cy>Credit: tmdXD</c>",
         category::misc,
         SillyTier::High},
        {"click_speed",
         "Click Player Speed",
         "Randomly increases or decreases the player speed everytime you jump.\n<cy>Credit: KGplayerA</c>",
         category::randoms,
         SillyTier::Medium},
        {"motivation",
         "Motivational Quotes",
         "You'll get motivational messages while playing! The motivator isn't in the best mood, though...\n<cy>Credit: Cheeseworks</c>",
         category::obstructive,
         SillyTier::Low},
        {"friends",
         "Friends",
         "Random friends fly across your screen while you play a level!\n<cy>Credit: Cheeseworks</c>",
         category::obstructive,
         SillyTier::Medium},
        {"size_changer",
         "Size Changer",
         "Randomly change the player size every time you jump.\n<cy>Credit: himynameisryan21</c>",
         category::randoms,
         SillyTier::Medium},
        {"timewarp_jump",
         "Timewarp Jump",
         "When you jump, there's a chance time will briefly slow down or speed up.\n<cy>Credit: KGplayerA</c>",
         category::randoms,
         SillyTier::Low},
    };
};