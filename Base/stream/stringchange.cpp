#include <iostream>
#include <sstream>
#include <string>

int main()
{
    // 字符串转换为整数和浮点数
    std::string numStr = "12345";
    std::string floatStr = "98.76";
    int number;
    double decimal;

    std::stringstream ss1(numStr);
    ss1 >> number;

    std::stringstream ss2(floatStr);
    ss2 >> decimal;

    std::cout << "字符串 \"" << numStr << "\" 转换为整数：" << number << std::endl;
    std::cout << "字符串 \"" << floatStr << "\" 转换为浮点数：" << decimal << std::endl;

    // 数字转换为字符串
    int num = 6789;
    double dec = 12.34;
    std::stringstream ss3;
    ss3 << num << " and " << dec;

    std::string combinedStr = ss3.str();
    std::cout << "数字转换为字符串：" << combinedStr << std::endl;

    return 0;
}