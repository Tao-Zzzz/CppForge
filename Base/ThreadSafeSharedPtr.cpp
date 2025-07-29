#include <iostream>
#include <atomic>
#include <mutex>
#include <thread>

// 控制块结构
struct ControlBlock
{
    std::atomic<int> ref_count;

    ControlBlock() : ref_count(1) {}
};

// 线程安全的 shared_ptr 实现
template <typename T>
class ThreadSafeSharedPtr
{
private:
    T *ptr;                // 指向管理的对象
    ControlBlock *control; // 指向控制块

    // 互斥锁，用于保护 ptr 和 control
    mutable std::mutex mtx;

    // 释放当前资源
    void release()
    {
        if (control)
        {
            // 原子递减引用计数
            if (--(control->ref_count) == 0)
            {
                delete ptr;
                delete control;
                std::cout << "Resource and ControlBlock destroyed." << std::endl;
            }
            else
            {
                std::cout << "Decremented ref_count to " << control->ref_count.load() << std::endl;
            }
        }
        ptr = nullptr;
        control = nullptr;
    }

public:
    // 默认构造函数
    ThreadSafeSharedPtr() : ptr(nullptr), control(nullptr)
    {
        std::cout << "Default constructed ThreadSafeSharedPtr (nullptr)." << std::endl;
    }

    // 参数化构造函数
    explicit ThreadSafeSharedPtr(T *p) : ptr(p)
    {
        if (p)
        {
            control = new ControlBlock();
            std::cout << "Constructed ThreadSafeSharedPtr, ref_count = " << control->ref_count.load() << std::endl;
        }
        else
        {
            control = nullptr;
        }
    }

    // 拷贝构造函数
    ThreadSafeSharedPtr(const ThreadSafeSharedPtr &other)
    {
        std::lock_guard<std::mutex> lock(other.mtx);
        ptr = other.ptr;
        control = other.control;
        if (control)
        {
            control->ref_count++;
            std::cout << "Copied ThreadSafeSharedPtr, ref_count = " << control->ref_count.load() << std::endl;
        }
    }

    // 拷贝赋值操作符
    ThreadSafeSharedPtr &operator=(const ThreadSafeSharedPtr &other)
    {
        if (this != &other)
        {
            // 为避免死锁，使用 std::scoped_lock 同时锁定两个互斥锁
            std::scoped_lock lock(mtx, other.mtx);
            release();
            ptr = other.ptr;
            control = other.control;
            if (control)
            {
                control->ref_count++;
                std::cout << "Assigned ThreadSafeSharedPtr, ref_count = " << control->ref_count.load() << std::endl;
            }
        }
        return *this;
    }

    // 移动构造函数
    ThreadSafeSharedPtr(ThreadSafeSharedPtr &&other) noexcept
    {
        std::lock_guard<std::mutex> lock(other.mtx);
        ptr = other.ptr;
        control = other.control;
        other.ptr = nullptr;
        other.control = nullptr;
        std::cout << "Moved ThreadSafeSharedPtr." << std::endl;
    }

    // 移动赋值操作符
    ThreadSafeSharedPtr &operator=(ThreadSafeSharedPtr &&other) noexcept
    {
        if (this != &other)
        {
            // 为避免死锁，使用 std::scoped_lock 同时锁定两个互斥锁
            std::scoped_lock lock(mtx, other.mtx);
            release();
            ptr = other.ptr;
            control = other.control;
            other.ptr = nullptr;
            other.control = nullptr;
            std::cout << "Move-assigned ThreadSafeSharedPtr." << std::endl;
        }
        return *this;
    }

    // 析构函数
    ~ThreadSafeSharedPtr()
    {
        release();
    }

    // 解引用操作符
    T &operator*() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return *ptr;
    }

    // 箭头操作符
    T *operator->() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return ptr;
    }

    // 获取引用计数
    int use_count() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return control ? control->ref_count.load() : 0;
    }

    // 获取裸指针
    T *get() const
    {
        std::lock_guard<std::mutex> lock(mtx);
        return ptr;
    }

    // 重置指针
    void reset(T *p = nullptr)
    {
        std::lock_guard<std::mutex> lock(mtx);
        release();
        ptr = p;
        if (p)
        {
            control = new ControlBlock();
            std::cout << "Reset ThreadSafeSharedPtr, ref_count = " << control->ref_count.load() << std::endl;
        }
        else
        {
            control = nullptr;
        }
    }
};

// 测试类
class Test
{
public:
    Test(int val) : value(val)
    {
        std::cout << "Test Constructor: " << value << std::endl;
    }
    ~Test()
    {
        std::cout << "Test Destructor: " << value << std::endl;
    }
    void show() const
    {
        std::cout << "Value: " << value << std::endl;
    }

private:
    int value;
};