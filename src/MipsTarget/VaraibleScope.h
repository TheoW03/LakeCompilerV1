#include <iostream>
#include <string>
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/MipsTarget/UtilFunctions.h"
using namespace std;

#ifndef VARAIBLE_H
#define VARAIBLE_H
struct Varaible
{
    Tokens *varType;
    int stackNum;
    int constant;
};
#endif

#ifndef SCOPE_DIMENSION_H
#define SCOPE_DIMENSION_H
struct Scope_dimension
{
    map<string, Varaible *> vars;
    int stack_allocation;
};
#endif
#ifndef SCOPE_MONITOR_H
#define SCOPE_MONITOR_H
struct Scope_Monitor
{
    vector<Scope_dimension *> &scope;
    map<string, FunctionNode *> f;
};
#endif
string allocateReg();
void freeReg();
string allocate_argumentRegister();
void reset_arg_register();
void allocate_Scope(vector<Scope_dimension *> &scope);
void deallocate_Scope(vector<Scope_dimension *> &scope);
Varaible *get_varaible(VaraibleReference *var, vector<Scope_dimension *> &scope);
Varaible *add_to_var(VaraibleDeclaration *var, vector<Scope_dimension *> &scope, int stack_number);