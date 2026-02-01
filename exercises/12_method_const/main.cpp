#include "../exercise.h"

class Fibonacci {
    size_t cache[16];
    int cached;

public:
    Fibonacci() : cached(2) {
        cache[0] = 0;
        cache[1] = 1;
        for (int i = 2; i < 16; ++i) cache[i] = 0;
    }

    size_t get(int i) {
        if (i < 0 || i >= 16) return 0;
        
        while (cached <= i) {
            cache[cached] = cache[cached - 1] + cache[cached - 2];
            ++cached;
        }
        
        return cache[i];
    }
};

int main(int argc, char **argv) {
    Fibonacci fib;
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}