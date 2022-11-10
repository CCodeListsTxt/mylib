#include "../include/http.hpp"
#include <iostream>
using namespace mylib;

const char *request = ""
                      "GET / HTTP/1.1\r\n"
                      "Accept-Encoding: gzip, deflate, br\r\n"
                      "Accept-Language: zh-CN,zh;q=0.9,en;q=0.8\r\n"
                      "Host: baidu.com\r\n"
                      "Sec-Fetch-Dest: document\r\n"
                      "Sec-Fetch-Mode: navigate\r\n"
                      "Sec-Fetch-Site: none\r\n"
                      "Sec-Fetch-User: ?1\r\n"
                      "Upgrade-Insecure-Requests: 1\r\n\r\n";

int main()
{
    HttpRequest re;
    HttpParser parser;
    parser.reset(request);

    parser.parseLine(re);
    parser.parseHeaders(re);
    cout << re.toString();

    return 0;
}