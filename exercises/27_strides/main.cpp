#include "../exercise.h"
#include <vector>
#include <iostream>

// READ: std::vector <https://zh.cppreference.com/w/cpp/container/vector_bool>
// READ: 模板特化 <https://zh.cppreference.com/w/cpp/language/template_specialization>

// TODO: 将下列 `?` 替换为正确的代码
int main(int argc, char **argv) {
    // 创建一个包含 100 个 true 的 vector<bool>
    std::vector<bool> vec(100, true);  // TODO: 正确调用构造函数
    
    // 验证所有元素都是 true
    ASSERT(vec[0], "Make this assertion pass.");
    ASSERT(vec[99], "Make this assertion pass.");
    ASSERT(vec.size() == 100, "Make this assertion pass.");
    
    // NOTICE: 平台相关！注意 CI:Ubuntu 上的值。
    // std::vector<bool> 是 std::vector 对 bool 类型的模板特化
    // 它使用位压缩来节省空间
    std::cout << "sizeof(std::vector<bool>) = " << sizeof(std::vector<bool>) << std::endl;
    
    // vector<bool> 对象本身的大小通常是 3 个指针（与普通 vector 相同）
    // 但注意：实现可能不同，这里使用更通用的断言
    ASSERT(sizeof(vec) == sizeof(std::vector<bool>), "Fill in the correct value.");
    
    {
        // 将第21个元素（索引20）设置为 false
        vec[20] = false;
        ASSERT(!vec[20], "Fill in `vec[20]` or `!vec[20]`.");  // 现在应该是 false
    }
    
    {
        // 在末尾添加一个 false 元素
        vec.push_back(false);
        ASSERT(vec.size() == 101, "Fill in the correct value.");  // 100 + 1 = 101
        ASSERT(!vec[100], "Fill in `vec[100]` or `!vec[100]`.");  // 新添加的元素应该是 false
    }
    
    {
        // 获取第31个元素（索引30）的引用
        auto ref = vec[30];
        
        // ref 应该是一个代理引用对象，不是直接的 bool&
        // 在创建时，vec[30] 是 true（因为我们初始化了所有元素为 true）
        ASSERT(ref, "Fill in `ref` or `!ref`");  // 应该是 true
        
        // 通过引用修改值
        ref = false;
        ASSERT(!ref, "Fill in `ref` or `!ref`");  // 现在应该是 false
        
        // THINK: WHAT and WHY?
        // ref 是一个代理对象，修改它会修改底层 vector 中的值
        ASSERT(!vec[30], "Fill in `vec[30]` or `!vec[30]`.");  // vec[30] 也应该变为 false
    }
    
    // 额外测试：展示 vector<bool> 的特性
    std::cout << "\n=== Additional tests for std::vector<bool> ===" << std::endl;
    
    // 1. 测试位压缩
    std::vector<bool> bits(64);
    std::vector<char> chars(64);
    std::cout << "Memory usage comparison:" << std::endl;
    std::cout << "  vector<bool>(64) object size: " << sizeof(bits) << " bytes" << std::endl;
    std::cout << "  vector<char>(64) object size: " << sizeof(chars) << " bytes" << std::endl;
    
    // 2. 测试 flip() 方法（vector<bool> 特有的方法）
    bits.flip();  // 翻转所有位
    std::cout << "After flip(), bits[0] = " << bits[0] << std::endl;
    
    // 3. 测试引用类型
    std::vector<bool>::reference bit_ref = bits[10];
    bit_ref = true;
    std::cout << "bits[10] = " << bits[10] << std::endl;
    
    // 4. 证明引用不是普通的 bool&
    bool regular_bool = true;
    bool& regular_ref = regular_bool;
    
    auto bit_ref2 = bits[20];
    
    std::cout << "\nType information:" << std::endl;
    std::cout << "  Type of regular_ref: " << typeid(regular_ref).name() << std::endl;
    std::cout << "  Type of bit_ref2: " << typeid(bit_ref2).name() << std::endl;
    std::cout << "  Are they the same type? " 
              << (typeid(regular_ref) == typeid(bit_ref2) ? "Yes" : "No") << std::endl;
    
    // 5. 展示迭代器行为
    std::cout << "\nIterating over vector<bool>:" << std::endl;
    std::vector<bool> small_vec = {true, false, true, true, false};
    std::cout << "Contents: ";
    for (auto it = small_vec.begin(); it != small_vec.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;
    
    // 6. 展示重新分配的容量增长
    std::cout << "\nCapacity growth (typical implementation stores bits in bytes):" << std::endl;
    std::vector<bool> capacity_test;
    for (int i = 0; i < 10; i++) {
        capacity_test.push_back(true);
        std::cout << "Size: " << capacity_test.size() 
                  << ", Capacity: " << capacity_test.capacity()
                  << " (approx " << (capacity_test.capacity() + 7) / 8 << " bytes)" << std::endl;
    }
    
    std::cout << "\nAll tests completed successfully!" << std::endl;
    
    return 0;
}