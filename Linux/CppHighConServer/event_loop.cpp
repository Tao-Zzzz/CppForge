#include "event_loop.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <set>

EventLoop::EventLoop(int thread_num) : _next_idx(0), _thread_num(thread_num)
{
    std::cout << "construt event_loop num is " << thread_num << std::endl;
    _work_threads.reserve(thread_num);
    for (int i = 0; i < thread_num; ++i)
    {
        // 1) 创建一个新的 IOThread
        auto thr = std::make_unique<IOThread>(i);
        // 2) 启动它
        thr->start();
        // 3) 再把它放进容器
        _work_threads.emplace_back(std::move(thr));
    }
}

EventLoop::~EventLoop()
{
    for (int i = 0; i < _thread_num; ++i)
    {
        _work_threads[i]->join();
    }

    std::cout << "EventLoop exit" << std::endl;
}

void EventLoop::StopIOThread()
{
    for (int i = 0; i < _thread_num; ++i)
    {
        _work_threads[i]->stop();
    }
}

void EventLoop::NotifyNewCons(std::vector<int> &conns)
{

    std::set<int> notify_threads;
    for (int i = 0; i < conns.size(); i++)
    {
        auto fd = conns[i];
        auto index = fd % _work_threads.size();
        _work_threads[index]->catche_new_con(fd);
        notify_threads.insert(index);
    }

    for (auto index : notify_threads)
    {
        _work_threads[index]->wakeup();
    }
}
