#include <atomic>
#include <iostream>
#include <thread>
#include <cassert>

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x_then_y()
{
    x.store(true, std::memory_order_seq_cst); // 1
    y.store(true, std::memory_order_seq_cst); // 2
}

void read_y_then_x()
{
    while (!y.load(std::memory_order_seq_cst))
    { // 3
        std::cout << "y load false" << std::endl;
    }

    if (x.load(std::memory_order_seq_cst))
    { // 4
        ++z;
    }
}

void TestOrderSeqCst()
{

    std::thread t1(write_x_then_y);
    std::thread t2(read_y_then_x);
    t1.join();
    t2.join();
    assert(z.load() != 0); // 5
}

void TestOrderRelaxed()
{
    std::atomic<bool> rx, ry;

    std::thread t1([&]()
                   {
                       rx.store(true, std::memory_order_relaxed); // 1
                       ry.store(true, std::memory_order_relaxed); // 2
                   });

    std::thread t2([&]()
                   {
                       while (!ry.load(std::memory_order_relaxed))
                           ;                                       // 3
                       assert(rx.load(std::memory_order_relaxed)); // 4
                   });

    t1.join();
    t2.join();
}

void TestReleaseAcquire()
{
    std::atomic<bool> rx, ry;

    std::thread t1([&]()
                   {
                       rx.store(true, std::memory_order_relaxed); // 1
                       ry.store(true, std::memory_order_release); // 2
                   });

    std::thread t2([&]()
                   {
                       while (!ry.load(std::memory_order_acquire))
                           ;                                       // 3
                       assert(rx.load(std::memory_order_relaxed)); // 4
                   });

    t1.join();
    t2.join();
}

void ConsumeDependency()
{
    std::atomic<std::string *> ptr;
    int data;

    std::thread t1([&]()
                   {
                       std::string *p = new std::string("Hello World"); // (1)
                       data = 42;                                       // (2)
                       ptr.store(p, std::memory_order_release);         // (3)
                   });

    std::thread t2([&]()
                   {
                       std::string *p2;
                       while (!(p2 = ptr.load(std::memory_order_consume)))
                           ;                         // (4)
                       assert(*p2 == "Hello World"); // (5)
                       assert(data == 42);           // (6)
                   });

    t1.join();
    t2.join();
}