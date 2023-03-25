
#include <iostream>
#include <string>
#include <vector>
#include "parser.h"
// #define String string

using namespace std;

int main()
{
    string str1 = "test";
    string str2 = " string cat \n";
    vector<Tokens> a = lex(readFile("test"));
    printList(a);
    Node* eq = parse(a);
    return 0;
}
