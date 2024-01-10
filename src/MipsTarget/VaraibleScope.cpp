// this will be designed to hold Varaible util functions and scope
#include <iostream>
#include <string>
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/MipsTarget/UtilFunctions.h"
#include "../../src/MipsTarget/Register.h"

using namespace std;

struct Varaible
{
    Tokens varType;
    int stackNum;
    int constant;
    virtual ~Varaible();
};
struct Array : Varaible
{
    Tokens varType;
};
Varaible::~Varaible()
{
}
struct Scope_dimension
{
    map<string, Varaible *> vars;
    int stack_allocation;
    Scope_dimension()
    {
    }
};
struct Scope_Monitor
{
    vector<unique_ptr<Scope_dimension>> scope;
    map<string, FunctionNode *> f;
    RegisterAllocation rg;
};

int nextRegister = -1;
int nextArgRegister = -1;
int registers_used = -1;

string allocateReg()
{
    if (nextRegister >= 9)
    {
        nextRegister = -1;
    }
    // if (registers_used < 9)
    // {
    //     registers_used++;
    // }
    nextRegister++;
    return "$t" + to_string(nextRegister);
}

int save_registers(string &global_string)
{

    for (int i = 0; i <= nextRegister; i++)
    {
        global_string += "move $s" + to_string(i) + ", " + "$t" + to_string(i) + "\n";
    }
    return nextRegister;
}
void bring_saveBack(string &global_string, int nextReg)
{
    for (int i = 0; i <= nextRegister; i++)
    {
        global_string += "move $t" + to_string(i) + ", " + "$s" + to_string(i) + "\n";
    }
}
void reset_registers()
{
    nextRegister = -1;
    registers_used = 0;
}
void freeReg()
{
    nextRegister--;
    if (nextRegister < 0)
    {
        nextRegister = 0;
        registers_used = 0;
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
void allocate_Scope(vector<Scope_dimension> &scope)
{

    // Scope_dimension *a = new Scope_dimension;
    // unique_ptr<Scope_dimension> a = make_unique<Scope_dimension>();
    Scope_dimension a;
    map<string, Varaible *> b;
    a.vars = b;
    scope.push_back(a);
}
void deallocate_Scope(vector<Scope_dimension> &scope)
{
    scope.pop_back();
}
Varaible *get_varaible(VaraibleReference *var, vector<Scope_dimension> &scope)
{
    for (int i = 0; i < scope.size(); i++)
    {
        map<string, Varaible *> b = scope[i].vars;

        if (b.find(var->varaible.buffer) != b.end())
        {

            return b[var->varaible.buffer];
        }
    }
    return nullptr;
}
Varaible *add_to_var(VaraibleDeclaration *var,
                     vector<Scope_dimension> &scope,
                     int stack_number)
{
    Varaible *a = new Varaible;

    a->constant = var->constant;
    a->stackNum = stack_number;
    a->varType = var->typeOfVar;
    for (int i = 0; i < scope.size(); i++)
    {
        map<string, Varaible *> b = scope[i].vars;
        if (b.find(var->varaible.buffer) != b.end())
        {

            cerr << var->varaible.buffer << " has been declared twice" << endl;
            exit(1);
            return nullptr;
        }
    }

    scope[scope.size() - 1].vars[var->varaible.buffer] = a;
    return a;
}
