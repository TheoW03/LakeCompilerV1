#include "../compilerFrontend/parser.h"
#include <iostream>
#include <string.h>
#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"
#include "../MipsTarget/MipsTarget.h"
using namespace std;

#ifndef BUILT_IN_H
#define BUILT_IN_H
class builtInFunction
{
public:
    funcCallNode *function;
    virtual void execute_code_integer(string &gen_string, string registers);
    virtual void execute_code_float(string &gen_string, string registers);
};

#endif

#ifndef PRINT_H
#define PRINT_H
class Print : public builtInFunction
{
public:
    Print();
    void execute_code_integer(string &gen_string, string registers);
    void execute_code_float(string &gen_string, string registers);
};
#endif

#ifndef Exit_H
#define Exit_H
class Exit : public builtInFunction
{
public:
    Exit();
    void execute_code_integer(string &gen_string, string registers);
    void execute_code_float(string &gen_string, string registers);
};
#endif