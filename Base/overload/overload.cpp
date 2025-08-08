#include <iostream>
#include <cmath>
#include <stdexcept>

class Complex
{
private:
    double real;
    double imag;

public:
    // 构造函数
    Complex(double r = 0.0, double i = 0.0) : real(r), imag(i) {}

    // 拷贝赋值运算符
    Complex &operator=(const Complex &other)
    {
        if (this == &other)
            return *this;
        this->real = other.real;
        this->imag = other.imag;
        return *this;
    }

    // 重载 + 运算符（成员函数）
    Complex operator+(const Complex &other) const
    {
        return Complex(this->real + other.real, this->imag + other.imag);
    }

    // 重载 - 运算符（成员函数）
    Complex operator-(const Complex &other) const
    {
        return Complex(this->real - other.real, this->imag - other.imag);
    }

    // 重载 * 运算符（成员函数）
    Complex operator*(const Complex &other) const
    {
        double r = this->real * other.real - this->imag * other.imag;
        double i = this->real * other.imag + this->imag * other.real;
        return Complex(r, i);
    }

    // 重载 / 运算符（成员函数）
    Complex operator/(const Complex &other) const
    {
        double denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0)
        {
            throw std::invalid_argument("除数为零！");
        }
        double r = (this->real * other.real + this->imag * other.imag) / denominator;
        double i = (this->imag * other.real - this->real * other.imag) / denominator;
        return Complex(r, i);
    }

    // 重载 += 运算符
    Complex &operator+=(const Complex &other)
    {
        this->real += other.real;
        this->imag += other.imag;
        return *this;
    }

    // 重载 -= 运算符
    Complex &operator-=(const Complex &other)
    {
        this->real -= other.real;
        this->imag -= other.imag;
        return *this;
    }

    // 重载 *= 运算符
    Complex &operator*=(const Complex &other)
    {
        double r = this->real * other.real - this->imag * other.imag;
        double i = this->real * other.imag + this->imag * other.real;
        this->real = r;
        this->imag = i;
        return *this;
    }

    // 重载 /= 运算符
    Complex &operator/=(const Complex &other)
    {
        double denominator = other.real * other.real + other.imag * other.imag;
        if (denominator == 0)
        {
            throw std::invalid_argument("除数为零！");
        }
        double r = (this->real * other.real + this->imag * other.imag) / denominator;
        double i = (this->imag * other.real - this->real * other.imag) / denominator;
        this->real = r;
        this->imag = i;
        return *this;
    }

    // 重载 == 运算符
    friend bool operator==(const Complex &lhs, const Complex &rhs)
    {
        return (lhs.real == rhs.real) && (lhs.imag == rhs.imag);
    }

    // 重载 != 运算符
    friend bool operator!=(const Complex &lhs, const Complex &rhs)
    {
        return !(lhs == rhs);
    }

    // 重载 < 运算符（基于模长）
    friend bool operator<(const Complex &lhs, const Complex &rhs)
    {
        double lhs_mod = std::sqrt(lhs.real * lhs.real + lhs.imag * lhs.imag);
        double rhs_mod = std::sqrt(rhs.real * rhs.real + rhs.imag * rhs.imag);
        return lhs_mod < rhs_mod;
    }

    // 重载 > 运算符
    friend bool operator>(const Complex &lhs, const Complex &rhs)
    {
        return rhs < lhs;
    }

    // 重载 <= 运算符
    friend bool operator<=(const Complex &lhs, const Complex &rhs)
    {
        return !(rhs < lhs);
    }

    // 重载 >= 运算符
    friend bool operator>=(const Complex &lhs, const Complex &rhs)
    {
        return !(lhs < rhs);
    }

    // 重载 << 运算符
    friend std::ostream &operator<<(std::ostream &os, const Complex &c)
    {
        os << "(" << c.real;
        if (c.imag >= 0)
            os << " + " << c.imag << "i)";
        else
            os << " - " << -c.imag << "i)";
        return os;
    }

    // 重载 >> 运算符
    friend std::istream &operator>>(std::istream &is, Complex &c)
    {
        // 简单输入格式：real imag
        is >> c.real >> c.imag;
        return is;
    }

    // 重载 ~ 运算符（取反复数）
    Complex operator~() const
    {
        return Complex(this->real, -this->imag);
    }

    // 重载逻辑非运算符（!）
    bool operator!() const
    {
        return (this->real == 0 && this->imag == 0);
    }

    // 重载下标运算符（如 c[0] 返回 real, c[1] 返回 imag）
    double &operator[](size_t index)
    {
        if (index == 0)
            return real;
        else if (index == 1)
            return imag;
        else
            throw std::out_of_range("下标越界！");
    }

    const double &operator[](size_t index) const
    {
        if (index == 0)
            return real;
        else if (index == 1)
            return imag;
        else
            throw std::out_of_range("下标越界！");
    }

    // 重载前置 ++ 运算符
    Complex &operator++()
    {
        ++real;
        ++imag;
        return *this;
    }

    // 重载后置 ++ 运符
    Complex operator++(int)
    {
        Complex temp = *this;
        ++real;
        ++imag;
        return temp;
    }

    // 重载前置 -- 运算符
    Complex &operator--()
    {
        --real;
        --imag;
        return *this;
    }

    // 重载后置 -- 运算符
    Complex operator--(int)
    {
        Complex temp = *this;
        --real;
        --imag;
        return temp;
    }

    // 重载函数调用运算符
    double operator()(const std::string &part) const
    {
        if (part == "real")
            return real;
        else if (part == "imag")
            return imag;
        else
            throw std::invalid_argument("参数错误！");
    }
};

// 示例
int main()
{
    Complex c1(3.0, 4.0);
    Complex c2(1.5, -2.5);
    Complex c3;

    // 赋值运算
    c3 = c1;
    std::cout << "c3 = " << c3 << std::endl;

    // 加法
    Complex c4 = c1 + c2;
    std::cout << "c1 + c2 = " << c4 << std::endl;

    // 减法
    Complex c5 = c1 - c2;
    std::cout << "c1 - c2 = " << c5 << std::endl;

    // 乘法
    Complex c6 = c1 * c2;
    std::cout << "c1 * c2 = " << c6 << std::endl;

    // 除法
    try
    {
        Complex c7 = c1 / c2;
        std::cout << "c1 / c2 = " << c7 << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << "错误: " << e.what() << std::endl;
    }

    // 比较运算
    if (c1 > c2)
        std::cout << "c1 的模长大于 c2" << std::endl;
    else
        std::cout << "c1 的模长不大于 c2" << std::endl;

    // 逻辑非运算
    Complex c_zero;
    if (!c1)
        std::cout << "c1 是零复数" << std::endl;
    else
        std::cout << "c1 不是零复数" << std::endl;

    if (!c_zero)
        std::cout << "c_zero 是零复数" << std::endl;
    else
        std::cout << "c_zero 不是零复数" << std::endl;

    // 取反运算
    Complex c_neg = ~c1;
    std::cout << "~c1 = " << c_neg << std::endl;

    // 下标运算
    try
    {
        std::cout << "c1[0] (real) = " << c1[0] << std::endl;
        std::cout << "c1[1] (imag) = " << c1[1] << std::endl;
        // std::cout << "c1[2] = " << c1[2] << std::endl; // 会抛出异常
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "错误: " << e.what() << std::endl;
    }

    // 自增自减运算符
    std::cout << "c1 = " << c1 << std::endl;
    std::cout << "++c1 = " << ++c1 << std::endl;
    std::cout << "c1++ = " << c1++ << std::endl;
    std::cout << "c1 = " << c1 << std::endl;
    std::cout << "--c1 = " << --c1 << std::endl;
    std::cout << "c1-- = " << c1-- << std::endl;
    std::cout << "c1 = " << c1 << std::endl;

    // 函数调用运算符
    std::cout << "c1 的实部: " << c1("real") << std::endl;
    std::cout << "c1 的虚部: " << c1("imag") << std::endl;

    // 输入运算
    std::cout << "请输入一个复数的实部和虚部，以空格分隔: ";
    Complex c_input;
    std::cin >> c_input;
    std::cout << "您输入的复数是: " << c_input << std::endl;

    return 0;
}