
#include <iostream>
#include <string>
#include <vector>
#include "MipsTarget/MipsTarget.h"
#include "ARMTarget/ARMTarget.h"
#include "compilerFrontend/Lexxer.h"
#include "compilerFrontend/parser.h" // #define String string

using namespace std;

template <typename Base, typename T>
bool instanceof (T * ptr)
{
    return dynamic_cast<Base>(ptr);
}


struct Data
{
    int x, y;
};
int main()
{
    // int scaledValue = (12.3 * (double)255);
    // cout << scaledValue << endl;
    // int integerPart = scaledValue >> 16;

    // // Extract the fractional part as an integer
    // int fractionalPart = scaledValue & 0xFFFF;

    // // Calculate the fractional part as an integer value
    // long integerFraction = (fractionalPart * 10000) / 65536;

    // cout << integerPart;
    // cout << ".";
    // cout << integerFraction << endl;
    // std::cout << integerPart << ".";
    vector<Tokens> a = lex(readFile("test.txt"));
    cout << "lexxed" << endl;
    printList(a);
    Node *c = functionParse(a);
    cout << "parsed" << endl;
    if (c == nullptr)
    {
        cout << "nullptr out \n";
        return 1;
    }
    gen_mips_target(c);
    cout << "compiled \n";
    return 0;
}
