#include <ctime>
#include <cstdlib>

struct RandomSeeder {
    RandomSeeder() { srand(time(0)); };
};