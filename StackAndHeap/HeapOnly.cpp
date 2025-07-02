#include <memory>
#include <iostream>

template <typename T>
class MyDeleter
{
public:
    void operator()(T *p) const
    {
        delete p;
    }
};


class HeapOnly{
private:
    ~HeapOnly() = default;

    template<typename T>
    friend class MyDeleter;

    public:

    HeapOnly() = default;
    // 静态工厂
    static std::unique_ptr<HeapOnly, MyDeleter<HeapOnly>> create(){
        // new HeapOnly() 现在可以调用这个公开的默认构造函数
        return std::unique_ptr<HeapOnly, MyDeleter<HeapOnly>>(new HeapOnly());
    }

    HeapOnly(const HeapOnly&) = delete;
    HeapOnly& operator=(const HeapOnly&) = delete;

    HeapOnly(HeapOnly&&) = default;
    HeapOnly& operator=(HeapOnly&&) = default;

    void doSomething(){ 
        std::cout << "oh ye, i am in the heap!" << std::endl;
    }
};



int main(){

    auto ptr = HeapOnly::create();
    ptr->doSomething();
    return 0;
}