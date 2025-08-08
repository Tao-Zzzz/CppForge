#include <iostream>
#include <string>

// 类的定义
class BigObject
{
public:
    std::string data;

    BigObject(const std::string &d) : data(d)
    {
        std::cout << "BigObject constructed with lvalue" << std::endl;
    }

    BigObject(std::string &&d) : data(std::move(d))
    {
        std::cout << "BigObject constructed with rvalue" << std::endl;
    }
};

// 处理函数，左值和右值的重载
void process(const BigObject &obj)
{
    std::cout << "Processing lvalue BigObject" << std::endl;
}

void process(BigObject &&obj)
{
    std::cout << "Processing rvalue BigObject" << std::endl;
}

// 包装函数，不使用原样转发
template <typename T>
void bad_wrapper(T &&obj)
{
    process(obj); // 始终以左值形式传递
}

// 包装函数，使用原样转发
template <typename T>
void good_wrapper(T &&obj)
{
    process(std::forward<T>(obj)); // 原样转发
}

int main()
{
    std::string s = "Sample data";

    // 使用bad_wrapper
    BigObject bo1 = BigObject(s);            // 使用lvalue构造
    BigObject bo2 = BigObject(std::move(s)); // 使用rvalue构造

    bad_wrapper(bo1);               // 始终作为左值处理
    bad_wrapper(BigObject("Temp")); // 作为左值处理，浪费移动语义

    // 使用good_wrapper
    good_wrapper(bo1);               // 作为左值处理
    good_wrapper(BigObject("Temp")); // 作为右值处理，利用移动语义

    return 0;
}