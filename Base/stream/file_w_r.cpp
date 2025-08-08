#include <iostream>
#include <fstream>

int main()
{
    // 以二进制模式写入数据
    std::ofstream outfile("binary.dat", std::ios::binary);
    if (!outfile)
    {
        std::cerr << "无法打开二进制文件。" << std::endl;
        return 1;
    }

    int number = 12345;
    outfile.write(reinterpret_cast<char *>(&number), sizeof(number));
    outfile.close();

    // 以二进制模式读取数据
    std::ifstream infile("binary.dat", std::ios::binary);
    if (!infile)
    {
        std::cerr << "无法打开二进制文件进行读取。" << std::endl;
        return 1;
    }

    int readNumber;
    infile.read(reinterpret_cast<char *>(&readNumber), sizeof(readNumber));
    std::cout << "读取的数字是：" << readNumber << std::endl;

    infile.close();
    return 0;
}