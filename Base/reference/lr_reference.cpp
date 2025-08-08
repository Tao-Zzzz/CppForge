#include <iostream>
#include <type_traits>

void func(int &x)
{
    std::cout << "func(int&)" << std::endl;
}

void func(int &&x)
{
    std::cout << "func(int&&)" << std::endl;
}

int main()
{
    int a = 5;
    const int &ref = a;

    // 判断类型
    std::cout << std::boolalpha;
    // 使用 decltype((a)) 获取 a 的类型，包括引用
    // `(a)` 是一个左值表达式，所以 decltype((a)) 是 `int&`
    std::cout << "a 是左值: " << std::is_lvalue_reference<decltype((a))>::value << std::endl;
    std::cout << "ref 是左值引用: " << std::is_lvalue_reference<decltype(ref)>::value << std::endl;

    // 调用函数
    func(a);  // 调用func(int&)
    func(10); // 调用func(int&&)

    return 0;
}