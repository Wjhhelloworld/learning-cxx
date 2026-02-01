#include "../exercise.h"
#include <iostream>
#include <cstddef>  // for size_t
#include <stdexcept>  // for std::out_of_range
#include <utility>    // for std::swap

// READ: 左值右值（概念）<https://learn.microsoft.com/zh-cn/cpp/c-language/l-value-and-r-value-expressions?view=msvc-170>
// READ: 左值右值（细节）<https://zh.cppreference.com/w/cpp/language/value_category>
// READ: 关于移动语义 <https://learn.microsoft.com/zh-cn/cpp/cpp/rvalue-reference-declarator-amp-amp?view=msvc-170#move-semantics>
// READ: 如果实现移动构造 <https://learn.microsoft.com/zh-cn/cpp/cpp/move-constructors-and-move-assignment-operators-cpp?view=msvc-170>

// READ: 移动构造函数 <https://zh.cppreference.com/w/cpp/language/move_constructor>
// READ: 移动赋值 <https://zh.cppreference.com/w/cpp/language/move_assignment>
// READ: 运算符重载 <https://zh.cppreference.com/w/cpp/language/operators>

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

    // 默认构造函数（可选，如果需要的话）
    DynFibonacci() : cache(nullptr), capacity(0), cached(0) {}

    // 析构函数
    ~DynFibonacci() {
        delete[] cache;  // 释放动态分配的数组
    }

    // 删除拷贝构造函数和拷贝赋值
    DynFibonacci(DynFibonacci const &) = delete;
    DynFibonacci &operator=(DynFibonacci const &) = delete;

    // TODO: 实现移动构造器
    DynFibonacci(DynFibonacci &&other) noexcept 
        : cache(other.cache), 
          capacity(other.capacity), 
          cached(other.cached) 
    {
        // 将源对象置为有效但空的状态
        other.cache = nullptr;
        other.capacity = 0;
        other.cached = 0;
    }

    // TODO: 实现移动赋值
    // NOTICE: ⚠ 注意移动到自身问题 ⚠
    DynFibonacci &operator=(DynFibonacci &&other) noexcept {
        // 检查自赋值
        if (this != &other) {
            // 释放当前资源
            delete[] cache;
            
            // 转移资源所有权
            cache = other.cache;
            capacity = other.capacity;
            cached = other.cached;
            
            // 将源对象置为有效但空的状态
            other.cache = nullptr;
            other.capacity = 0;
            other.cached = 0;
        }
        return *this;
    }

    // TODO: 实现正确的缓存优化斐波那契计算
    size_t operator[](int i) {
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
    size_t operator[](int i) const {
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

    // NOTICE: 不要修改这个方法
    bool is_alive() const {
        return cache != nullptr;  // 检查指针是否为空
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
        // 测试1：基本功能
        DynFibonacci fib(12);
        ASSERT(fib[10] == 55, "fibonacci(10) should be 55");
        std::cout << "Test 1 passed: fib[10] = " << fib[10] << std::endl;

        // 测试2：移动构造
        DynFibonacci const fib_ = std::move(fib);
        ASSERT(!fib.is_alive(), "Object moved");
        ASSERT(fib_[10] == 55, "fibonacci(10) should be 55");
        std::cout << "Test 2 passed: fib moved to fib_" << std::endl;
        std::cout << "  fib.is_alive() = " << fib.is_alive() << std::endl;
        std::cout << "  fib_.is_alive() = " << fib_.is_alive() << std::endl;
        std::cout << "  fib_[10] = " << fib_[10] << std::endl;

        // 测试3：移动赋值和自移动赋值
        DynFibonacci fib0(6);
        DynFibonacci fib1(12);
        
        // 先填充fib1的缓存
        size_t val10 = fib1[10];
        std::cout << "\nBefore move assignment:" << std::endl;
        std::cout << "  fib0[5] = " << fib0[5] << ", capacity = " << fib0.getCapacity() << std::endl;
        std::cout << "  fib1[10] = " << val10 << ", capacity = " << fib1.getCapacity() << std::endl;
        
        // 移动赋值
        fib0 = std::move(fib1);
        std::cout << "\nAfter move assignment:" << std::endl;
        std::cout << "  fib0[10] = " << fib0[10] << ", capacity = " << fib0.getCapacity() << std::endl;
        std::cout << "  fib1.is_alive() = " << fib1.is_alive() << std::endl;
        
        // 自移动赋值
        fib0 = std::move(fib0);  // 应该安全处理
        ASSERT(fib0[10] == 55, "fibonacci(10) should be 55");
        std::cout << "Test 3 passed: move assignment and self-move assignment" << std::endl;
        
        // 测试4：验证移动后原对象不可用但有效
        std::cout << "\nTest 4: State after move:" << std::endl;
        std::cout << "  fib1.capacity = 0? " << (fib1.getCapacity() == 0) << std::endl;
        std::cout << "  fib1.cached = 0? " << (fib1.getCachedCount() == 0) << std::endl;
        
        // 测试5：从临时对象移动构造
        DynFibonacci fib2 = DynFibonacci(8);
        ASSERT(fib2[5] == 5, "fibonacci(5) should be 5");
        std::cout << "\nTest 5 passed: move from temporary" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}