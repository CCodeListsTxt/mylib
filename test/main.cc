#include <iostream>
#include <string>

using namespace std;

int main()
{
    char p[] = "hello";
    string str(p);
    cout << str << endl;
    p[0] = 'a';
    cout << str <<endl;

    return 0;
}