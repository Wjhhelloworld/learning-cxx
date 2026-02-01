#include "../exercise.h"
#include <cmath>
#include <iostream>

enum class DataType {
    Float,
    Double,
};

/// @brief Tagged union 即标签化联合体，是联合体的一种常见应用。
///        Rust enum 在实现上就是标签化联合体。
struct TaggedUnion {
    DataType type;
    // NOTICE: struct/union 可以相互任意嵌套。
    union {
        float f;
        double d;
    };
};

// TODO: 将这个函数模板化用于 sigmoid_dyn
// 原始 sigmoid 函数模板化
template<typename T>
T sigmoid(T x) {
    return static_cast<T>(1) / (static_cast<T>(1) + std::exp(-x));
}

// 模板特化用于不同数据类型（可选）
template<>
float sigmoid<float>(float x) {
    return 1.0f / (1.0f + std::expf(-x));  // 使用单精度版本
}

template<>
double sigmoid<double>(double x) {
    return 1.0 / (1.0 + std::exp(-x));  // 使用双精度版本
}

// 动态分发的 sigmoid 函数
TaggedUnion sigmoid_dyn(TaggedUnion x) {
    TaggedUnion ans{x.type};  // 保持相同的类型
    
    // TODO: 根据 type 调用 sigmoid
    switch (x.type) {
        case DataType::Float:
            ans.f = sigmoid(x.f);  // 调用 float 版本
            break;
        case DataType::Double:
            ans.d = sigmoid(x.d);  // 调用 double 版本
            break;
    }
    
    return ans;
}

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    // 测试 float 类型
    TaggedUnion xf{DataType::Float};
    xf.f = 5.f;
    auto yf = sigmoid_dyn(xf);
    ASSERT(yf.type == DataType::Float, "type mismatch");
    ASSERT(yf.f == 1.f / (1.f + std::exp(-5.f)), "sigmoid float");
    std::cout << "Float test passed: sigmoid(5.0f) = " << yf.f << std::endl;
    
    // 测试 double 类型
    TaggedUnion xd{DataType::Double};
    xd.d = 5.0;
    auto yd = sigmoid_dyn(xd);
    ASSERT(yd.type == DataType::Double, "type mismatch");
    ASSERT(yd.d == 1.0 / (1.0 + std::exp(-5.0)), "sigmoid double");
    std::cout << "Double test passed: sigmoid(5.0) = " << yd.d << std::endl;
    
    // 额外测试：验证模板函数的通用性
    std::cout << "\nTesting template function directly:" << std::endl;
    
    // 测试 float
    float f_input = 2.5f;
    float f_result = sigmoid(f_input);
    std::cout << "sigmoid(" << f_input << "f) = " << f_result << std::endl;
    
    // 测试 double
    double d_input = 2.5;
    double d_result = sigmoid(d_input);
    std::cout << "sigmoid(" << d_input << ") = " << d_result << std::endl;
    
    // 测试不同值
    std::cout << "\nSigmoid function behavior:" << std::endl;
    std::cout << "sigmoid(0.0) = " << sigmoid(0.0) << std::endl;
    std::cout << "sigmoid(1.0) = " << sigmoid(1.0) << std::endl;
    std::cout << "sigmoid(-1.0) = " << sigmoid(-1.0) << std::endl;
    std::cout << "sigmoid(10.0) = " << sigmoid(10.0) << std::endl;
    std::cout << "sigmoid(-10.0) = " << sigmoid(-10.0) << std::endl;
    
    return 0;
}