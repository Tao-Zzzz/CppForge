#include <atomic>
#include <memory>
#include <iostream>

template <typename T, size_t Cap>
class CircularQueLight : private std::allocator<T>
{
public:
    CircularQueLight() : _max_size(Cap + 1),
                         _data(std::allocator<T>::allocate(_max_size)), _head(0), _tail(0) {}

    CircularQueLight(const CircularQueLight &) = delete;
    CircularQueLight &operator=(const CircularQueLight &) volatile = delete;
    CircularQueLight &operator=(const CircularQueLight &) = delete;

    bool pop(T &val)
    {

        size_t h;
        do
        {
            h = _head.load(std::memory_order_relaxed); // 1 处
            // 判断头部和尾部指针是否重合，如果重合则队列为空
            if (h == _tail.load(std::memory_order_acquire)) // 2处
            {
                std::cout << "circular que empty ! " << std::endl;
                return false;
            }

            // 判断如果此时要读取的数据和tail_update是否一致，如果一致说明尾部数据未更新完
            if (h == _tail_update.load(std::memory_order_acquire)) // 3处
            {
                return false;
            }
            val = _data[h]; // 2处

        } while (!_head.compare_exchange_strong(h,
                                                (h + 1) % _max_size, std::memory_order_release, std::memory_order_relaxed)); // 4 处
        std::cout << "pop data success, data is " << val << std::endl;
        return true;
    }

    bool push(const T &val)
    {
        size_t t;
        do
        {
            t = _tail.load(std::memory_order_relaxed); // 5
            // 判断队列是否满
            if ((t + 1) % _max_size == _head.load(std::memory_order_acquire))
            {
                std::cout << "circular que full ! " << std::endl;
                return false;
            }

        } while (!_tail.compare_exchange_strong(t,
                                                (t + 1) % _max_size, std::memory_order_release, std::memory_order_relaxed)); // 6

        _data[t] = val;
        size_t tailup;
        do
        {
            tailup = t;

        } while (_tail_update.compare_exchange_strong(tailup,
                                                      (tailup + 1) % _max_size, std::memory_order_release, std::memory_order_relaxed)); // 7

        std::cout << "called push data success " << val << std::endl;
        return true;
    }

private:
    size_t _max_size;
    T *_data;
    std::atomic<size_t> _head;
    std::atomic<size_t> _tail;
};