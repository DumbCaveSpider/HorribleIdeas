#include <ctime>
#include <cstdlib>

namespace horrible {
    struct RandomSeeder {
        RandomSeeder() { srand(time(0)); };
    };
};