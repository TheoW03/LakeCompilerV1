#include <iostream>
#include "../MipsTarget/builtInFunction.h"
#include "../compilerFrontend/parser.h"
#include "../compilerFrontend/Lexxer.h"

using namespace std;

#pragma region print

Print::Print(funcCallNode *function)
{
    Print::function = function;
}

void Print::execute_code(string &gen_string)
{
}

#pragma endregion
