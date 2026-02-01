#include "../exercise.h"
#include <cstring>
#include <iostream>

// READ: 模板非类型实参 <https://zh.cppreference.com/w/cpp/language/template_parameters#%E6%A8%A1%E6%9D%BF%E9%9D%9E%E7%B1%BB%E5%9E%8B%E5%AE%9E%E5%8F%82>

template<unsigned int N, class T>
struct Tensor {
    unsigned int shape[N];
    T *data;

    Tensor(unsigned int const shape_[N]) {
        unsigned int size = 1;
        // TODO: 填入正确的 shape 并计算 size
        for (unsigned int i = 0; i < N; ++i) {
            shape[i] = shape_[i];
            size *= shape[i];  // 计算总元素数量
        }
        data = new T[size];
        std::memset(data, 0, size * sizeof(T));
    }
    
    ~Tensor() {
        delete[] data;
    }

    // 为了保持简单，禁止复制和移动
    Tensor(Tensor const &) = delete;
    Tensor(Tensor &&) noexcept = delete;

    T &operator[](unsigned int const indices[N]) {
        return data[data_index(indices)];
    }
    
    T const &operator[](unsigned int const indices[N]) const {
        return data[data_index(indices)];
    }

    // 辅助函数：获取总元素数量
    unsigned int total_size() const {
        unsigned int size = 1;
        for (unsigned int i = 0; i < N; ++i) {
            size *= shape[i];
        }
        return size;
    }

    // 辅助函数：打印张量信息
    void print_info() const {
        std::cout << "Tensor<" << N << ", " << typeid(T).name() << ">" << std::endl;
        std::cout << "Shape: [";
        for (unsigned int i = 0; i < N; ++i) {
            std::cout << shape[i];
            if (i < N - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        std::cout << "Total size: " << total_size() << std::endl;
    }

private:
    unsigned int data_index(unsigned int const indices[N]) const {
        unsigned int index = 0;
        unsigned int stride = 1;  // 当前维度的步长
        
        // 从最后一个维度向前计算索引
        for (int i = N - 1; i >= 0; --i) {
            // 检查索引是否越界
            ASSERT(indices[i] < shape[i], "Invalid index");
            
            // TODO: 计算 index
            // 索引计算公式：index = Σ(indices[i] * stride_i)
            // 其中 stride_i = Π(shape[j]) for j = i+1 to N-1
            index += indices[i] * stride;
            
            // 更新步长用于下一个维度
            if (i > 0) {
                stride *= shape[i];
            }
        }
        
        return index;
    }
};

// ---- 不要修改以下代码 ----
int main(int argc, char **argv) {
    std::cout << "Test 1: 4D Tensor of int" << std::endl;
    {
        unsigned int shape[]{2, 3, 4, 5};
        auto tensor = Tensor<4, int>(shape);
        tensor.print_info();
        
        // 计算预期总大小：2 * 3 * 4 * 5 = 120
        ASSERT(tensor.total_size() == 120, "Total size should be 120");

        unsigned int i0[]{0, 0, 0, 0};
        tensor[i0] = 1;
        ASSERT(tensor[i0] == 1, "tensor[i0] should be 1");
        ASSERT(tensor.data[0] == 1, "tensor.data[0] should be 1");
        
        // 验证索引 [1, 2, 3, 4]
        // 计算索引：1 * (3*4*5) + 2 * (4*5) + 3 * 5 + 4 = 1*60 + 2*20 + 15 + 4 = 60 + 40 + 15 + 4 = 119
        unsigned int i1[]{1, 2, 3, 4};
        tensor[i1] = 2;
        ASSERT(tensor[i1] == 2, "tensor[i1] should be 2");
        ASSERT(tensor.data[119] == 2, "tensor.data[119] should be 2");
        
        std::cout << "Test 1 passed!" << std::endl;
    }
    
    std::cout << "\nTest 2: 3D Tensor of float" << std::endl;
    {
        unsigned int shape[]{7, 8, 128};
        auto tensor = Tensor<3, float>(shape);
        tensor.print_info();
        
        // 计算预期总大小：7 * 8 * 128 = 7168
        ASSERT(tensor.total_size() == 7168, "Total size should be 7168");

        unsigned int i0[]{0, 0, 0};
        tensor[i0] = 1.f;
        ASSERT(tensor[i0] == 1.f, "tensor[i0] should be 1");
        ASSERT(tensor.data[0] == 1.f, "tensor.data[0] should be 1");
        
        // 验证索引 [3, 4, 99]
        // 计算索引：3 * (8*128) + 4 * 128 + 99 = 3*1024 + 4*128 + 99 = 3072 + 512 + 99 = 3683
        unsigned int i1[]{3, 4, 99};
        tensor[i1] = 2.f;
        ASSERT(tensor[i1] == 2.f, "tensor[i1] should be 2");
        ASSERT(tensor.data[3683] == 2.f, "tensor.data[3683] should be 2");
        
        std::cout << "Test 2 passed!" << std::endl;
    }
    
    // 额外测试：2D Tensor
    std::cout << "\nTest 3: 2D Tensor of double" << std::endl;
    {
        unsigned int shape[]{3, 4};
        auto tensor = Tensor<2, double>(shape);
        tensor.print_info();
        
        // 测试一些位置
        for (unsigned int i = 0; i < 3; ++i) {
            for (unsigned int j = 0; j < 4; ++j) {
                unsigned int indices[]{i, j};
                tensor[indices] = i * 10.0 + j;
            }
        }
        
        // 验证值
        unsigned int indices1[]{1, 2};
        ASSERT(tensor[indices1] == 12.0, "tensor[1][2] should be 12.0");
        
        unsigned int indices2[]{2, 3};
        ASSERT(tensor[indices2] == 23.0, "tensor[2][3] should be 23.0");
        
        std::cout << "Test 3 passed!" << std::endl;
    }
    
    // 测试边界情况：1D Tensor
    std::cout << "\nTest 4: 1D Tensor of int" << std::endl;
    {
        unsigned int shape[]{10};
        auto tensor = Tensor<1, int>(shape);
        tensor.print_info();
        
        for (unsigned int i = 0; i < 10; ++i) {
            unsigned int indices[]{i};
            tensor[indices] = i * 2;
        }
        
        unsigned int indices[]{5};
        ASSERT(tensor[indices] == 10, "tensor[5] should be 10");
        
        std::cout << "Test 4 passed!" << std::endl;
    }
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    
    return 0;
}