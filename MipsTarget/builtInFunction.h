#include "../compilerFrontend/parser.h"
#include <iostream>
#include <string.h>
#include "../compilerFrontend/Lexxer.h"

using namespace std;
#ifndef builtInFunction
#define builtInFunction
class builtInFunction{
    
};
#endif  

#ifndef PRINT_H
#define PRINT_H
class Print
{
private:
    funcCallNode *function;

public:
    Print(funcCallNode *function);
    void execute_code(string &gen_string);
};
#endif