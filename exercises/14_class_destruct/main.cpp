#include "../exercise.h"
#include <iostream>
#include <cstddef>  // for size_t
#include <stdexcept>  // for std::out_of_range

// READ: 析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>
// READ: RAII <https://learn.microsoft.com/zh-cn/cpp/cpp/object-lifetime-and-resource-management-modern-cpp?view=msvc-170>

/// @brief 任意缓存容量的斐波那契类型。
/// @details 可以在构造时传入缓存容量，因此需要动态分配缓存空间。
class DynFibonacci {
    size_t *cache;
    int capacity;  // 缓存容量
    int cached;    // 已缓存的数量

public:
    // TODO: 实现动态设置容量的构造器
    DynFibonacci(int cap): 
        cache(new size_t[cap]),  // 动态分配数组
        capacity(cap),           // 保存容量
        cached(0)                // 初始时没有缓存任何值
    {
        // 检查容量是否有效
        if (cap <= 0) {
            throw std::invalid_argument("Capacity must be positive");
        }
        
        // 初始化缓存数组为0
        for (int i = 0; i < capacity; ++i) {
            cache[i] = 0;
        }
        
        // 初始化前两个斐波那契数
        if (capacity >= 1) {
            cache[0] = 0;
            cached = 1;
        }
        if (capacity >= 2) {
            cache[1] = 1;
            cached = 2;
        }
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;  // 释放动态分配的数组
    }

    // 禁止拷贝构造和拷贝赋值（避免浅拷贝问题）
    DynFibonacci(const DynFibonacci&) = delete;
    DynFibonacci& operator=(const DynFibonacci&) = delete;

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t get(int i) {
        // 检查索引是否在有效范围内
        if (i < 0 || i >= capacity) {
            throw std::out_of_range("Index out of range");
        }
        
        // 如果请求的值还没有被缓存，先计算并缓存
        while (cached <= i) {
            if (cached == 0) {
                cache[0] = 0;
                cached = 1;
            } else if (cached == 1) {
                cache[1] = 1;
                cached = 2;
            } else {
                cache[cached] = cache[cached - 1] + cache[cached - 2];
                ++cached;
            }
        }
        
        return cache[i];
    }

    // 可选：添加获取容量的方法
    int getCapacity() const {
        return capacity;
    }

    // 可选：添加获取已缓存数量的方法
    int getCachedCount() const {
        return cached;
    }
};

int main(int argc, char **argv) {
    try {
        DynFibonacci fib(12);
        ASSERT(fib.get(10) == 55, "fibonacci(10) should be 55");
        std::cout << "fibonacci(10) = " << fib.get(10) << std::endl;
        
        // 测试缓存功能
        std::cout << "Capacity: " << fib.getCapacity() << std::endl;
        std::cout << "Cached count: " << fib.getCachedCount() << std::endl;
        
        // 测试其他值
        std::cout << "fibonacci(0) = " << fib.get(0) << std::endl;
        std::cout << "fibonacci(1) = " << fib.get(1) << std::endl;
        std::cout << "fibonacci(5) = " << fib.get(5) << std::endl;
        std::cout << "fibonacci(11) = " << fib.get(11) << std::endl;
        
        // 测试异常处理
        try {
            std::cout << "fibonacci(12) = " << fib.get(12) << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "Correctly caught exception: " << e.what() << std::endl;
        }
        
        // 测试不同容量的实例
        DynFibonacci fib2(5);
        std::cout << "\nSmaller cache (capacity=5):" << std::endl;
        std::cout << "fibonacci(4) = " << fib2.get(4) << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}