#include <iostream>
#include <fstream>
#include <string>
#include <map>

int main()
{
    // >> 可以按单词逐个读取
    std::ifstream infile("input.txt");
    if (!infile)
    {
        std::cerr << "无法打开 input.txt 进行读取。" << std::endl;
        return 1;
    }

    std::map<std::string, int> wordCount;
    std::string word;

    // 逐词读取并统计
    while (infile >> word)
    {
        // 可以添加更多的处理，如去除标点符号、转换为小写等
        wordCount[word]++;
    }

    infile.close();

    std::ofstream outfile("word_count.txt");
    if (!outfile)
    {
        std::cerr << "无法打开 word_count.txt 进行写入。" << std::endl;
        return 1;
    }

    // 写入统计结果
    for (const auto &pair : wordCount)
    {
        outfile << pair.first << ": " << pair.second << std::endl;
    }

    outfile.close();

    std::cout << "单词统计已完成，结果保存在 word_count.txt 中。" << std::endl;

    return 0;
}