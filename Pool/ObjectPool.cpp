#include <iostream>
#include <stack>
#include <memory>
#include <mutex>


template<typename T>
class ObjectPool{
public:
    ObjectPool(size_t preallocate = 0){
        std::lock_guard<std::mutex> lock(mtx);
        for(size_t i = 0; i < preallocate; ++i){
            pool.push(std::make_shared<T>());
        }
    }

    ObjectPool(const ObjectPool&) = delete;
    ObjectPool& operator=(const ObjectPool&) = delete;

    ~ObjectPool() = default;

    std::shared_ptr<T> acquire(){
        std::lock_guard<std::mutex> lock(mtx);
        if(pool.empty()){
            return std::make_shared<T>();
        }else{
            std::shared_ptr<T> obj = pool.top();
            pool.pop();
            return obj;
        }
    }

    void release(std::shared_ptr<T> obj){
        std::lock_guard<std::mutex> lock(mtx);
        pool.push(obj);
    }

private:
    std::stack<std::shared_ptr<T>> pool;
    std::mutex mtx;
};

class MyObject{
public:
    static int count;
    MyObject(){
        count ++;
        std::cout << "MyObject constructed. Total: " << count << std::endl;
    }
    ~MyObject(){
        count --;
        std::cout << "MyObject destructed. Total: " << count << std::endl;
    }
    void do_something(){
        std::cout << "MyObject doing something. Total: " << count << std::endl;
    }
};

int MyObject::count = 0;

int main(){
    // 创建一个预分配 3 个对象的对象池
    ObjectPool<MyObject> myObjectPool(3);

    std::cout << "\n--- Acquiring objects ---" << std::endl;
    std::shared_ptr<MyObject> obj1 = myObjectPool.acquire(); // 从池中获取
    std::shared_ptr<MyObject> obj2 = myObjectPool.acquire(); // 从池中获取
    std::shared_ptr<MyObject> obj3 = myObjectPool.acquire(); // 从池中获取
    std::shared_ptr<MyObject> obj4 = myObjectPool.acquire(); // 池空，创建新对象

    obj1->do_something();
    obj2->do_something();

    std::cout << "\n--- Releasing objects ---" << std::endl;
    myObjectPool.release(obj1); // 归还 obj1 到池中
    myObjectPool.release(obj2); // 归还 obj2 到池中

    // obj3 和 obj4 没有归还，它们会在各自的 shared_ptr 超出作用域时被销毁
    // 观察 MyObject 的析构函数调用时机

    std::cout << "\n--- Acquiring again ---" << std::endl;
    std::shared_ptr<MyObject> obj5 = myObjectPool.acquire(); // 从池中获取之前归还的 obj1
    std::shared_ptr<MyObject> obj6 = myObjectPool.acquire(); // 从池中获取之前归还的 obj2

    obj5->do_something();

    std::cout << "\n--- End of main ---" << std::endl;
    // 当 main 函数结束时，所有 shared_ptr（包括 obj3, obj4, obj5, obj6）
    // 会被销毁，并触发对应 MyObject 的析构函数。
    // ObjectPool 析构时，池中剩余的 shared_ptr 也会被销毁。
    return 0;
}