#include "../exercise.h"
#include <iostream>

// READ: 静态字段 <https://zh.cppreference.com/w/cpp/language/static>
// READ: 虚析构函数 <https://zh.cppreference.com/w/cpp/language/destructor>

struct A {
    // TODO: 正确初始化静态字段
    // 静态成员变量需要在类外定义和初始化
    static int num_a;

    A() {
        ++num_a;
    }
    // 基类析构函数必须是虚函数，确保正确删除派生类对象
    virtual ~A() {
        --num_a;
    }

    virtual char name() const {
        return 'A';
    }
};

// 在类外定义并初始化静态成员变量
int A::num_a = 0;

struct B final : public A {
    // TODO: 正确初始化静态字段
    static int num_b;

    B() {
        ++num_b;
    }
    ~B() {
        --num_b;
    }

    char name() const final {
        return 'B';
    }
};

// 在类外定义并初始化静态成员变量
int B::num_b = 0;

int main(int argc, char **argv) {
    auto a = new A;
    auto b = new B;
    ASSERT(A::num_a == 2, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(a->name() == 'A', "Fill in the correct value for a->name()");
    ASSERT(b->name() == 'B', "Fill in the correct value for b->name()");

    delete a;
    delete b;
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    A *ab = new B; // 派生类指针可以随意转换为基类指针
    ASSERT(A::num_a == 1, "Fill in the correct value for A::num_a");
    ASSERT(B::num_b == 1, "Fill in the correct value for B::num_b");
    ASSERT(ab->name() == 'B', "Fill in the correct value for ab->name()");

    // TODO: 基类指针无法随意转换为派生类指针，补全正确的转换语句
    // 使用 dynamic_cast 进行安全的向下转型
    B *bb_ptr = dynamic_cast<B*>(ab);
    ASSERT(bb_ptr != nullptr, "ab should point to a B object");
    B &bb = *bb_ptr;
    ASSERT(bb.name() == 'B', "Fill in the correct value for bb->name()");

    // TODO: ---- 以下代码不要修改，通过改正类定义解决编译问题 ----
    delete ab; // 通过指针可以删除指向的对象，即使是多态对象
    // 由于 A 的析构函数现在是虚函数，会正确调用 B 的析构函数
    ASSERT(A::num_a == 0, "Every A was destroyed");
    ASSERT(B::num_b == 0, "Every B was destroyed");

    // 额外测试：验证动态类型转换
    std::cout << "All tests passed!" << std::endl;
    
    // 测试动态转换失败的情况
    A *aa = new A;
    B *ba = dynamic_cast<B*>(aa);
    ASSERT(ba == nullptr, "Should not be able to cast A* to B* when it's not a B object");
    delete aa;
    
    return 0;
}