# [<img src="logo.png" width="25" alt="The mod's logo." />](https://www.geode-sdk.org/mods/arcticwoof.horrible_ideas) Horrible Ideas
A plethora of ways to ruin your gaming experience...

> [<img alt="Latest Version" src="https://img.shields.io/github/v/release/DumbCaveSpider/HorribleIdeas?include_prereleases&sort=semver&display_name=release&style=for-the-badge&logo=github&logoColor=ffffff&label=Version">](../../releases/) [<img alt="Code License" src="https://img.shields.io/github/license/DumbCaveSpider/HorribleIdeas?style=for-the-badge&logo=gnu&logoColor=ffffff&label=License">](LICENSE.md)
>  
> [<img alt="Downloads" src="https://img.shields.io/github/downloads/DumbCaveSpider/HorribleIdeas/total?style=for-the-badge&logo=geode&logoColor=ffffff&label=Downloads">](https://www.geode-sdk.org/mods/arcticwoof.horrible_ideas)

---

## About
This silly lil' mod adds a mod menu filled to the brim with joke mods! Spice up your gameplay by adding some truly terrible features to absolutely wreck your entire game. There's plenty to pick from, and you absolutely will NOT be disappointed at all!

---

### Options
When pressing `Tab`, a menu will pop up with a list of joke mod options you can toggle anytime on your game to do some interesting things to your gaming experience. From random mirror portals to game-breaking glitches! You can view more information within the contents of the menu itself.

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
Want to add your own insane stuff to this mod? You can register your very own horrible options by using this mod's API! You can see its [documentation here](./include). We're hyped to see how much more you can really mess up this game.

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

> [!NOTE]
> *If you plan on publishing a mod that acts as an add-on, all we ask is to please be sure to follow the safe code practices in the [documentation](./include) as best as you can!*

---

### [<img alt="Trailer Likes" height="15" src="https://img.shields.io/youtube/likes/PI_poDUUauw?style=for-the-badge&logo=youtube">](https://www.youtube.com/watch?v=PI_poDUUauw&list=PL0dsSu2pR5cERnq7gojZTKVRvUwWo2Ohu) Watch the [Horrible Mods](https://www.youtube.com/watch?v=Ssl49pNmW_0&list=PL0dsSu2pR5cERnq7gojZTKVRvUwWo2Ohu) series out now on YouTube!