#include <iostream>
#include <sstream>
#include <string>

int main()
{
    std::string data = "2023 JohnDoe 85.6";
    int year;
    std::string name;
    double grade;

    std::stringstream ss(data);
    ss >> year >> name >> grade;

    std::cout << "Year: " << year << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Grade: " << grade << std::endl;

    return 0;
}