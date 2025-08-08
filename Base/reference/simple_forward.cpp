#include <iostream>
#include <string>
#include <utility>

// 目标函数，分别有左值和右值的重载
void process(int &x)
{
    std::cout << "process(int&)" << std::endl;
}

void process(int &&x)
{
    std::cout << "process(int&&)" << std::endl;
}

// 通用的包装函数，使用转发引用
template <typename T>
void wrapper(T &&arg)
{
    process(std::forward<T>(arg)); // 原样转发
}

int main()
{
    int a = 10;
    wrapper(a);  // 传递左值，调用process(int&)
    wrapper(20); // 传递右值，调用process(int&&)
    return 0;
}