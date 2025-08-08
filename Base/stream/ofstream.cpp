#include <iostream>
#include <iomanip> // 引入iomanip头文件以使用操纵器

int main()
{
    double pi = 3.141592653589793;
    int num = 42;

    // 设置浮点数的显示精度
    std::cout << "Pi with default precision: " << pi << std::endl;
    std::cout << "Pi with 4 decimal places: " << std::fixed << std::setprecision(4) << pi << std::endl;

    // 设置字段宽度和对齐方式
    std::cout << "Number with width 5: |" << std::setw(5) << num << "|" << std::endl;
    std::cout << "Number with width 5 (left aligned): |" << std::left << std::setw(5) << num << "|" << std::endl;

    return 0;
}