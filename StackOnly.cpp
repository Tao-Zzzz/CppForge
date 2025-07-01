#include <iostream>


class StackOnly{
public:
    StackOnly() = default;
    void doSometing()
    {
        std::cout << "oh ye, i am in the stack" << std::endl;
    }

private:
    void* operator new(size_t) = delete;
    void* operator new[](size_t) = delete;
    void operator delete(void* p) = delete;
    void operator delete[](void* p) = delete;
};

int main(){
    StackOnly obj;
    obj.doSometing();

    return 0;
}