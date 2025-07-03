#include <iostream>

class MyClass{
public:
    int value;

    MyClass(int value){
        this->value = value;
        std::cout << "构造函数: value = " << this->value << std::endl;
    }

    MyClass& setValue(int value){
        this->value = value;
        return *this;
    }

    void printAddress(){
        std::cout << "当前对象地址: " << this << std::endl;
    }

    bool isSameObject(const MyClass& other){
        return this == &other;
    }

    void callSelf(){
        this->printValue();
    }

private:
    void printValue(){
        std::cout << "当前对象值: " << this->value << std::endl;
    }
};

int main(){
    MyClass obj1(10);

    obj1.setValue(20).setValue(30);
    obj1.callSelf();

    obj1.printAddress();

    MyClass obj2(40);
    std::cout << "是否是同一对象: " << obj1.isSameObject(obj2) << std::endl;

    obj1.callSelf();
    return 0;
}