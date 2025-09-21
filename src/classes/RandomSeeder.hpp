#include <ctime>
#include <cstdlib>
#include <mutex>

namespace horrible {
    struct RandomSeeder {
        RandomSeeder() {
            static std::once_flag seededFlag;
            std::call_once(seededFlag, []() { std::srand(static_cast<unsigned>(std::time(nullptr))); });
        };
    };
};