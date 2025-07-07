#include <iostream>
#include <stack>
#include <memory>
#include <mutex>
#include <cassert>

template <typename T>
class MemoryPool
{
public:
    explicit MemoryPool(size_t preallocate = 0)
    {
        for (size_t i = 0; i < preallocate; ++i)
        {
            void *block = ::operator new(sizeof(T));
            freeBlocks.push(block);
        }
    }

    ~MemoryPool()
    {
        while (!freeBlocks.empty())
        {
            ::operator delete(freeBlocks.top());
            freeBlocks.pop();
        }
    }

    // 分配一个对象（构造 + 包成 shared_ptr 自动释放）
    template <typename... Args>
    std::shared_ptr<T> create(Args &&...args)
    {
        void *mem = nullptr;

        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!freeBlocks.empty())
            {
                mem = freeBlocks.top();
                freeBlocks.pop();
            }
            else
            {
                mem = ::operator new(sizeof(T));
            }
        }

        // 在预分配内存上构造对象
        T *obj = new (mem) T(std::forward<Args>(args)...);

        // 自定义 deleter：先析构，再归还内存
        return std::shared_ptr<T>(obj, [this](T *ptr)
                                  {
            ptr->~T();
            std::lock_guard<std::mutex> lock(mtx);
            freeBlocks.push(static_cast<void*>(ptr)); });
    }

private:
    std::stack<void *> freeBlocks;
    std::mutex mtx;
};
