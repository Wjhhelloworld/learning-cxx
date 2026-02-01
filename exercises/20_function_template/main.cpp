#include "../exercise.h"
#include <iostream>
#include <cmath>  // for fabs
#include <type_traits>  // for std::is_same_v

// READ: 函数模板 <https://zh.cppreference.com/w/cpp/language/function_template>

// TODO: 将这个函数模板化
// 创建一个函数模板，支持不同类型的加法
template<typename T>
T plus(const T& a, const T& b) {
    return a + b;
}

// 针对浮点数的特化版本，允许不同类型浮点数的混合运算
template<typename T1, typename T2>
auto plus(const T1& a, const T2& b) -> decltype(a + b) {
    return a + b;
}

// 用于浮点数比较的辅助函数
template<typename T>
bool float_equal(T a, T b, T epsilon = std::numeric_limits<T>::epsilon() * 10) {
    // 如果两个数非常接近，直接返回 true
    if (std::fabs(a - b) <= epsilon) {
        return true;
    }
    
    // 处理接近0的情况
    if (a == 0 || b == 0) {
        return std::fabs(a - b) <= epsilon;
    }
    
    // 使用相对误差
    return std::fabs((a - b) / std::max(std::fabs(a), std::fabs(b))) <= epsilon;
}

int main(int argc, char **argv) {
    // 整数测试
    ASSERT(plus(1, 2) == 3, "Plus two int");
    ASSERT(plus(1u, 2u) == 3u, "Plus two unsigned int");
    
    // 浮点数直接比较（精确值）
    ASSERT(plus(1.25f, 2.5f) == 3.75f, "Plus two float");
    ASSERT(plus(1.25, 2.5) == 3.75, "Plus two double");
    
    // THINK: 浮点数何时可以判断 ==？何时必须判断差值？
    // 浮点数比较的黄金法则：永远不要直接比较浮点数是否相等！
    // 0.1 和 0.2 在二进制浮点数中无法精确表示
    // 0.1 + 0.2 != 0.3 在二进制浮点数中是正常的
    
    // TODO: 修改判断条件使测试通过
    // 使用误差范围内的比较
    double result = plus(0.1, 0.2);
    double expected = 0.3;
    
    // 方法1：使用绝对误差比较
    const double eps_abs = 1e-10;
    ASSERT(std::fabs(result - expected) < eps_abs, 
           "How to make this pass? - Using absolute epsilon");
    
    // 方法2：使用相对误差比较（更通用）
    const double eps_rel = 1e-9;
    double diff = std::fabs(result - expected);
    double max_val = std::max(std::fabs(result), std::fabs(expected));
    ASSERT(diff < eps_rel * max_val, 
           "How to make this pass? - Using relative epsilon");
    
    // 方法3：使用我们定义的辅助函数
    ASSERT(float_equal(result, expected), 
           "How to make this pass? - Using float_equal helper");
    
    // 测试不同类型的混合运算
    std::cout << "Testing mixed types:" << std::endl;
    std::cout << "plus(1.5f, 2) = " << plus(1.5f, 2) << std::endl;
    std::cout << "plus(3, 4.2) = " << plus(3, 4.2) << std::endl;
    
    // 测试浮点数精度问题
    std::cout << "\nDemonstrating floating-point precision issues:" << std::endl;
    double a = 0.1;
    double b = 0.2;
    double sum = plus(a, b);
    std::cout << "0.1 + 0.2 = " << sum << std::endl;
    std::cout << "0.1 + 0.2 == 0.3? " << (sum == 0.3) << std::endl;
    std::cout << "std::numeric_limits<double>::epsilon() = " 
              << std::numeric_limits<double>::epsilon() << std::endl;
    std::cout << "Difference: " << std::fabs(sum - 0.3) << std::endl;
    
    // 验证模板可以处理更多类型
    std::cout << "\nTesting with different types:" << std::endl;
    
    // 整型
    int int_result = plus(10, 20);
    std::cout << "plus(10, 20) = " << int_result << std::endl;
    
    // 无符号整型
    unsigned uint_result = plus(10u, 20u);
    std::cout << "plus(10u, 20u) = " << uint_result << std::endl;
    
    // 浮点数
    float float_result = plus(1.5f, 2.5f);
    std::cout << "plus(1.5f, 2.5f) = " << float_result << std::endl;
    
    // 双精度
    double double_result = plus(1.234, 2.345);
    std::cout << "plus(1.234, 2.345) = " << double_result << std::endl;
    
    return 0;
}