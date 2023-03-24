
#include <iostream>
#include <string>
#include "tools.h"
#include <vector>

// #define String string

using namespace std;

int main()
{
    string str1 = "test";
    string str2 = " string cat \n";
    vector<Tokens> a = lex(readFile("test"));
    printList(a);
    return 0;
}
