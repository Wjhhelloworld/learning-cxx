#include "../exercise.h"

struct FibonacciCache {
    unsigned long long cache[16];
    int cached;
};

static unsigned long long fibonacci(FibonacciCache &cache, int i) {
    if (i < 0 || i >= 16) return 0;
    
    while (cache.cached <= i) {
        if (cache.cached == 0) {
            cache.cache[0] = 0;
            cache.cached = 1;
        } else if (cache.cached == 1) {
            cache.cache[1] = 1;
            cache.cached = 2;
        } else {
            cache.cache[cache.cached] = cache.cache[cache.cached - 1] + cache.cache[cache.cached - 2];
            ++cache.cached;
        }
    }
    
    return cache.cache[i];
}

int main(int argc, char **argv) {
    FibonacciCache fib{};
    ASSERT(fibonacci(fib, 10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fibonacci(fib, 10) << std::endl;
    return 0;
}