# [<img src="../logo.png" width="30" alt="The mod's logo." />](https://www.geode-sdk.org/mods/arcticwoof.horrible_ideas) Horrible Ideas

## Development
You can directly access the Horrible Ideas mod menu API by including the [`HorribleIdeas.hpp`](HorribleIdeas.hpp) file in your code. Make sure to include the **`horribleideas`** namespace to directly access all needed classes and methods.
```cpp
#include <arcticwoof.horrible_ideas/include/HorribleIdeas.hpp>

using namespace horribleideas;
```

### Classes
Here are some important classes we highly suggest you keep in mind while working with the API.

#### enum class `horribleideas::SillyTier`
An enum class that defines how chaotic or funny an option is.

#### class `geode::PlatformID`
A dynamic enum class provided by Geode that defines the player's current platform.

#### struct `horribleideas::Option`
The object structure of an option.
- `std::string` **`id`**: Unique ID of the option
- `std::string` **`name`**: Name of the option
- `std::string` **`description`**: Description of the option
- `std::string` **`category`**: Name of the category this option should be under
- `SillyTier` **`silly`**: How silly the option is
- `bool` **`restart`** (`false`): If the option requires a restart to take effect
- `std::vector<PlatformID>` **`platforms`** (`{ PlatformID::Desktop, PlatformID::Mobile }`): Platforms that the option supports

#### class `horribleideas::HorribleOptionEvent`
An event that fires any time any option is changed.
- `std::string` **`getId()`**: Get the unique ID of the option
- `bool` **`getIsToggled()`**: Get the toggle boolean of the option

#### class `horribleideas::HorribleOptionEventFilter`
- `ListenerResult` **`handle(std::function<Callback> fn, HorribleOptionEvent* event)`**: Event handler
  - `std::function<Callback>` **`fn`**: Callback function containing a pointer to the event that fired
  - `HorribleOptionEvent*` **`event`**: Pointer to the event that fired

#### Summary
| Type         | Name                        | Description                          |
| ------------ | --------------------------- | ------------------------------------ |
| `enum class` | `SillyTier`                 | Defines how silly an option is       |
| `struct`     | `Option`                    | Represents a toggleable option       |
| `class`      | `HorribleOptionEvent`       | Fired when an option is toggled      |
| `class`      | `HorribleOptionEventFilter` | Filters through option toggle events |

### Options
You can register and check any and as many options as you desire through this API.

#### Registering
This mod makes it easy for players to access the options they want to use. You can register your own options by using the **`horribleideas::registerOption`** method inside an `$execute` block. You will need to pass one parameter, which is an **`Option`** struct for the option you want to register.

*Required fields of the **`Option`** struct are, in order: `id`, `name`, `description`, `category`, and `silly`. Optional fields are `restart` and `platforms`.*

> [!TIP]
> Be sure to prefix your option's unique ID with your Geode mod ID by appending **`_spr`** after the end of the string to prevent conflicts with this mod or other mods that may also register options with possibly identical IDs.

```cpp
$execute{
    registerOption({
        "something-interesting"_spr,
        "Something Interesting",
        "This is something that is very interesting.",
        "Stuff!",
        SillyTier::Medium
        });
};
```

You can include optional fields **`restart`** and **`platforms`** as well! The array for `platforms` uses Geode's dynamic **`PlatformID`** class to identify the exact platform the player is running Geometry Dash on. By default, Horrible Ideas sets every option to be compatible for `PlatformID::Desktop` and `PlatformID::Mobile`, essentially covering all platforms. However, you can also get very specific about the exact platform you can run your own options on if absolutely necessary, though such a case may not present itself often.

> [!IMPORTANT]
> Even if `restart` is **enabled** for your option, the global event for it *will still fire* whenever the player changes it mid-game. What this setting does is actually just notify the player that your option will only load after they restart the game.

```cpp
$execute{
    registerOption({
        "cool-things"_spr,
        "Cool Things",
        "Some really really cool things.",
        "Stuff!",
        SillyTier::Low,
        true, // Cannot apply until after restart
        {
            PlatformID::Android32,
            PlatformID::X64 // Support certain platforms
        }
        });
};
```

This will automatically include your option in Horrible Ideas's pre-existing list of options, and will appear in the menu for the player whenever they open it.

#### Handling
Once you've registered an option on `$execute`, you can use other methods to work with the option.

##### Static Conditioning
You can begin by using **`horribleideas::get`** and provide your option's unique ID to check if an option is enabled or disabled.
```cpp
using namespace geode::prelude;
using namespace horribleideas;

class $modify(CoolThingsPlayLayer, PlayLayer) {
    struct Fields {
        bool enabled = horribleideas::get("cool-things"_spr); // If this option is set to true or false
    };

    bool init(GJGameLevel * level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;

        if (m_fields->enabled) {
            // do stuff!
        };

        return true;
    };
};
```

##### Events
If you would like or need to re-implement or remove an option's functionality live as the player clicks on the toggle for that option, you can listen to the **`HorribleOptionEvent`** global event, which fires any time any option is toggled.

> [!IMPORTANT]
> Be sure to return `ListenerResult::Propagate` after you're finished handling each event to ensure other remaining event instances function properly.

```cpp
using namespace geode::prelude;
using namespace horribleideas;

class $modify(SomethingInterestingMenuLayer, MenuLayer) {
    struct Fields {
        EventListener<HorribleOptionEventFilter> m_horribleListener; // Listen to any options being toggled
    };

    bool init() {
        if (!MenuLayer::init()) return false;

        // implemented logic here

        m_fields->m_horribleListener = {
            [=](HorribleOptionEvent* event) {
                if (event->getId() != "something-interesting"_spr) return ListenerResult::Propagate; 
                
                if (event->getIsToggled()) {
                    // handle re-implementation here
                };

                return ListenerResult::Propagate;
            },
            HorribleOptionEventFilter()
        };

        return true;
    };
};
```

### Watch Out!
Some common pitfalls may include the following.
- Forgetting to use `_spr` when defining unique option ID
- Not returning `ListenerResult::Propagate` in event callbacks
- Registering options outside the `$execute` block

Always double-check your code to make sure it follows safe practices.

*Happy modding!*