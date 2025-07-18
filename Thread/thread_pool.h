#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include <functional>

class ThreadPool
{
public:
    ThreadPool(const ThreadPool &) = delete;
    ThreadPool &operator=(const ThreadPool &) = delete;

    // 局部变量单例
    static ThreadPool &instance()
    {
        static ThreadPool ins;
        return ins;
    }

    // void()的函数
    using Task = std::packaged_task<void()>;

    ~ThreadPool()
    {
        stop();
    }

    // 返回的是一个future, 类型要根据推导
    template <class F, class... Args>
    auto commit(F &&f, Args &&...args) -> std::future<decltype(f(args...))>
    {
        using RetType = decltype(f(args...));
        if (stop_.load())
            return std::future<RetType>{};

        // 提前绑定好参数, 变成一个不接收任何参数的std::function<void()>, 只需要执行即可
        auto task = std::make_shared<std::packaged_task<RetType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<RetType> ret = task->get_future();
        {
            std::lock_guard<std::mutex> cv_mt(cv_mt_);
            tasks_.emplace([task]
                           {
                            //(*task)(); 的意思是：执行 task 中封装的函数体。 
                            (*task)(); });
        }
        cv_lock_.notify_one();
        return ret;
    }

    int idleThreadCount()
    {
        return thread_num_;
    }

private:
    ThreadPool(unsigned int num = 5)
        : stop_(false)
    {
        // 规范化
        {
            if (num < 1)
                thread_num_ = 1;
            else
                thread_num_ = num;
        }
        start();
    }

    void start()
    {
        for (int i = 0; i < thread_num_; ++i)
        {
            pool_.emplace_back([this]()
                               {
                while (!this->stop_.load()) {
                    Task task;
                    {
                        std::unique_lock<std::mutex> cv_mt(cv_mt_);
                        // 任务队列不为空
                        this->cv_lock_.wait(cv_mt, [this] {
                            return this->stop_.load() || !this->tasks_.empty();
                        });
                        if (this->tasks_.empty())
                            return;

                        // 取走任务
                        task = std::move(this->tasks_.front());
                        this->tasks_.pop();
                    }
                    this->thread_num_--;
                    task();
                    this->thread_num_++;
                } });
        }
    }
    
    void stop()
    {
        stop_.store(true);
        cv_lock_.notify_all();
        for (auto &td : pool_)
        {
            if (td.joinable())
            {
                std::cout << "join thread " << td.get_id() << std::endl;
                td.join();
            }
        }
    }

private:
    std::mutex cv_mt_;
    std::condition_variable cv_lock_;
    std::atomic_bool stop_;
    std::atomic_int thread_num_;
    std::queue<Task> tasks_;
    std::vector<std::thread> pool_;
};

#endif // !__THREAD_POOL_H__