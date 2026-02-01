#include "../exercise.h"
#include <vector>

int main(int argc, char **argv) {
    std::vector<int> vec{1, 2, 3, 4, 5};
    ASSERT(vec.size() == 5, "Fill in the correct value.");
    
    int ans[]{1, 2, 3, 4, 5};
    ASSERT(std::memcmp(vec.data(), ans, sizeof(ans)) == 0, "Fill in the correct values.");
    
    return 0;
}