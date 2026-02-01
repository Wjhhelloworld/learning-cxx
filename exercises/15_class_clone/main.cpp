#include "../exercise.h"
#include <iostream>
#include <cstddef>  // for size_t
#include <stdexcept>  // for std::out_of_range

// READ: 复制构造函数 <https://zh.cppreference.com/w/cpp/language/copy_constructor>
// READ: 函数定义（显式弃置）<https://zh.cppreference.com/w/cpp/language/function>

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

    // TODO: 实现复制构造器 - 深拷贝
    DynFibonacci(DynFibonacci const &other): 
        cache(new size_t[other.capacity]),  // 分配新的内存
        capacity(other.capacity),           // 复制容量
        cached(other.cached)                // 复制已缓存数量
    {
        // 复制缓存内容
        for (int i = 0; i < cached; ++i) {
            cache[i] = other.cache[i];
        }
        
        // 初始化剩余部分为0
        for (int i = cached; i < capacity; ++i) {
            cache[i] = 0;
        }
    }

    // TODO: 实现析构器，释放缓存空间
    ~DynFibonacci() {
        delete[] cache;  // 释放动态分配的数组
    }

    // 禁止拷贝赋值（避免浅拷贝问题）
    DynFibonacci& operator=(DynFibonacci const &) = delete;

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

    // NOTICE: 不要修改这个方法
    // NOTICE: 名字相同参数也相同，但 const 修饰不同的方法是一对重载方法，可以同时存在
    //         本质上，方法是隐藏了 this 参数的函数
    //         const 修饰作用在 this 上，因此它们实际上参数不同
    size_t get(int i) const {
        if (i < 0 || i >= capacity) {
            throw std::out_of_range("Index out of range");
        }
        
        if (i < cached) {
            return cache[i];
        }
        
        // 对于 const 方法，我们不能修改对象状态
        // 所以只能返回已经缓存的值，否则抛出异常
        throw std::out_of_range("Value not cached yet in const method");
    }

    // 辅助方法：获取容量
    int getCapacity() const {
        return capacity;
    }

    // 辅助方法：获取已缓存数量
    int getCachedCount() const {
        return cached;
    }
};

int main(int argc, char **argv) {
    try {
        DynFibonacci fib(12);
        
        // 先调用一次 get(10) 来填充缓存
        size_t result = fib.get(10);
        ASSERT(result == 55, "fibonacci(10) should be 55");
        
        std::cout << "Original object - fibonacci(10) = " << result << std::endl;
        std::cout << "Original cached count: " << fib.getCachedCount() << std::endl;
        
        // 复制构造器创建新对象
        DynFibonacci const fib_ = fib;  // 调用复制构造函数
        
        // 验证复制后的对象
        size_t result2 = fib_.get(10);
        ASSERT(result2 == fib.get(10), "Object cloned");
        std::cout << "Copied object - fibonacci(10) = " << result2 << std::endl;
        std::cout << "Copied cached count: " << fib_.getCachedCount() << std::endl;
        std::cout << "Copied capacity: " << fib_.getCapacity() << std::endl;
        
        // 测试修改原对象不影响复制后的对象
        std::cout << "\nModifying original object..." << std::endl;
        std::cout << "Original - fibonacci(11) = " << fib.get(11) << std::endl;
        std::cout << "Original cached count now: " << fib.getCachedCount() << std::endl;
        
        // 复制后的对象仍然只能访问到原来的缓存
        std::cout << "Copied - cached count still: " << fib_.getCachedCount() << std::endl;
        
        // 测试 const 方法的边界情况
        try {
            std::cout << "\nTrying to get uncached value from const object..." << std::endl;
            std::cout << "Copied - fibonacci(11) = " << fib_.get(11) << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "Correctly caught exception: " << e.what() << std::endl;
        }
        
        // 验证它们是不同的对象（不同的内存地址）
        // 可以通过其他方式验证，比如分别修改
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}