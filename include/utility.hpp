#pragma once
#include <chrono>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>

namespace mylib
{
    using namespace std;

    class Noncopyable
    {
    protected:
        Noncopyable() = default;
        ~Noncopyable() = default;

    private:
        Noncopyable(const Noncopyable &);
        Noncopyable &operator=(const Noncopyable &);
    };

    // 日期字符串
    string localString(const string &format);
    inline string localTimeString() { return localString("%H:%M:%S"); }
    inline string localDateString() { return localString("%Y-%m-%d"); }
    inline string localDateTimeString() { return localString("%Y-%m-%d %H:%M:%S"); }

    // 分割字符串
    vector<string> splitString(const string &str, const string &delim);
    // 判断是否是空白字符串
    bool isSpace(char ch);
    // 删除字符串两端空白字符
    string trim(const string &str);
}