#include <iostream>
#include <string>
#include <utility>

// 目标函数，分别有左值和右值的重载
void process(const std::string& s) {
    std::cout << "Processing lvalue: " << s << std::endl;
}

void process(std::string&& s) {
    std::cout << "Processing rvalue: " << s << std::endl;
}

// 通用的包装函数，实现原样转发
template<typename T>
void wrapper(T&& arg) {
    process(std::forward<T>(arg));
}

int main() {
    std::string name = "Alice";
    wrapper(name);                     // 传递左值，调用process(const std::string&)
    wrapper("Bob");                    // 传递右值，调用process(std::string&&)
    wrapper(std::move(name));          // 传递右值，调用process(std::string&&)

    return 0;
}