#include "../exercise.h"
#include <memory>

int main(int argc, char **argv) {
    auto shared = std::make_shared<int>(10);
    std::shared_ptr<int> ptrs[]{shared, shared, shared};
    std::weak_ptr<int> observer = shared;
    
    ASSERT(observer.use_count() == 4, "");
    ptrs[0].reset();
    ASSERT(observer.use_count() == 3, "");
    ptrs[1] = nullptr;
    ASSERT(observer.use_count() == 2, "");
    
    return 0;
}