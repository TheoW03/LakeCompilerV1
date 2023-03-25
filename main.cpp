
#include <iostream>
#include <string>
#include <vector>
#include "transpile.h"

#include "parser.h"
// #define String string

using namespace std;

int main()
{
    vector<Tokens> a = lex(readFile("test"));
    vector<Tokens> b = a;
    Node *eq = parse(a);
    printList(b);
    if (eq == nullptr)
    {
        cout << "nullptr out \n";
    }
    traverse(eq);
    return 0;
}
