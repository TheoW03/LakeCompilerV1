
#include <iostream>
#include <string>
#include <vector>
#include "MipsTarget/MipsTarget.h"
#include "x86TargetFiles/x86Target.h"
#include "compilerFrontend/Lexxer.h"
#include "compilerFrontend/parser.h"// #define String string

using namespace std;
struct Data
{
    int x, y;
};
int main()
{
    vector<Tokens> a = lex(readFile("test.txt"));
    // // cout << "lexxed" << endl;
    printList(a);
    Node *c = functionParse(a);
    cout << "parsed" << endl;
    if (c == nullptr)
    {
        cout << "nullptr out \n";
        return 1;
    }
    gen_mips_target(c);
    cout << "should be compiled \n";
    return 0;
}
