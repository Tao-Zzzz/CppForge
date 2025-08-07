#include <cstddef>

// 定义 TypeList 模板，用于表示类型列表
template <typename... Ts>
struct TypeList
{
};

// 定义 LengthOf 模板，用于计算类型列表长度
template <typename List>
struct LengthOf;

// 特化 LengthOf 模板，计算 TypeList<Ts...> 的长度
template <typename... Ts>
struct LengthOf<TypeList<Ts...>>
{
    static constexpr std::size_t value = sizeof...(Ts);
};

// 测试用例
int main()
{
    // 测试空类型列表
    static_assert(LengthOf<TypeList<>>::value == 0, "Length of empty TypeList should be 0");

    // 测试单个类型的类型列表
    static_assert(LengthOf<TypeList<int>>::value == 1, "Length of TypeList<int> should be 1");

    // 测试多个类型的类型列表
    static_assert(LengthOf<TypeList<int, double, char>>::value == 3, "Length of TypeList<int, double, char> should be 3");

    // 测试复杂类型的类型列表
    static_assert(LengthOf<TypeList<float, bool, long, void>>::value == 4, "Length of TypeList<float, bool, long, void> should be 4");

    return 0;
}