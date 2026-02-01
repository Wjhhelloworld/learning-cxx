#include "../exercise.h"
#include <iostream>

// READ: 派生类 <https://zh.cppreference.com/w/cpp/language/derived_class>

static int i = 0;

struct X {
    int x;

    X(int x_) : x(x_) {
        std::cout << ++i << ". " << "X(" << x << ')' << std::endl;
    }
    X(X const &other) : x(other.x) {
        std::cout << ++i << ". " << "X(X const &) : x(" << x << ')' << std::endl;
    }
    ~X() {
        std::cout << ++i << ". " << "~X(" << x << ')' << std::endl;
    }
};
struct A {
    int a;

    A(int a_) : a(a_) {
        std::cout << ++i << ". " << "A(" << a << ')' << std::endl;
    }
    A(A const &other) : a(other.a) {
        std::cout << ++i << ". " << "A(A const &) : a(" << a << ')' << std::endl;
    }
    ~A() {
        std::cout << ++i << ". " << "~A(" << a << ')' << std::endl;
    }
};
struct B : public A {
    X x;

    B(int b) : A(1), x(b) {
        std::cout << ++i << ". " << "B(" << a << ", X(" << x.x << "))" << std::endl;
    }
    B(B const &other) : A(other.a), x(other.x) {
        std::cout << ++i << ". " << "B(B const &) : A(" << a << "), x(X(" << x.x << "))" << std::endl;
    }
    ~B() {
        std::cout << ++i << ". " << "~B(" << a << ", X(" << x.x << "))" << std::endl;
    }
};

int main(int argc, char **argv) {
    X x = X(1);
    A a = A(2);
    B b = B(3);

    // TODO: 补全三个类型的大小
    // X 只有一个 int 成员，大小应该是 sizeof(int)，通常是 4
    // A 也只有一个 int 成员，大小同样是 sizeof(int)
    // B 继承自 A 并包含一个 X 成员，所以大小是 sizeof(A) + sizeof(X)
    // 由于内存对齐，实际大小可能会有所不同
    static_assert(sizeof(X) == sizeof(int), "There is an int in X");
    static_assert(sizeof(A) == sizeof(int), "There is an int in A");
    static_assert(sizeof(B) == sizeof(A) + sizeof(X), "B is an A with an X");
    
    std::cout << "sizeof(X) = " << sizeof(X) << std::endl;
    std::cout << "sizeof(A) = " << sizeof(A) << std::endl;
    std::cout << "sizeof(B) = " << sizeof(B) << std::endl;

    i = 0;
    std::cout << std::endl
              << "-------------------------" << std::endl
              << std::endl;

    // 这是不可能的，A 无法提供 B 增加的成员变量的值
    // B ba = A(4);  // 编译错误：不能将 A 转换为 B

    // 这也是不可能的，因为 A 是 B 的一部分，就好像不可以把套娃的外层放进内层里。
    A ab = B(5); // 然而这个代码可以编译和运行！
    // 这里发生了对象切片（object slicing）
    // 一个 B 对象被赋值给 A 对象，B 中新增的成员被"切掉"了
    // THINK: 观察打印出的信息，推测把大象放进冰箱分几步？
    // 步骤：
    // 1. 创建临时 B 对象（调用 B 的构造函数）
    // 2. 从 B 对象复制构造 A 对象（调用 A 的拷贝构造函数）
    // 3. 销毁临时 B 对象
    // THINK: 这样的代码是"安全"的吗？
    // 不安全！这会导致对象切片问题。B 对象特有的数据和行为会丢失。
    // NOTICE: 真实场景中不太可能出现这样的代码

    i = 0;
    std::cout << std::endl
              << "-------------------------" << std::endl
              << std::endl;

    return 0;
}