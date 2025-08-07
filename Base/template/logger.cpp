#include <iostream>

// Base case for recursion: no arguments
void logMessages(){

}

template<typename T, typename ...arg>
void logMessages(const T& frist, const arg&... rest){
    std::cout << first << std::endl;
    logMessages(rest...);
}

int main()
{
    // Test case 1: Mixed types (int, double, string, char)
    logMessages(42, 3.14, std::string("Hello"), 'A');

    // Test case 2: Single argument (string)
    logMessages("Single message");

    // Test case 3: Multiple strings
    logMessages("First", "Second", "Third");

    // Test case 4: No arguments
    logMessages();

    // Test case 5: Numeric types only
    logMessages(1, 2.5, 3.7f);

    return 0;
}