#include <type_traits>
#include <iostream>

// 使用 std::void_t 简化 has_foo
template <typename, typename = std::void_t<>>
struct has_foo : std::false_type
{
};

template <typename T>
struct has_foo<T, std::void_t<decltype(std::declval<T>().foo())>> : std::true_type
{
};

// 函数仅在 T 有 foo() 成员时启用
template <typename T>
std::enable_if_t<has_foo<T>::value, void>
call_foo(T &obj)
{
    obj.foo();
    std::cout << "foo() called." << std::endl;
}

class WithFoo
{
public:
    void foo() { std::cout << "WithFoo::foo()" << std::endl; }
};

class WithoutFoo
{
};

int main()
{
    WithFoo wf;
    call_foo(wf); // 输出: WithFoo::foo()
                  //      foo() called.

    // WithoutFoo wf2;
    // call_foo(wf2); // 编译错误，没有匹配的函数
    return 0;
}