#include "../exercise.h"
#include <array>

int main(int argc, char **argv) {
    std::array<int, 5> arr{{1, 2, 3, 4, 5}};
    ASSERT(arr.size() == 5, "Fill in the correct value.");
    ASSERT(sizeof(arr) == 5 * sizeof(int), "Fill in the correct value.");
    
    int ans[]{1, 2, 3, 4, 5};
    ASSERT(std::memcmp(arr.data(), ans, arr.size() * sizeof(int)) == 0, "Fill in the correct values.");
    
    return 0;
}