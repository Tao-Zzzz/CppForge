
#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>

class Logger
{
public:
    // 获取单例实例
    static Logger &getInstance();

    // 禁止拷贝和赋值
    Logger(const Logger &) = delete;
    Logger &operator=(const Logger &) = delete;

    // 记录日志
    void log(const std::string &message);

private:
    Logger(); // 私有构造函数
    ~Logger();

    std::ofstream logFile;
    std::mutex mtx;
};

#endif // LOGGER_H
