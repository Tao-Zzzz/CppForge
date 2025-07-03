#include <iostream>

void counter(){
    static int count = 0;
    count++;
    std::cout << "Counter: " << count << std::endl;
}

static int globalVar = 10;

static void secretFunction(){
    std::cout << "Secret Function, Global: " << globalVar << std::endl;
}

class MyClass{
public:
    static int sharedCount;
    MyClass(){
        std::cout << "Default Class count: " << sharedCount << std::endl;
        sharedCount++;
    }
    
    static void printCount(){
        std::cout << "Shared Count: " << sharedCount << std::endl;
    }
};

// 必须要有默认值啊
int MyClass::sharedCount = 0;

// void func(static int x){
//     std::cout << "Static Parameter (deprecated): " << x << std::endl;
// }

int main(){
    counter();
    counter();
    counter();

    secretFunction();


    MyClass obj1, obj2;
    MyClass::printCount();

    //func(5);

    return 0;
}
