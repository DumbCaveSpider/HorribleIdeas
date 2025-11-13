#pragma once

#ifdef GEODE_IS_WINDOWS
#ifdef AWCW_HORRIBLE_API_EXPORTING
#define AWCW_HORRIBLE_API_DLL __declspec(dllexport)
#else
#define AWCW_HORRIBLE_API_DLL __declspec(dllimport)
#endif
#else
#ifdef AWCW_HORRIBLE_API_EXPORTING
#define AWCW_HORRIBLE_API_DLL __attribute__((visibility("default")))
#else
#define AWCW_HORRIBLE_API_DLL
#endif
#endif

#include <Geode/Geode.hpp>

using namespace geode::prelude;

// Namespace with all horrible option functions
namespace horribleideas {
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

    // How silly a mod option is
    enum class AWCW_HORRIBLE_API_DLL SillyTier {
        None = 0,   // Null
        Low = 1,    // Not so silly
        Medium = 2, // Somewhat silly
        High = 3    // Very silly
    };

    // A horrible option
    struct AWCW_HORRIBLE_API_DLL Option {
        std::string id; // Unique ID of the option
        std::string name; // Name of the option
        std::string description; // Description of the option
        std::string category; // Name of the category this option should be under
        SillyTier silly; // How silly the option is
        bool restart = false; // If the option requires a restart to take effect
        std::vector<PlatformID> platforms = { PlatformID::Desktop, PlatformID::Mobile }; // Platforms that the option supports

        // Full constructor second
        Option(
            std::string id,
            std::string name,
            std::string description,
            std::string category,
            SillyTier silly,
            bool restart = false,
            std::vector<PlatformID> platforms = { PlatformID::Desktop, PlatformID::Mobile }) : id(std::move(id)),
            name(std::move(name)),
            description(std::move(description)),
            category(std::move(category)),
            silly(silly),
            restart(restart),
            platforms(std::move(platforms)) {};
    };

    // Event for option toggles
    class HorribleOptionEvent : public Event {
    protected:
        std::string id;
        bool isToggled;

    public:
        HorribleOptionEvent(std::string id, bool isToggled);

        AWCW_HORRIBLE_API_DLL std::string getId() const; // Get the unique ID of the option
        AWCW_HORRIBLE_API_DLL bool getIsToggled() const; // Get the toggle boolean of the option
    };

    // Filter for option toggle event
    class AWCW_HORRIBLE_API_DLL HorribleOptionEventFilter : public EventFilter<HorribleOptionEvent> {
    public:
        using Callback = ListenerResult(HorribleOptionEvent*);

        /**
         * Event handler
         *
         * @param fn Callback function containing a pointer to the event that fired
         * @param event Pointer to the event that fired
         */
        ListenerResult handle(std::function<Callback> fn, HorribleOptionEvent* event) {
            return fn(event);
        };
    };

    // All horrible mods
    static inline std::vector<Option> m_options = {
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
        {"meme_death",
         "Meme Death Sounds",
         "Plays a meme sound effect on certain percentage where you died at.\n<cy>Credit: imdissapearinghelp</c>",
         Category::misc,
         SillyTier::Low},
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
        {"flashbang",
         "Flashbang",
         "Randomly blinds you for a split second while playing a level.\n<cy>Credit: tmdX3</c>",
         Category::obstructive,
         SillyTier::Medium},
        {
            "gambler",
            "Gambler",
            "While playing a level and reach at the 95%, there's a 50/50 chance you either complete the level or blast into the space backwards.\n<cy>Credit: Timered</c>",
            Category::misc,
            SillyTier::Medium,
        },
        {"placebo",
         "Placebo",
         "A small chance that when you start a level, all the horrible mods you have enabled are disabled, or all the horrible mods you have disabled are enabled.\n<cy>Credit: tmdXD</c>",
         Category::misc,
         SillyTier::High},
        {"click_speed",
         "Click Player Speed",
         "Randomly increases or decreases your player speed everytime you jump.\n<cy>Credit: KGplayerA</c>",
         Category::randoms,
         SillyTier::Medium} };

    /**
     * Returns if the mod is enabled or disabled
     *
     * @param id The ID of the option to check
     *
     * @returns Boolean of the current value
     */
    AWCW_HORRIBLE_API_DLL bool get(std::string_view id);

    /**
     * Returns the chance value for an option
     *
     * @param id The ID of the option to check
     *
     * @returns Integer of the chance value
     */
    int getChance(std::string_view id);

    /**
     * Returns if the mod is enabled or disabled
     *
     * @param id The ID of the option to check
     * @param enable True to enable the option, false to disable
     *
     * @returns Boolean of the old value
     */
    bool set(std::string_view id, bool enable);

    /**
     * Register an option during load
     *
     * @param option The constructed option
     */
    AWCW_HORRIBLE_API_DLL void registerOption(const Option& option);

    /**
     * Returns a snapshot of all registered options
     *
     * @returns An array of every option, main and external
     */
    std::vector<Option> getRegisteredOptions();
};