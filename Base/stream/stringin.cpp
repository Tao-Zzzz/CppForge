#include <iostream>
#include <sstream>
#include <string>

int main()
{
    int id = 101;
    std::string name = "Alice";
    double score = 95.5;

    std::stringstream ss;
    ss << "ID: " << id << ", Name: " << name << ", Score: " << score;

    std::string result = ss.str();
    std::cout << result << std::endl;

    return 0;
}