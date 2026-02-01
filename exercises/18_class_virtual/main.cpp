#include "../exercise.h"
#include <iostream>

// READ: 虚函数 <https://zh.cppreference.com/w/cpp/language/virtual>

struct A {
    virtual char virtual_name() const {
        return 'A';
    }
    char direct_name() const {
        return 'A';
    }
};
struct B : public A {
    // READ: override <https://zh.cppreference.com/w/cpp/language/override>
    char virtual_name() const override {
        return 'B';
    }
    char direct_name() const {
        return 'B';
    }
};
struct C : public B {
    // READ: final <https://zh.cppreference.com/w/cpp/language/final>
    char virtual_name() const final {
        return 'C';
    }
    char direct_name() const {
        return 'C';
    }
};
struct D : public C {
    // D 不能重写 virtual_name()，因为 C 中的 virtual_name() 是 final 的
    // char virtual_name() const override {}  // 编译错误
    
    char direct_name() const {
        return 'D';
    }
};

int main(int argc, char **argv) {
    constexpr auto MSG = "Replace '?' with its correct name.";

    A a;
    B b;
    C c;
    D d;

    // 直接调用：根据对象的实际类型调用对应的方法
    ASSERT(a.virtual_name() == 'A', MSG);    // A 的 virtual_name()
    ASSERT(b.virtual_name() == 'B', MSG);    // B 的 virtual_name()
    ASSERT(c.virtual_name() == 'C', MSG);    // C 的 virtual_name()
    ASSERT(d.virtual_name() == 'C', MSG);    // D 继承 C 的 virtual_name()，不能重写
    
    ASSERT(a.direct_name() == 'A', MSG);     // A 的 direct_name()
    ASSERT(b.direct_name() == 'B', MSG);     // B 的 direct_name()
    ASSERT(c.direct_name() == 'C', MSG);     // C 的 direct_name()
    ASSERT(d.direct_name() == 'D', MSG);     // D 的 direct_name()

    // 通过基类引用调用
    A &rab = b;  // rab 引用 B 对象，但类型是 A&
    B &rbc = c;  // rbc 引用 C 对象，但类型是 B&
    C &rcd = d;  // rcd 引用 D 对象，但类型是 C&

    // 虚函数：动态绑定，根据对象的实际类型调用
    ASSERT(rab.virtual_name() == 'B', MSG);  // 实际对象是 B，调用 B::virtual_name()
    ASSERT(rbc.virtual_name() == 'C', MSG);  // 实际对象是 C，调用 C::virtual_name()
    ASSERT(rcd.virtual_name() == 'C', MSG);  // 实际对象是 D，但继承 C::virtual_name()
    
    // 非虚函数：静态绑定，根据引用的类型调用
    ASSERT(rab.direct_name() == 'A', MSG);   // 引用类型是 A&，调用 A::direct_name()
    ASSERT(rbc.direct_name() == 'B', MSG);   // 引用类型是 B&，调用 B::direct_name()
    ASSERT(rcd.direct_name() == 'C', MSG);   // 引用类型是 C&，调用 C::direct_name()

    // 多级继承引用
    A &rac = c;  // rac 引用 C 对象，但类型是 A&
    B &rbd = d;  // rbd 引用 D 对象，但类型是 B&

    ASSERT(rac.virtual_name() == 'C', MSG);  // 实际对象是 C，调用 C::virtual_name()
    ASSERT(rbd.virtual_name() == 'C', MSG);  // 实际对象是 D，但继承 C::virtual_name()
    
    ASSERT(rac.direct_name() == 'A', MSG);   // 引用类型是 A&，调用 A::direct_name()
    ASSERT(rbd.direct_name() == 'B', MSG);   // 引用类型是 B&，调用 B::direct_name()

    // 跨越多级继承
    A &rad = d;  // rad 引用 D 对象，但类型是 A&

    ASSERT(rad.virtual_name() == 'C', MSG);  // 实际对象是 D，但继承 C::virtual_name()
    ASSERT(rad.direct_name() == 'A', MSG);   // 引用类型是 A&，调用 A::direct_name()

    // 打印结果验证
    std::cout << "All tests passed!" << std::endl;
    std::cout << "Virtual function behavior:" << std::endl;
    std::cout << "  a.virtual_name() = " << a.virtual_name() << std::endl;
    std::cout << "  b.virtual_name() = " << b.virtual_name() << std::endl;
    std::cout << "  c.virtual_name() = " << c.virtual_name() << std::endl;
    std::cout << "  d.virtual_name() = " << d.virtual_name() << std::endl;
    std::cout << "\nDirect (non-virtual) function behavior:" << std::endl;
    std::cout << "  a.direct_name() = " << a.direct_name() << std::endl;
    std::cout << "  b.direct_name() = " << b.direct_name() << std::endl;
    std::cout << "  c.direct_name() = " << c.direct_name() << std::endl;
    std::cout << "  d.direct_name() = " << d.direct_name() << std::endl;

    return 0;
}

// READ: 扩展阅读-纯虚、抽象 <https://zh.cppreference.com/w/cpp/language/abstract_class>
// READ: 扩展阅读-虚继承 <https://zh.cppreference.com/w/cpp/language/derived_class>