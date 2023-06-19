
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

void handle_cli(char *argv[], int argc)
{
    if (argc == 1)
    {
        return;
    }
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
        {
            vector<Tokens> a = lex(readFile("test.txt"));
            printList(a);
            cout << "lexxed" << endl;
            printList(a);
            Node *c = functionParse(a);
            cout << "parsed" << endl;
            if (c == nullptr)
            {
                cout << "nullptr out \n";
                return;
            }
            gen_mips_target(c);
            cout << "demo compiled \n";
            return;
        }
    }
    char *file = argv[1];

    vector<Tokens> a = lex(readFile(file));

    for (int i = 0; i < argc; i++)
    {

        if (strcmp(argv[i], "-token") == 0)
        {
            printList(a);
        }
    }
    Node *c = functionParse(a);
    string file1 = "";
    for (int i = 0; i < argc; i++)
    {

        if (strcmp(argv[i], "-o") == 0)
        {
            file1 = argv[i + 1];
        }
    }

    gen_mips_target(c, file1);
    cout << "compiled sucessfully" << endl;
}
int main(int argc, char *argv[])
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
    handle_cli(argv, argc);

    // vector<Tokens> a = lex(readFile("test.txt"));
    // printList(a);
    // cout << "lexxed" << endl;
    // printList(a);
    // Node *c = functionParse(a);
    // cout << "parsed" << endl;
    // if (c == nullptr)
    // {
    //     cout << "nullptr out \n";
    //     return 1;
    // }
    // gen_mips_target(c);
    // cout << "compiled \n";
    return 0;
}
