#include <iostream>
#include <type_traits>
#include <vector>
#include <string>

// 辅助类型，用于 SFINAE
template <typename T>
class has_size
{
private:
    template <typename U>
    static auto test(int) -> decltype(std::declval<U>().size(), std::true_type());

    template <typename>
    static std::false_type test(...);

public:
    static constexpr bool value = decltype(test<T>(0))::value;
};

// 函数模板，根据是否具有 size() 成员进行重载
template <typename T>
typename std::enable_if<has_size<T>::value, void>::type
printSize(const T &container)
{
    std::cout << "Size: " << container.size() << std::endl;
}

template <typename T>
typename std::enable_if<!has_size<T>::value, void>::type
printSize(const T &value)
{
    std::cout << "No size available." << std::endl;
}

int main()
{
    std::vector<int> vec = {1, 2, 3};
    std::string str = "Hello";
    int num = 42;

    printSize(vec); // 输出：Size: 3
    printSize(str); // 输出：Size: 5
    printSize(num); // 输出：No size available.

    return 0;
}