#include <future>
#include <thread>
#include <iostream>
void myFunction(std::promise<int> &&promise)
{
    // 模拟一些工作
    std::this_thread::sleep_for(std::chrono::seconds(1));
    promise.set_value(42); // 设置 promise 的值
}

void threadFunction(std::shared_future<int> future)
{
    try
    {
        int result = future.get();
        std::cout << "Result: " << result << std::endl;
    }
    catch (const std::future_error &e)
    {
        std::cout << "Future error: " << e.what() << std::endl;
    }
}

void use_shared_future()
{
    std::promise<int> promise;
    std::shared_future<int> future = promise.get_future();

    std::thread myThread1(myFunction, std::move(promise)); // 将 promise 移动到线程中

    // 使用 share() 方法获取新的 shared_future 对象

    std::thread myThread2(threadFunction, future);

    std::thread myThread3(threadFunction, future);

    myThread1.join();
    myThread2.join();
    myThread3.join();
}

int main(){
    use_shared_future();
    return 0;
}