#include "../include/utility.hpp"

namespace mylib
{
    string localString(const string &format)
    {
        char buf[128];
        time_t now = time(nullptr);
        tm *tm_now = localtime(&now);
        strftime(buf, 128, format.data(), tm_now);
        return string(buf);
    }

    vector<string> splitString(const string &str, const string &delim)
    {
        vector<string> res;
        if (str.empty())
            return res;

        char *s = new char[str.length() + 1];
        strcpy(s, str.c_str());

        char *d = new char[delim.length() + 1];
        strcpy(d, delim.c_str());

        char *p = strtok(s, d);
        while (p)
        {
            string s = p;
            res.push_back(s);
            p = strtok(NULL, d);
        }

        return res;
    }

    bool isSpace(char ch) { return ch == ' ' || ch == '\n' || ch == '\r'; }

    string trim(const string &str)
    {
        if (str.empty())
            return str;
        int left_index = 0, right_index = str.size() - 1;
        while (isSpace(str[left_index]))
            left_index += 1;
        while (isSpace(str[right_index]))
            right_index -= 1;
        return str.substr(left_index, right_index - left_index + 1);
    }

    string unicodeToString(char32_t ch)
    {
        u32string unicode_str;
        unicode_str += ch;
        return wstring_convert<codecvt_utf8<char32_t>, char32_t>{}.to_bytes(unicode_str);
    }

    size_t findFirstOf(const string &str, size_t begin, char ch)
    {
        size_t size = str.size(), cur_index = begin;
        while (cur_index < size)
        {
            if (str[cur_index] == ch)
                return cur_index;
            cur_index += 1;
        }
        return string::npos;
    }

    string toLower(const string &str)
    {
        string ret;
        for (char ch : str)
            ret += std::tolower(ch);
        return ret;
    }

    string toUpper(const string &str)
    {
        string ret;
        for (char ch : str)
            ret += std::toupper(ch);
        return ret;
    }

}
