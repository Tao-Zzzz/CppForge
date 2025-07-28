#include "configmgr.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

// 去除字符串首尾空白
static void trim(std::string &s)
{
    const char *ws = " \t\r\n";
    auto b = s.find_first_not_of(ws);
    auto e = s.find_last_not_of(ws);
    if (b == std::string::npos)
    {
        s.clear();
    }
    else
    {
        s = s.substr(b, e - b + 1);
    }
}

bool ConfigMgr::loadFromFile(const std::string &filepath)
{
    std::ifstream in(filepath);
    if (!in.is_open())
        return false;

    section_.clear();
    kv_.clear();

    std::string line;
    while (std::getline(in, line))
    {
        // 去除注释（; 或 # 之后全部丢弃）
        auto pos = line.find_first_of(";#");
        if (pos != std::string::npos)
        {
            line.resize(pos);
        }
        trim(line);
        if (line.empty())
            continue;

        // 处理 [section]
        if (line.front() == '[' && line.back() == ']')
        {
            section_ = line.substr(1, line.size() - 2);
            trim(section_);
            continue;
        }

        // 处理 key = value
        auto eq = line.find('=');
        if (eq == std::string::npos)
            continue;
        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);
        trim(key);
        trim(val);
        if (!section_.empty())
        {
            key = section_ + "." + key;
        }
        kv_[key] = val;
    }

    return true;
}

// —— 以下对 fromString 在类外做全特化 —— //

template <>
int ConfigMgr::fromString<int>(const std::string &s)
{
    return std::stoi(s);
}

template <>
long ConfigMgr::fromString<long>(const std::string &s)
{
    return std::stol(s);
}

template <>
double ConfigMgr::fromString<double>(const std::string &s)
{
    return std::stod(s);
}

template <>
bool ConfigMgr::fromString<bool>(const std::string &s)
{
    std::string t = s;
    std::transform(t.begin(), t.end(), t.begin(),
                   [](unsigned char c)
                   { return std::tolower(c); });
    return (t == "1" || t == "true" || t == "yes");
}

template <>
std::string ConfigMgr::fromString<std::string>(const std::string &s)
{
    return s;
}

// 注意：因为我们只对这几种类型做了特化，其他类型如果调用会链接错误
