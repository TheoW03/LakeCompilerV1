#include <iostream>
#include <string>
#include <vector>
#include "src/MipsTarget/MipsTarget.h"
#include "src/CompilerFrontend/Lexxer.h"
#include "src/CompilerFrontend/parser.h"
#include <cmath>
#include <functional>
#include "src/LLVMBackend/LLVMTarget.h"
#include "src/MipsTarget/ExpirementalTypeChecking.h"
using namespace std;



#pragma endregion

void handle_cli(char *argv[], int argc)
{
    if (argc == 1)
    {
        return;
    }
    char *file = argv[1];

    vector<Tokens> a = lex(readFile(file));
    cout << "lexxed sucessfully" << endl;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-token") == 0)
        {
            printList(a);
        }
    }

    vector<unique_ptr<FunctionNode>> c = move(parse(a));

    cout << "parsed successfully" << endl;
    string file1 = "";
    for (int i = 0; i < argc; i++)
    {

        if (strcmp(argv[i], "-o") == 0)
        {
            file1 = argv[i + 1];
        }
    }
    gen_mips_target(move(c), file1);
	
    cout << "compiled successfully" << endl;
}



int main(int argc, char *argv[])
{
    // gen_LLVM(functions, "");'
    // char *file = argv[1];
    vector<Tokens> a = lex(readFile("test.lk"));

    // // unique_ptr<Node> op = safe_parse(a);
    // // Node op = nullptr
    // // inorderTraversal(op);
    unique_ptr<Node> b = move(testExpressionParse(a));
    int iisCast = 0;
    type_of c = traverse_tree_integer(move(b));
    cout << (c.value) << endl;
    // cout << c << endl;
    int l = ((int)(1.5 + 3.5));
    // int pi = 3.1415;
    cout << l << endl;
    // testExpressionParse(a);
    // handle_cli(argv, argc);
    // float a = (float)1.5 + (int)1.5;
    // cout << a << endl;
    return 0;
}
