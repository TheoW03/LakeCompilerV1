
#include <iostream>
#include <string>
#include <vector>
#include "transpile.h"
#include "Lexxer.h"

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
    cout << "lexxed" << endl;
    printList(a);
    // Node *c = parse(a);
    // cout << "parsed" << endl;

    // if (c == nullptr)
    // {
    //     cout << "nullptr out \n";
    // }
    // gencode(c,"test.s");
    return 0;
}
