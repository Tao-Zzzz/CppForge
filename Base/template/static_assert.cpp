#include <type_traits>

template<typename T>
struct IsPointer{
    static constexpr bool value = false;
};

template <typename T>
struct IsPointer<T*>
{
    static constexpr bool value = true;
};

int main(){
    static_assert(IsPointer<int*>::value, "int * is a pointer");
    static_assert(!IsPointer<int>::value, "int is a pointer");
}