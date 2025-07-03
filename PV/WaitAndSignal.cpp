#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::queue<int> queue;
std::mutex mtx;
std::condition_variable cv;
const int MAX_SIZE = 5;

void producer(){
    for(int i = 0; i < 5; i++){
        {
            std::unique_lock<std::mutex> lock(mtx);
            while(queue.size() >= MAX_SIZE){
                cv.wait(lock);
            }
            queue.push(i);
            std::cout << "Produced: " << i << std::endl;
            lock.unlock();
            cv.notify_one();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(){
    while(true){
        {
            std::unique_lock<std::mutex> lock(mtx);
            while(queue.empty()){
                cv.wait(lock);
            }
            int data = queue.front();
            queue.pop();
            std::cout << "Consumed: " << data << std::endl;
            lock.unlock();
            cv.notify_one();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::cout << "i am still working" << std::endl;
    }
}

int main(){
    std::thread prod(producer);
    std::thread cons(consumer);
    prod.join();
    cons.detach();
    //std::cout << "come to this ?" << std::endl;
    return 0;
}