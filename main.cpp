
#include <iostream>
#include <string>
#include <vector>
#include "transpile.h"

#include "parser.h"
// #define String string

using namespace std;
struct Data
{
    int x, y;
};
int main()
{
    vector<Tokens> a = lex(readFile("test"));
    printList(a);
    Node *c = parse(a);

    if (c == nullptr)
    {
        cout << "nullptr out \n";
    }
    gen_opertors(c);
    print_global();
    return 0;
}
