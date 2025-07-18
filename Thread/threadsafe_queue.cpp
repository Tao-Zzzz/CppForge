
#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

template <typename T>
class threadsafe_queue
{
private:
    mutable std::mutex mut;
    std::queue<T> data_queue;
    std::condition_variable data_cond;

public:
    threadsafe_queue()
    {
    }
    // 用的是其他队列的互斥量
    threadsafe_queue(threadsafe_queue const &other)
    {
        std::lock_guard<std::mutex> lk(other.mut);
        data_queue = other.data_queue;
    }
    // push后唤醒一个线程
    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(mut);
        data_queue.push(new_value);
        data_cond.notify_one();
    }
    void wait_and_pop(T &value)
    {
        std::unique_lock<std::mutex> lk(mut);
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        value = data_queue.front();
        data_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(mut);
        // 如果是空, 则等待
        data_cond.wait(lk, [this]
                       { return !data_queue.empty(); });
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool try_pop(T &value)
    {
        std::lock_guard<std::mutex> lk(mut);
        if (data_queue.empty())
            return false;
        value = data_queue.front();
        data_queue.pop();
        return true;
    }
    std::shared_ptr<T> try_pop()
    {
        std::lock_guard<std::mutex> lk(mut);
        // 空则返回空, 不进行等待, 但也不因此出现线程不安全的情况
        // 因为只有一个互斥量,所以只会有一个实际线程在运行
        if (data_queue.empty())
            return std::shared_ptr<T>();
        std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
        data_queue.pop();
        return res;
    }
    bool empty() const
    {
        std::lock_guard<std::mutex> lk(mut);
        return data_queue.empty();
    }
};

void test_safe_que()
{
    threadsafe_queue<int> safe_que;
    std::mutex mtx_print;
    std::thread producer(
        [&]()
        {
            for (int i = 0;; i++)
            {
                safe_que.push(i);
                {
                    std::lock_guard<std::mutex> printlk(mtx_print);
                    std::cout << "producer push data is " << i << std::endl;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(200));
            }
        });

    std::thread consumer1(
        [&]()
        {
            for (;;)
            {
                auto data = safe_que.wait_and_pop();
                {
                    std::lock_guard<std::mutex> printlk(mtx_print);
                    std::cout << "consumer1 wait and pop data is " << *data << std::endl;
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });

    std::thread consumer2(
        [&]()
        {
            for (;;)
            {
                auto data = safe_que.try_pop();
                if (data != nullptr)
                {
                    {
                        std::lock_guard<std::mutex> printlk(mtx_print);
                        std::cout << "consumer2 try_pop data is " << *data << std::endl;
                    }
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(500));
            }
        });

    producer.join();
    consumer1.join();
    consumer2.join();
}

int main()
{

    test_safe_que();
    return 1;
}