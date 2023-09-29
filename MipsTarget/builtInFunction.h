#include <iostream>
#include <string>
#include <vector>

#include "../compilerFrontend/parser.h"
#include "../compilerFrontend/Lexxer.h"
#include "../MipsTarget/UtilFunctions.h"
#include "../compilerFrontend/optimizations.h"
#include "../MipsTarget/MipsTarget.h"
#include "../MipsTarget/VaraibleScope.h"

using namespace std;

#ifndef BUILT_IN_H
#define BUILT_IN_H
class builtInFunction
{
public:
    funcCallNode *function;
    virtual void execute_code_integer(string &gen_string, string registers);
    virtual void execute_code_float(string &gen_string, string registers);
    virtual void setup_params(vector<Node *> params, string &gen_string, map<string, Varaible *> map);
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
    void setup_params(vector<Node *> params, string &gen_string, map<string, Varaible *> map);
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
    void setup_params(vector<Node *> params, string &gen_string, map<string, Varaible *> map);
};
#endif