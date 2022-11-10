// #include "../include/log.hpp"
// #include "../include/json.hpp"
#include <iostream>
#include <map>
#include <string>

// using namespace mylib;
using namespace std;

string func()
{
    map<string, string> k;
    try
    {
        return k.at("wjdqil");
    }
    catch (...)
    {
        cout << "hello";
        return "hello";
    }
}

int main()
{
    map<string, string> k;
    func();

    return 0;
}
