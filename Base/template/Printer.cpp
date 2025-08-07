#include <iostream>

template<typename T>
class Printer{
public:
    Printer(T value) : value_(value) {}

    void print() const{
        std::cout << "General Printer: " << value_ << std::endl;
    }
private:
    T value_;
};

template <>
class Printer<bool>
{
public:
    Printer(bool value) : value_(value) {}

    void print() const
    {
        std::cout << "Boolean Printer: " << (value_ ? "true" : "false") << std::endl;
    }

private:
    bool value_;
};