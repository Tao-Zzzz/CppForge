#include <mutex>
#include <iostream>
#include <memory>

// 局部静态变量, 不是成员变量, 返回的是引用
class Single2
{
private:
    Single2()
    {
    }
    // 禁止拷贝构造和拷贝赋值
    Single2(const Single2 &) = delete;
    Single2 &operator=(const Single2 &) = delete;

public:
    static Single2 &GetInst()
    {
        static Single2 single;
        return single;
    }
};

// 饿汉式, 解决局部静态变量线程安全问题, 成员变量, 指针, 在主函数最开始初始化
class Single2Hungry
{
private:
    Single2Hungry()
    {
    }
    Single2Hungry(const Single2Hungry &) = delete;
    Single2Hungry &operator=(const Single2Hungry &) = delete;

public:
    static Single2Hungry *GetInst()
    {
        if (single == nullptr)
        {
            single = new Single2Hungry();
        }
        return single;
    }

private:
    static Single2Hungry *single;
};

// 懒汉式, 要加锁, 什么时候初始化都可以, 指针, 但是不知道在哪释放
class SinglePointer
{
private:
    SinglePointer()
    {
    }
    SinglePointer(const SinglePointer &) = delete;
    SinglePointer &operator=(const SinglePointer &) = delete;

public:
    static SinglePointer *GetInst()
    {
        if (single != nullptr)
        {
            return single;
        }
        // 非空则加锁处理
        s_mutex.lock();
        // 加锁后再次检查是否已经初始化
        if (single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        single = new SinglePointer();
        s_mutex.unlock();
        return single;
    }

private:
    static SinglePointer *single;
    static std::mutex s_mutex;
};


// 可以利用智能指针完成自动回收, 也是懒汉,
class SingleAuto
{
private:
    SingleAuto()
    {
    }
    SingleAuto(const SingleAuto &) = delete;
    SingleAuto &operator=(const SingleAuto &) = delete;

public:
    ~SingleAuto()
    {
        std::cout << "single auto delete success " << std::endl;
    }
    static std::shared_ptr<SingleAuto> GetInst()
    {
        if (single != nullptr)
        {
            return single;
        }
        s_mutex.lock();
        if (single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        single = std::shared_ptr<SingleAuto>(new SingleAuto);
        s_mutex.unlock();
        return single;
    }

private:
    static std::shared_ptr<SingleAuto> single;
    static std::mutex s_mutex;
};


// 为了规避用户手动释放内存，可以提供一个辅助类帮忙回收内存
// 并将单例类的析构函数写为私有
class SingleAutoSafe;
class SafeDeletor
{
public:
    // 辅助类运行delete
    void operator()(SingleAutoSafe *sf)
    {
        std::cout << "this is safe deleter operator()" << std::endl;
        delete sf;
    }
};

class SingleAutoSafe
{
private:
    SingleAutoSafe() {}
    ~SingleAutoSafe()
    {
        std::cout << "this is single auto safe deletor" << std::endl;
    }
    SingleAutoSafe(const SingleAutoSafe &) = delete;
    SingleAutoSafe &operator=(const SingleAutoSafe &) = delete;
    // 定义友元类，通过友元类调用该类析构函数
    friend class SafeDeletor;

public:
    // 虽然看起来很安全, 但实际运行的可能就出现错误, 因为编译器的问题不知道
    // 是2先运行还是3先运行
    static std::shared_ptr<SingleAutoSafe> GetInst()
    {
        // 1处
        if (single != nullptr)
        {
            return single;
        }
        s_mutex.lock();
        // 2处
        if (single != nullptr)
        {
            s_mutex.unlock();
            return single;
        }
        // 额外指定删除器
        // 3 处
        single = std::shared_ptr<SingleAutoSafe>(new SingleAutoSafe, SafeDeletor());
        // 也可以指定删除函数
        //  single = std::shared_ptr<SingleAutoSafe>(new SingleAutoSafe, SafeDelFunc);
        s_mutex.unlock();
        return single;
    }

private:
    static std::shared_ptr<SingleAutoSafe> single;
    static std::mutex s_mutex;
};


class SingletonOnce
{
private:
    SingletonOnce() = default;
    SingletonOnce(const SingletonOnce &) = delete;
    SingletonOnce &operator=(const SingletonOnce &st) = delete;
    static std::shared_ptr<SingletonOnce> _instance;

public:
    static std::shared_ptr<SingletonOnce> GetInstance()
    {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]()
                       { _instance = std::shared_ptr<SingletonOnce>(new SingletonOnce); });

        return _instance;
    }

    void PrintAddress()
    {
        std::cout << _instance.get() << std::endl;
    }

    ~SingletonOnce()
    {
        std::cout << "this is singleton destruct" << std::endl;
    }
};

std::shared_ptr<SingletonOnce> SingletonOnce::_instance = nullptr;