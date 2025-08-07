#include <iostream>
#include <string>
#include <stdexcept>

// Fixed-size stack class template
template <typename T, std::size_t N>
class FixedStack
{
private:
    T elements[N];    // Array to store stack elements
    std::size_t size; // Current number of elements

public:
    // Constructor
    FixedStack() : size(0) {}

    // Push an element onto the stack
    void push(const T &value)
    {
        if (size >= N)
        {
            throw std::overflow_error("Stack is full");
        }
        elements[size++] = value;
    }

    // Pop an element from the stack
    void pop()
    {
        if (size == 0)
        {
            throw std::underflow_error("Stack is empty");
        }
        --size;
    }

    // Get the top element of the stack
    const T &top() const
    {
        if (size == 0)
        {
            throw std::underflow_error("Stack is empty");
        }
        return elements[size - 1];
    }

    // Check if the stack is empty
    bool empty() const
    {
        return size == 0;
    }

    // Check if the stack is full
    bool full() const
    {
        return size == N;
    }

    // Get the current size of the stack
    std::size_t get_size() const
    {
        return size;
    }
};

int main()
{
    // Test FixedStack<int, 5>
    FixedStack<int, 5> intStack;
    try
    {
        // Push elements
        intStack.push(10);
        intStack.push(20);
        intStack.push(30);
        std::cout << "Int Stack Top: " << intStack.top() << std::endl;       // Should print 30
        std::cout << "Int Stack Size: " << intStack.get_size() << std::endl; // Should print 3
        intStack.pop();
        std::cout << "Int Stack Top after pop: " << intStack.top() << std::endl; // Should print 20
        // Push more elements
        intStack.push(40);
        intStack.push(50);
        intStack.push(60); // Stack is full after this
        std::cout << "Int Stack Full: " << (intStack.full() ? "Yes" : "No") << std::endl;
        // intStack.push(70); // Uncommenting this should throw an overflow error
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Test FixedStack<std::string, 3>
    FixedStack<std::string, 3> stringStack;
    try
    {
        // Push elements
        stringStack.push("First");
        stringStack.push("Second");
        std::cout << "String Stack Top: " << stringStack.top() << std::endl;       // Should print Second
        std::cout << "String Stack Size: " << stringStack.get_size() << std::endl; // Should print 2
        stringStack.pop();
        std::cout << "String Stack Top after pop: " << stringStack.top() << std::endl; // Should print First
        stringStack.push("Third");
        stringStack.push("Fourth"); // Stack is full after this
        std::cout << "String Stack Full: " << (stringStack.full() ? "Yes" : "No") << std::endl;
        // stringStack.push("Fifth"); // Uncommenting this should throw an overflow error
        stringStack.pop();
        stringStack.pop();
        std::cout << "String Stack Empty: " << (stringStack.empty() ? "Yes" : "No") << std::endl;
        // stringStack.pop(); // Uncommenting this should throw an underflow error
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}