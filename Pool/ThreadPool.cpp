#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>

class ThreadPool{
public:
    explicit ThreadPool(size_t thread_count) : stop(false){
        for(size_t i = 0; i < thread_count; ++i){
            workers.emplace_back([this](){
                while(true){
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        
                        cv.wait(lock, [this](){
                            return stop || !tasks.empty();
                        });

                        if(stop && tasks.empty()){
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                    }

                    task();
                }
            });
        }
    }

    // 用move来获得任务..
    void enqueue(std::function<void()> task){
        {
            std::unique_lock<std::mutex> lock(mtx);
            tasks.push(std::move(task));
        }
    }

    template<typename F,typename... Args>
    auto submit(F&& f, Args&&... args) 
        -> std::future<typename std::invoke_result_t<F, Args...>>{
            using return_type = typename std::invoke_result_t<F, Args...>;

            auto task_ptr = std::make_shared<std::packaged_task<return_type()>>(
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
            );

            {
                std::unique_lock<std::mutex> lock(mtx);
                tasks.emplace([task_ptr](){
                    (*task_ptr)();
                });
            }

            cv.notify_one();
            return task_ptr->get_future();
        }

    ~ThreadPool(){
        {
            std::unique_lock<std::mutex> lock(mtx);
            stop = true;
        }

        cv.notify_all();
        // 线程销毁..
        for(auto& t : workers){
            t.join();
        }
    }


private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex mtx;
    std::condition_variable cv;
    bool stop;
};

int main(){
    ThreadPool pool(4);
    
    for(int i = 0; i < 2; ++i){
        pool.enqueue([i](){
            std::cout << "Task " << i << " is running in thread" << std::this_thread::get_id() << std::endl;

        });
    }

    auto future1 = pool.submit([](int a, int b){
        return a + b;
    }, 3, 4);

    auto future2 = pool.submit([](){
        std::cout << "Hello from task!" << std::endl;
        return 42;
    });


    std::cout << "Result1: " << future1.get() << std::endl;
    std::cout << "Result2: " << future2.get() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}