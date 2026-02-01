#include "../exercise.h"
#include <cstring>
#include <iostream>
// READ: 类模板 <https://zh.cppreference.com/w/cpp/language/class_template>

template<class T>
struct Tensor4D {
    unsigned int shape[4];
    T *data;

    Tensor4D(unsigned int const shape_[4], T const *data_) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (int i = 0; i < 4; i++) {
            shape[i] = shape_[i];
            size *= shape[i];
        }
        data = new T[size];
        std::memcpy(data, data_, size * sizeof(T));
    }
    
    ~Tensor4D() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor4D(Tensor4D const &) = delete;
    Tensor4D(Tensor4D &&) noexcept = delete;

    // 这个加法需要支持"单向广播"。
    // 具体来说，`others` 可以具有与 `this` 不同的形状，形状不同的维度长度必须为 1。
    // `others` 长度为 1 但 `this` 长度不为 1 的维度将发生广播计算。
    // 例如，`this` 形状为 `[1, 2, 3, 4]`，`others` 形状为 `[1, 2, 1, 4]`，
    // 则 `this` 与 `others` 相加时，3 个形状为 `[1, 2, 1, 4]` 的子张量各自与 `others` 对应项相加。
    Tensor4D &operator+=(Tensor4D const &others) {
        // TODO: 实现单向广播的加法
        
        // 1. 检查形状兼容性
        for (int i = 0; i < 4; i++) {
            // 广播规则：others.shape[i] 必须为 1 或等于 this->shape[i]
            if (others.shape[i] != 1 && others.shape[i] != shape[i]) {
                throw std::invalid_argument("Incompatible shapes for broadcasting");
            }
        }
        
        // 2. 计算总元素数量
        unsigned int total_size = shape[0] * shape[1] * shape[2] * shape[3];
        unsigned int others_total_size = others.shape[0] * others.shape[1] * others.shape[2] * others.shape[3];
        
        // 3. 处理不同的广播情况
        if (others_total_size == 1) {
            // 情况1：全为1的广播（标量）
            T scalar_value = others.data[0];
            for (unsigned int i = 0; i < total_size; i++) {
                data[i] += scalar_value;
            }
        } else {
            // 情况2：部分维度的广播
            // 计算每个维度的步长
            unsigned int stride_this[4];
            unsigned int stride_others[4];
            
            // 计算步长：对于每个维度，步长是后面所有维度大小的乘积
            stride_this[3] = 1;
            stride_others[3] = 1;
            for (int i = 2; i >= 0; i--) {
                stride_this[i] = stride_this[i+1] * shape[i+1];
                stride_others[i] = stride_others[i+1] * others.shape[i+1];
            }
            
            // 嵌套循环遍历所有元素
            for (unsigned int i = 0; i < shape[0]; i++) {
                for (unsigned int j = 0; j < shape[1]; j++) {
                    for (unsigned int k = 0; k < shape[2]; k++) {
                        for (unsigned int l = 0; l < shape[3]; l++) {
                            // 计算当前元素在data数组中的索引
                            unsigned int idx_this = i * stride_this[0] + 
                                                   j * stride_this[1] + 
                                                   k * stride_this[2] + 
                                                   l * stride_this[3];
                            
                            // 计算对应的others元素索引
                            unsigned int idx_others = 0;
                            if (others.shape[0] > 1) idx_others += i * stride_others[0];
                            if (others.shape[1] > 1) idx_others += j * stride_others[1];
                            if (others.shape[2] > 1) idx_others += k * stride_others[2];
                            if (others.shape[3] > 1) idx_others += l * stride_others[3];
                            
                            // 执行加法
                            data[idx_this] += others.data[idx_others];
                        }
                    }
                }
            }
        }
        
        return *this;
    }
    
    // 辅助函数：打印张量信息
    void print_info() const {
        std::cout << "Shape: [" << shape[0] << ", " << shape[1] << ", " 
                  << shape[2] << ", " << shape[3] << "]" << std::endl;
        std::cout << "Data: ";
        unsigned int total_size = shape[0] * shape[1] * shape[2] * shape[3];
        for (unsigned int i = 0; i < total_size && i < 10; i++) {
            std::cout << data[i] << " ";
        }
        if (total_size > 10) std::cout << "...";
        std::cout << std::endl;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    std::cout << "Test 1: Same shape addition" << std::endl;
    {
        unsigned int shape[]{1, 2, 3, 4};
        // clang-format off
        int data[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        auto t0 = Tensor4D(shape, data);
        auto t1 = Tensor4D(shape, data);
        t0 += t1;
        for (auto i = 0u; i < sizeof(data) / sizeof(*data); ++i) {
            ASSERT(t0.data[i] == data[i] * 2, "Tensor doubled by plus its self.");
        }
        std::cout << "Test 1 passed!" << std::endl;
    }
    
    std::cout << "\nTest 2: Broadcast in last dimension" << std::endl;
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        float d0[]{
            1, 1, 1, 1,
            2, 2, 2, 2,
            3, 3, 3, 3,

            4, 4, 4, 4,
            5, 5, 5, 5,
            6, 6, 6, 6};
        // clang-format on
        unsigned int s1[]{1, 2, 3, 1};
        // clang-format off
        float d1[]{
            6,
            5,
            4,

            3,
            2,
            1};
        // clang-format on

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            // 计算预期值：每个元素应该变成7
            // 因为每个位置的d0元素加上对应的d1元素
            // 例如：d0[0]=1 + d1[0]=6 = 7
            // d0[4]=2 + d1[1]=5 = 7，等等
            ASSERT(t0.data[i] == 7.f, "Every element of t0 should be 7 after adding t1 to it.");
        }
        std::cout << "Test 2 passed!" << std::endl;
    }
    
    std::cout << "\nTest 3: Scalar broadcast" << std::endl;
    {
        unsigned int s0[]{1, 2, 3, 4};
        // clang-format off
        double d0[]{
             1,  2,  3,  4,
             5,  6,  7,  8,
             9, 10, 11, 12,

            13, 14, 15, 16,
            17, 18, 19, 20,
            21, 22, 23, 24};
        // clang-format on
        unsigned int s1[]{1, 1, 1, 1};
        double d1[]{1};

        auto t0 = Tensor4D(s0, d0);
        auto t1 = Tensor4D(s1, d1);
        t0 += t1;
        for (auto i = 0u; i < sizeof(d0) / sizeof(*d0); ++i) {
            ASSERT(t0.data[i] == d0[i] + 1, "Every element of t0 should be incremented by 1 after adding t1 to it.");
        }
        std::cout << "Test 3 passed!" << std::endl;
    }
    
    
    
    std::cout << "\nAll tests completed!" << std::endl;
    
    return 0;
}