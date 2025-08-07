#include <type_traits>
#include <iostream>

// 适用于整数类型
template <typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_type(T value)
{
    std::cout << "Integral type: " << value << std::endl;
}

// 适用于浮点类型
template <typename T>
typename std::enable_if<std::is_floating_point<T>::value, void>::type
print_type(T value)
{
    std::cout << "Floating point type: " << value << std::endl;
}

int main()
{
    print_type(10);   // 输出: Integral type: 10
    print_type(3.14); // 输出: Floating point type: 3.14
    // print_type("Hello"); // 编译错误，没有匹配的函数
    return 0;
}