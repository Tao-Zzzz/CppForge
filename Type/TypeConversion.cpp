#include <iostream>
#include <string>

class Base{
public:
    virtual void print(std::string whoPrint = "") {
        std::cout << whoPrint << "Base\n";
    } 
};

class Derived: public Base{
public:
    void print(std::string whoPrint = "") override{
        Base::print("I am Drevied: I Print this: ");
        std::cout << "Derived\n";
    }
};

int main(){
    int i = 65;
    char c =static_cast<char>(i);
    std::cout << "static_cast: int to char: " << c << std::endl;

    Base* base = new Derived();
    base->print();
    Derived* derived = static_cast<Derived*>(base);
    derived->print();

    const int x = 42;
    int* x_ptr = const_cast<int*>(&x);
    *x_ptr = 100;
    std::cout << "const_cast: modifed const int: " << *x_ptr 
    << " and original is: "<< x << std::endl;

    int num = 0x41424344; // ABCD
    char* str = reinterpret_cast<char*>(&num);
    std::cout << "reinterpret_cast: int to char*: " << str[0] << std::endl;

    Base* b = new Derived();
    b->print();
    Derived* d  = dynamic_cast<Derived*>(b);
    if(d){
        d->print();
    }else{
        std::cout << "dynamic_cast failed" << std::endl;
    }

    Base* b2 = new Base();
    b2->print();
    Derived* d2 = dynamic_cast<Derived*>(b2);
    if(d2){
        d2->print();
    }else{
        std::cout << "dynamic_cast failed" << std::endl;
    }

    delete base;
    delete b2;

    return 0;
}