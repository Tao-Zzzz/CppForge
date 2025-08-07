#include <iostream>
#include <vector>
#include <type_traits>

template<typename T, typename = void>
struct has_size : std::false_type { };

template<typename T>
struct has_size<T, std::void_t<decltype(std::declval<T>().size())>> : std::true_type{};

template<typename T>
typename std::enable_if<has_size<T>::value, void>::type
enableIfExample(const T& container){
    std::cout << "Container has size" << container.size() << std::endl;
}

int main(){
    std::vector<int> vec = {1, 2, 3};
    enableIfExample(vec);

    return 0;
}