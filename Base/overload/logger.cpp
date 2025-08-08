#include <iostream>
#include <string>

class Logger
{
private:
    std::string message;

public:
    // 构造函数
    Logger(const std::string &msg = "") : message(msg) {}

    // 重载逗号运算符（成员函数）
    Logger operator,(const Logger &other) const
    {
        // 简单示例：连接日志消息
        return Logger(this->message + ", " + other.message);
    }

    // 重载 << 运算符用于输出
    friend std::ostream &operator<<(std::ostream &os, const Logger &l);
};

// 实现 << 运算符
std::ostream &operator<<(std::ostream &os, const Logger &l)
{
    os << l.message;
    return os;
}

// 示例
int main()
{
    Logger log1("启动");
    Logger log2("加载配置");
    Logger log3("初始化");
    Logger combined = (log1, log2, log3);
    std::cout << "组合日志: " << combined << std::endl;
    return 0;
}