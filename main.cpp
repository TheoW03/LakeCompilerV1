
#include <iostream>
#include <string>
#include <vector>
#include "MipsTarget/MipsTarget.h"
#include "ARMTarget/ARMTarget.h"
#include "compilerFrontend/Lexxer.h"
#include "compilerFrontend/parser.h" // #define String string
#include <cmath>
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
    // int a = 3000;
    // int *ptr;

    // *ptr = *ptr + 1;


    // int**** ppptr = &a;
    // int *ptr = &ppptr;
    // int scaledValue = (12.3 * (double)OFFSET);
    // cout << scaledValue << endl;
    // int integerPart = scaledValue / OFFSET;

    // // Extract the fractional part as an integer
    // scaledValue = abs(scaledValue);
    // cout << scaledValue << endl;
    // // scaledValue *= -1; //abs
    // int fractionalPart = scaledValue & 0xFFFF;

    // // Calculate the fractional part as an integer value
    // long integerFraction = (fractionalPart * 10000) / 0xFFFF;

    // cout << integerPart;
    // cout << ".";
    // cout << integerFraction << endl;
    // // exit(0);
    // std::cout << integerPart << ".";
    vector<Tokens> a = lex(readFile("test.txt"));
    printList(a);
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
