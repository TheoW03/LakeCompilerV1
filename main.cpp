
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
    // gen_LLVM(functions, "");
    handle_cli(argv, argc);
    return 0;
}
