
#include <iostream>
#include <string>
#include <vector>
#include "src/MipsTarget/MipsTarget.h"
#include "src/CompilerFrontend/Lexxer.h"
#include "src/CompilerFrontend/parser.h" // #define String string
#include <cmath>
#include <functional>
#include "src/LLVMBackend/LLVMTarget.h"
using namespace std;

#pragma endregion

void handle_cli(char *argv[], int argc)
{
    if (argc == 1)
    {
        return;
    }
    // for (int i = 0; i < argc; i++)
    // {
    //     if (strcmp(argv[i], "-d") == 0)
    //     {
    //         vector<Tokens> a = lex(readFile("test.lk"));
    //         cout << "lexxed" << endl;
    //         vector<FunctionNode *> c = functionParse(a);
    //         cout << "parsed" << endl;
    //         if (c.size() == 0)
    //         {
    //             cout << "nullptr out \n";
    //             return;
    //         }
    //         gen_mips_target(c);
    //         cout << "demo compiled \n";
    //         return;
    //     }
    // }
    char *file = argv[1];

    vector<Tokens> a = lex(readFile(file));
    cout << "lexxed sucessfully" << endl;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-token") == 0)
        {
            // cout << strcmp(argv[i], "-token") << endl;

            // cout << "print" << endl;
            printList(a);
        }
    }

    vector<FunctionNode *> c = functionParse(a);
    cout << "parsed successfully" << endl;
    string file1 = "";
    for (int i = 0; i < argc; i++)
    {

        if (strcmp(argv[i], "-o") == 0)
        {
            file1 = argv[i + 1];
        }
    }
    gen_mips_target(c, file1);

    cout << "compiled successfully" << endl;
}
int main(int argc, char *argv[])
{
    char *file = "test.lk";
    vector<Tokens> a = lex(readFile(file));
    printList(a);
    vector<FunctionNode *> functions = functionParse(a);
    gen_mips_target(functions);
    // gen_LLVM(functions, "");

    // cout << "parsed " << functions[0]->statements.size() << endl;

    // handle_cli(argv, argc);
    // int array[10]; // Simulate an array or data structure
    // Load a value from the array

    // int value = 33;
    // Decrement the value by 1
    // value--;

    // Store the decremented value back into the array
    // array[2] = value;

    // Perform bitwise operations
    // int value1 = value >> 27;
    // int value2 = 0 << 5;
    // int result1 = value1 | value2;

    // int value3 = value;
    // int value4 = value >> 27;
    // int value5 = 0 << 5;
    // int result2 = (value3 & ~((1 << 27) - 1)) | (value4 << 5) | value5;

    // // Stack manipulation
    // int stack[100];
    // int stack_pointer = 0;

    // // Push data onto the stack
    // stack[stack_pointer++] = result1;

    // // Pop data from the stack
    // int popped_value = stack[--stack_pointer];

    // // Output the results
    // std::cout << "Result1: " << result1 << std::endl;
    // std::cout << "Result2: " << result2 << std::endl;

    // return 0;
    // vector<Tokens> a = lex(readFile("test.lk"));
    // Node *c = testExpressionParse(a);
    // rewrite_vars(c);
    // cout << "compiled" << endl;
    // printList(a);
    // vector<FunctionNode *> b = functionParse(a);
    // gen_mips_target(b);
    //     cout << "compiled sucessfully" << endl;

    // vector<FunctionNode *> c = functionParse(a);
    // cout << c.size() << endl;

    // Print the final result
    // std::cout << "Result: " << result.value << std::endl; // Output: Result: 12

    return 0;
}
