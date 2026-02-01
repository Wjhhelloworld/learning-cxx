#include "../exercise.h"
#include <vector>

int main(int argc, char **argv) {
    std::vector<bool> vec(100, true);
    ASSERT(vec[0], "Make this assertion pass.");
    ASSERT(vec[99], "Make this assertion pass.");
    ASSERT(vec.size() == 100, "Make this assertion pass.");
    
    vec[20] = false;
    ASSERT(!vec[20], "Fill in `vec[20]` or `!vec[20]`.");
    
    return 0;
}