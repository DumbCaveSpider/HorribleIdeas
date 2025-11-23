# Horrible Ideas
**A plethora of ways to ruin your experience...**

*by [ArcticWoof](user:7689052) & [Cheeseworks](user:6408873)!*

---

## About
This silly lil' mod adds a mod menu filled to the brim with crazy joke mods! Spice up your gameplay by adding some truly terrible features to absolutely wreck your entire game. There's plenty to pick from, and you absolutely will NOT be disappointed at all!

---

### Options
When pressing `Tab` or by pressing the button which can be found in a number of places in the game, a menu will pop up with a list of horrible mod options you can toggle anytime on your game to do some interesting things to your gaming experience. From random mirror portals to game-breaking glitches! You can view more information within the contents of the menu itself.

#### Player Life
Give the player a limited health-like meter that must always stay above 0 to prevent the player from dying.

#### Jumpscares
Typically give a chance to teleport you to a whole different level, mid-level. Boo. Haha.

#### Randoms
Minor but possibly devastating inconveniences that just pop in from time to time.

#### Chances
Trolls that usually happen on some sort of player interaction.

#### Obstructive
Disturb the player's accessibility to the gameplay.

---

### Developers
Want to add your own insane stuff to this mod? You can register your very own horrible options by using this mod's API! You can see its [documentation here](https://github.com/DumbCaveSpider/HorribleIdeas/tree/main/include). We're hyped to see how much more you can really mess up this game.

```cpp
using namespace horribleideas;

$execute {
    auto optionManager = OptionManager::get();

    optionManager->registerOption({
        "my-option"_spr,
        "My Very Cool Option!",
        "This option is so very cool!",
        "Cool Options",
        SillyTier::Medium
    });
};
```

> ![icon](frame:GJ_infoIcon_001.png?scale=0.5) <cj>*If you plan on publishing a mod that acts as an add-on to Horrible Ideas, all we ask is to please be sure to follow the safe code practices in the [documentation](https://github.com/DumbCaveSpider/HorribleIdeas/tree/main/include) as best as you can!*</c>

---

**![YT](frame:gj_ytIcon_001.png?scale=0.375) Watch the [Horrible Mods](https://www.youtube.com/watch?v=Ssl49pNmW_0&list=PL0dsSu2pR5cERnq7gojZTKVRvUwWo2Ohu) series out now on YouTube!**