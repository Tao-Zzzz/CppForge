#include <iostream>
#include "SharedPtr.hpp" // ������� SharedPtr �ڴ�ͷ�ļ�


void testSharedPtr() {
    // ���� 1: �������������
    {
        forge::SharedPtr<int> sp1(new int(10));
        std::cout << "Test 1: *sp1 = " << *sp1 << ", use_count = " << sp1.use_count() << "\n"; // Ӧ��� 10, 1
    } // sp1 ���٣��ڴ��ͷ�

    // ���� 2: ��������
    {
        forge::SharedPtr<int> sp2(new int(20));
        forge::SharedPtr<int> sp3(sp2);
        std::cout << "Test 2: *sp2 = " << *sp2 << ", *sp3 = " << *sp3 << ", use_count = " << sp2.use_count() << "\n"; // Ӧ��� 20, 20, 2
    } // sp2, sp3 ���٣��ڴ��ͷ�

    // ���� 3: ������ֵ
    {
        forge::SharedPtr<int> sp4(new int(30));
        forge::SharedPtr<int> sp5;
        sp5 = sp4;
        std::cout << "Test 3: *sp5 = " << *sp5 << ", use_count = " << sp5.use_count() << "\n"; // Ӧ��� 30, 2
    }

    // ���� 4: �ƶ�����
    {
        forge::SharedPtr<int> sp6(new int(40));
        forge::SharedPtr<int> sp7(std::move(sp6));
        std::cout << "Test 4: *sp7 = " << *sp7 << ", sp6.get() = " << sp6.get() << ", use_count = " << sp7.use_count() << "\n"; // Ӧ��� 40, nullptr, 1
    }

    // ���� 5: �ƶ���ֵ
    {
        forge::SharedPtr<int> sp8(new int(50));
        forge::SharedPtr<int> sp9;
        sp9 = std::move(sp8);
        std::cout << "Test 5: *sp9 = " << *sp9 << ", sp8.get() = " << sp8.get() << ", use_count = " << sp9.use_count() << "\n"; // Ӧ��� 50, nullptr, 1
    }

    // ���� 6: reset
    {
        forge::SharedPtr<int> sp10(new int(60));
        sp10.reset();
        std::cout << "Test 6: sp10.get() = " << sp10.get() << ", use_count = " << sp10.use_count() << "\n"; // Ӧ��� nullptr, 0
    }

    // ���� 7: ��Ա����
    {
        struct Test { int x = 100; };
        forge::SharedPtr<Test> sp11(new Test);
        std::cout << "Test 7: sp11->x = " << sp11->x << "\n"; // Ӧ��� 100
    }
}

int main() {
    testSharedPtr();
    return 0;
}