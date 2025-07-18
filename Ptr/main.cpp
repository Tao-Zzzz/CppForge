#include <iostream>
#include "SharedPtr.hpp" // 假设你的 SharedPtr 在此头文件
#include "UniquePtr.hpp"
#include "WeakPtr.hpp"


void testSharedPtr() {
    // 测试 1: 基本构造和析构
    {
        forge::SharedPtr<int> sp1(new int(10));
        std::cout << "Test 1: *sp1 = " << *sp1 << ", use_count = " << sp1.use_count() << "\n"; // 应输出 10, 1
    } // sp1 销毁，内存释放

    // 测试 2: 拷贝构造
    {
        forge::SharedPtr<int> sp2(new int(20));
        forge::SharedPtr<int> sp3(sp2);
        std::cout << "Test 2: *sp2 = " << *sp2 << ", *sp3 = " << *sp3 << ", use_count = " << sp2.use_count() << "\n"; // 应输出 20, 20, 2
    } // sp2, sp3 销毁，内存释放

    // 测试 3: 拷贝赋值
    {
        forge::SharedPtr<int> sp4(new int(30));
        forge::SharedPtr<int> sp5;
        sp5 = sp4;
        std::cout << "Test 3: *sp5 = " << *sp5 << ", use_count = " << sp5.use_count() << "\n"; // 应输出 30, 2
    }

    // 测试 4: 移动构造
    {
        forge::SharedPtr<int> sp6(new int(40));
        forge::SharedPtr<int> sp7(std::move(sp6));
        std::cout << "Test 4: *sp7 = " << *sp7 << ", sp6.get() = " << sp6.get() << ", use_count = " << sp7.use_count() << "\n"; // 应输出 40, nullptr, 1
    }

    // 测试 5: 移动赋值
    {
        forge::SharedPtr<int> sp8(new int(50));
        forge::SharedPtr<int> sp9;
        sp9 = std::move(sp8);
        std::cout << "Test 5: *sp9 = " << *sp9 << ", sp8.get() = " << sp8.get() << ", use_count = " << sp9.use_count() << "\n"; // 应输出 50, nullptr, 1
    }

    // 测试 6: reset
    {
        forge::SharedPtr<int> sp10(new int(60));
        sp10.reset();
        std::cout << "Test 6: sp10.get() = " << sp10.get() << ", use_count = " << sp10.use_count() << "\n"; // 应输出 nullptr, 0
    }

    // 测试 7: 成员访问
    {
        struct Test { int x = 100; };
        forge::SharedPtr<Test> sp11(new Test);
        std::cout << "Test 7: sp11->x = " << sp11->x << "\n"; // 应输出 100
    }
}

void testUniqueAndWeakPtr() {
    // 测试 UniquePtr
    {
        forge::UniquePtr<int> up1(new int(10));
        std::cout << "UniquePtr Test 1: *up1 = " << *up1 << ", get = " << up1.get() << "\n"; // 应输出 10, 非空地址

        forge::UniquePtr<int> up2 = std::move(up1);
        std::cout << "UniquePtr Test 2: *up2 = " << *up2 << ", up1.get() = " << up1.get() << "\n"; // 应输出 10, nullptr

        up2.reset(new int(20));
        std::cout << "UniquePtr Test 3: *up2 = " << *up2 << "\n"; // 应输出 20

        int* raw = up2.release();
        std::cout << "UniquePtr Test 4: raw = " << *raw << ", up2.get() = " << up2.get() << "\n"; // 应输出 20, nullptr
        delete raw;
    }

    // 测试 WeakPtr
    {
        // Test 1: Basic WeakPtr creation
        forge::SharedPtr<int> sp1(new int(30));
        forge::WeakPtr<int> wp1(sp1);
        std::cout << "WeakPtr Test 1: wp1.expired() = " << wp1.expired() << ", sp1.use_count() = " << sp1.use_count() << "\n"; // 0, 1

        // Test 2: Lock creates a new SharedPtr
        forge::SharedPtr<int> sp2 = wp1.lock();
        std::cout << "WeakPtr Test 2: *sp2 = " << *sp2 << ", sp1.use_count() = " << sp1.use_count() << "\n"; // 30, 2

        // Test 3: Release all SharedPtrs
        sp1.reset();
        sp2.reset(); // Ensure sp2 also releases the resource
        std::cout << "WeakPtr Test 3: wp1.expired() = " << wp1.expired() << "\n"; // Should be 1 (true)

        // Test 4: Lock after resource is freed
        forge::SharedPtr<int> sp3 = wp1.lock();
        std::cout << "WeakPtr Test 4: sp3.get() = " << sp3.get() << "\n"; // Should be nullptr
    }
}
int main() {
    testUniqueAndWeakPtr();
    //testSharedPtr();
    return 0;
}