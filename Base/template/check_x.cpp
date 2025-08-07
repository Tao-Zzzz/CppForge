#include <type_traits>
#include <iostream>

// 1. 定义一个 Trait 用于检测 T 是否有非 void 的 `value_type`
template <typename T, typename = void>
struct has_non_void_value_type : std::false_type
{
};

// 仅当 T 有 `value_type` 且 `value_type` 不是 void 时，特化为 std::true_type
template <typename T>
struct has_non_void_value_type<T, std::enable_if_t<!std::is_void_v<typename T::value_type>>> : std::true_type
{
};

// 2. 定义 TypePrinter 主模板，使用一个布尔参数控制特化
template <typename T, bool HasValueType = has_non_void_value_type<T>::value>
struct TypePrinter;

// 3. 特化：当 HasValueType 为 true 时，表示 T 有非 void 的 `value_type`
template <typename T>
struct TypePrinter<T, true>
{
    static void print()
    {
        std::cout << "T has a member type 'value_type'." << std::endl;
    }
};

// 特化：当 HasValueType 为 false 时，表示 T 没有 `value_type` 或 `value_type` 是 void
template <typename T>
struct TypePrinter<T, false>
{
    static void print()
    {
        std::cout << "hello world! T does not have a member type 'value_type'." << std::endl;
    }
};

// 测试结构体
struct WithValueType
{
    using value_type = int;
};

struct WithoutValueType
{
};

struct WithVoidValueType
{
    using value_type = void;
};

int main()
{
    TypePrinter<WithValueType>::print();     // 输出: T has a member type 'value_type'.
    TypePrinter<WithoutValueType>::print();  // 输出: hello world! T does not have a member type 'value_type'.
    TypePrinter<WithVoidValueType>::print(); // 输出: hello world! T does not have a member type 'value_type'.
    return 0;
}