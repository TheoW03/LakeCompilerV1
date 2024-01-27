#include <iostream>
#include <string>
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/MipsTarget/UtilFunctions.h"
#include "../../src/MipsTarget/Register.h"

using namespace std;

#ifndef VARAIBLE_H
#define VARAIBLE_H
struct Varaible
{
    Tokens varType;
    int stackNum;
    int constant;
    virtual ~Varaible();
    vector<unique_ptr<Node>> array_dimensions;
};
#endif

#ifndef SCOPE_DIMENSION_H
#define SCOPE_DIMENSION_H
struct Scope_dimension
{
    map<string, Varaible *> vars;
    int stack_allocation;
    Scope_dimension();
};
#endif

// #ifndef ARRAY
// #define ARRAY
// struct Array : Varaible
// {
//     Tokens varType;
// };
// #endif

#ifndef SCOPE_MONITOR_H
#define SCOPE_MONITOR_H
struct Scope_Monitor
{
    vector<Scope_dimension> scope;
    map<string, FunctionNode *> f;
    RegisterAllocation rg;
    // Registers *registers;
};
#endif
string allocateReg();
void freeReg();
string allocate_argumentRegister();
void reset_arg_register();
void allocate_Scope(vector<Scope_dimension> &scope);
void deallocate_Scope(vector<Scope_dimension> &scope);
Varaible *get_varaible(VaraibleReference *var, vector<Scope_dimension> &scope);
Varaible *add_to_var(VaraibleDeclaration *var, vector<Scope_dimension> &scope,
                     int stack_number);
void reset_registers();
int save_registers(string &global_string);
void bring_saveBack(string &global_string, int nextReg);
Varaible *add_to_var_arr(ArrayDeclaration *var,
                         vector<Scope_dimension> &scope,
                         int stack_number);

Varaible *getArr(ArrayRef *var, vector<Scope_dimension> &scope);
