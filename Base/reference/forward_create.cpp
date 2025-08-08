#include <iostream>
#include <string>
#include <utility>

// 类的定义
class Person
{
public:
    std::string name;
    int age;

    // 左值引用构造函数
    Person(const std::string &n, int a) : name(n), age(a)
    {
        std::cout << "Constructed Person(const std::string&, int)" << std::endl;
    }

    // 右值引用构造函数
    Person(std::string &&n, int a) : name(std::move(n)), age(a)
    {
        std::cout << "Constructed Person(std::string&&, int)" << std::endl;
    }
};

// 工厂函数，使用原样转发构造函数参数
template <typename T, typename... Args>
T create(Args &&...args)
{
    return T(std::forward<Args>(args)...);
}

int main()
{
    std::string name = "Alice";

    // 传递左值
    Person p1 = create<Person>(name, 30);
    // 传递右值
    Person p2 = create<Person>(std::string("Bob"), 25);

    return 0;
}