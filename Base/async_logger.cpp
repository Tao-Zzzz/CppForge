#include <queue>
#include <string>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <fstream>
#include <atomic>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <iostream>

// 辅助函数，将单个参数转换为字符串
template <typename T>
std::string to_string_helper(T &&arg)
{
    std::ostringstream oss;
    oss << std::forward<T>(arg);
    return oss.str();
}

// 线程安全的日志队列
class LogQueue
{
public:
    void push(const std::string &msg)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queue_.push(msg);
        cond_var_.notify_one();
    }

    bool pop(std::string &msg)
    {
        std::unique_lock<std::mutex> lock(mutex_);
        
        cond_var_.wait(lock, [this](){
            return !queue_.empty() || is_shutdown_;
        });

        if(queue_.empty() || is_shutdown_){
            return false;
        }

        msg = std::move(queue_.front());
        queue_.pop();
        return true;
    }

    void shutdown()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        is_shutdown_ = true;
        cond_var_.notify_all();
    }

private:
    std::queue<std::string> queue_;
    std::mutex mutex_;
    std::condition_variable cond_var_;
    bool is_shutdown_ = false;
};

// Logger 类
class Logger
{
public:
    Logger(const std::string &filename) : log_file_(filename, std::ios::out | std::ios::app), exit_flag_(false)
    {
        if (!log_file_.is_open())
        {
            throw std::runtime_error("无法打开日志文件");
        }
        worker_thread_ = std::thread(&Logger::processQueue, this);
    }

    ~Logger()
    {
        log_queue_.shutdown();
        if (worker_thread_.joinable())
        {
            worker_thread_.join();
        }
        if (log_file_.is_open())
        {
            log_file_.close();
        }
    }

    // 日志接口：支持带格式字符串的日志
    template <typename... Args>
    void log(const std::string &format, Args &&...args)
    {
        std::string level_str;
        switch (level)
        {
        case LogLevel::INFO:
            level_str = "[INFO] ";
            break;
        case LogLevel::DEBUG:
            level_str = "[DEBUG] ";
            break;
        case LogLevel::ERROR:
            level_str = "[ERROR] ";
            break;
        }
        log_queue_.push(formatMessage(format, std::forward<Args>(args)...));
    }

private:
    LogQueue log_queue_;
    std::thread worker_thread_;
    std::ofstream log_file_;
    std::atomic<bool> exit_flag_;

    void processQueue()
    {
        std::string msg;
        while (log_queue_.pop(msg))
        {
            log_file_ << msg << std::endl;
        }
    }

    // 使用模板折叠格式化日志消息，支持 "{}" 占位符
    template <typename... Args>
    std::string formatMessage(const std::string &format, Args &&...args)
    {
        std::vector<std::string> arg_strings = {to_string_helper(std::forward<Args>(args))...};
        std::ostringstream oss;
        size_t arg_index = 0;
        size_t pos = 0;
        size_t placeholder = format.find("{}", pos);

        while (placeholder != std::string::npos)
        {
            oss << format.substr(pos, placeholder - pos);
            if (arg_index < arg_strings.size())
            {
                oss << arg_strings[arg_index++];
            }
            else
            {
                // 没有足够的参数，保留 "{}"
                oss << "{}";
            }
            pos = placeholder + 2; // 跳过 "{}"
            placeholder = format.find("{}", pos);
        }

        // 添加剩余的字符串
        oss << format.substr(pos);

        // 如果还有剩余的参数，按原方式拼接
        while (arg_index < arg_strings.size())
        {
            oss << arg_strings[arg_index++];
        }

        return "[" + getCurrentTime() + "] " + (oss.str());
    }
};

std::string getCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&now_time));
    return std::string(buffer);
}

// 使用示例
int main()
{
    try
    {
        Logger logger("log.txt");

        logger.log("Starting application.");

        int user_id = 42;
        std::string action = "login";
        double duration = 3.5;
        std::string world = "World";

        logger.log("User {} performed {} in {} seconds.", user_id, action, duration);
        logger.log("Hello {}", world);
        logger.log("This is a message without placeholders.");
        logger.log("Multiple placeholders: {}, {}, {}.", 1, 2, 3);

        // 模拟一些延迟以确保后台线程处理完日志
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    catch (const std::exception &ex)
    {
        std::cerr << "日志系统初始化失败: " << ex.what() << std::endl;
    }

    return 0;
}