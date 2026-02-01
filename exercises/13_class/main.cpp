#include "../exercise.h"

class DynFibonacci {
    size_t *cache;
    int capacity;
    int cached;

public:
    DynFibonacci(int cap) : cache(new size_t[cap]), capacity(cap), cached(0) {
        if (capacity >= 1) { cache[0] = 0; cached = 1; }
        if (capacity >= 2) { cache[1] = 1; cached = 2; }
    }
    
    ~DynFibonacci() { delete[] cache; }
    
    size_t get(int i) {
        if (i < 0 || i >= capacity) throw std::out_of_range("Index out of range");
        
        while (cached <= i) {
            if (cached == 0) { cache[0] = 0; cached = 1; }
            else if (cached == 1) { cache[1] = 1; cached = 2; }
            else { cache[cached] = cache[cached - 1] + cache[cached - 2]; ++cached; }
        }
        
        return cache[i];
    }
};

int main(int argc, char **argv) {
    DynFibonacci fib(12);
    ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
    std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
    return 0;
}