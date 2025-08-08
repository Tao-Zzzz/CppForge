#include <iostream>
#include <fstream>
#include <string>

int main()
{
    // 创建一个 fstream 对象，并以读写模式打开文件 "data.txt"
    std::fstream file("data.txt", std::ios::in | std::ios::out | std::ios::app);

    // 检查文件是否成功打开
    if (!file)
    {
        std::cerr << "无法打开文件进行读写。" << std::endl;
        return 1;
    }

    // 向文件追加新内容
    file << "追加的一行内容。\n";

    // 将文件指针移动到文件开头以读取内容
    file.seekg(0, std::ios::beg);

    std::string line;

    std::cout << "文件内容如下：" << std::endl;

    // 读取文件内容并输出
    while (std::getline(file, line))
    {
        std::cout << line << std::endl;
    }

    // 关闭文件
    file.close();

    return 0;
}