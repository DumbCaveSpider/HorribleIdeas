#include "../Options.hpp"

#include <Horrible.hpp>
#include <HorribleIdeas.hpp>

using namespace horrible;
using namespace horribleideas;

namespace horrible {
    std::vector<Option> options::getAll() {
        return getRegisteredOptions();
    };
};