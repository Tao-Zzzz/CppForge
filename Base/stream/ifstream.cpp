#include <iostream>
#include <fstream>
#include <string> // 引入 string 头文件

int main()
{
    // 创建一个ifstream对象，并打开文件 "output.txt"
    std::ifstream infile("output.txt");

    // 检查文件是否成功打开
    if (!infile)
    {
        std::cerr << "无法打开文件进行读取。" << std::endl;
        return 1;
    }

    std::string line;

    // 逐行读取文件内容
    while (std::getline(infile, line))
    {
        std::cout << line << std::endl;
    }

    // 关闭文件
    infile.close();

    return 0;
}