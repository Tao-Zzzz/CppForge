#include <type_traits>

// 定义 TypeIdentity 模板类
template <typename T>
struct TypeIdentity
{
    using type = T;
};

// 测试用例
int main()
{
    // 验证 TypeIdentity<int>::type 是 int
    static_assert(std::is_same<TypeIdentity<int>::type, int>::value, "TypeIdentity<int> should be int");

    // 验证 TypeIdentity<double>::type 是 double
    static_assert(std::is_same<TypeIdentity<double>::type, double>::value, "TypeIdentity<double> should be double");

    // 验证 TypeIdentity<char>::type 是 char
    static_assert(std::is_same<TypeIdentity<char>::type, char>::value, "TypeIdentity<char> should be char");

    return 0;
}