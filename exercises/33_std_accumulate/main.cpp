#include "../exercise.h"
#include <numeric>

// READ: `std::accumulate` <https://zh.cppreference.com/w/cpp/algorithm/accumulate>

int main(int argc, char **argv) {
    using DataType = float;
    int shape[]{1, 3, 224, 224};
    
    // 计算总元素数量：1 * 3 * 224 * 224
    int total_elements = std::accumulate(
        shape,                       // 起始位置
        shape + 4,                   // 结束位置（shape有4个元素）
        1,                           // 初始值（乘法从1开始）
        std::multiplies<int>()       // 乘法操作
    );
    
    // 计算总字节数：总元素数量 × 每个float的大小
    int size = total_elements * sizeof(DataType);
    
    // 验证结果：1 * 3 * 224 * 224 * 4 = 602112
    ASSERT(size == 602112, "4x1x3x224x224 = 602112");
    
    return 0;
}