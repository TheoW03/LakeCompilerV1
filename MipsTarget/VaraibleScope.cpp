// this will be designed to hold Varaible util functions and scope
#include <iostream>
#include <string>
#include "../compilerFrontend/parser.h"
#include "../compilerFrontend/Lexxer.h"
#include "../MipsTarget/UtilFunctions.h"

using namespace std;

struct Varaible
{
    Tokens *varType;
    int stackNum;
    int constant;
};

struct Scope_dimension
{
    map<string, Varaible *> vars;
    int stack_allocation;
};

int nextRegister = -1;
int nextArgRegister = -1;
string allocateReg()
{
    if (nextRegister >= 9)
    {
        nextRegister = -1;
    }
    nextRegister++;
    return "$t" + to_string(nextRegister);
}
void freeReg()
{
    nextRegister--;
    if (nextRegister < 0)
    {
        nextRegister = 0;
    }
}
string allocate_argumentRegister()
{
    if (nextArgRegister >= 9)
    {
        nextArgRegister = -1;
    }
    nextArgRegister++;
    return "$a" + to_string(nextArgRegister);
}
void reset_arg_register()
{
    nextArgRegister = -1;
}
void allocate_Scope(vector<Scope_dimension *> &scope)
{
    Scope_dimension *a = new Scope_dimension;
    map<string, Varaible *> b;
    a->vars = b;
    scope.push_back(a);
}
void deallocate_Scope(VaraibleReference *var, vector<Scope_dimension *> &scope)
{
    scope.erase(scope.end());
}
Varaible *get_varaible(VaraibleReference *var, vector<Scope_dimension *> &scope)
{
    for (int i = 0; i < scope.size(); i++)
    {
        map<string, Varaible *> b = scope[i]->vars;
        if (b.find(var->varailbe->buffer) != b.end())
        {
            return b[var->varailbe->buffer];
        }
    }
    return nullptr;
}