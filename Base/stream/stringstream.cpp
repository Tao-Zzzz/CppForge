#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::string csvData = "John,25,80.5";
    std::string name;
    int age;
    double score;

    std::stringstream ss(csvData);
    std::getline(ss, name, ','); // 使用逗号作为分隔符
    ss >> age;
    ss.ignore(1); // 忽略逗号
    ss >> score;

    std::cout << "Name: " << name << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "Score: " << score << std::endl;

    return 0;
}