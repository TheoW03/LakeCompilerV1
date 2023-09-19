#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <typeinfo>

#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"
#include "../compilerFrontend/optimizations.h"

using namespace std;

#ifndef INSTANCEOF_H
#define INSTANCEOF_H
template <typename Base, typename T>
bool instanceof (T * ptr);
#endif

#ifndef CAST_TO_H
#define CAST_TO_H
template <typename Base, typename T>
T *cast_to(T *ptr);
#endif

#ifndef VARAIBLE_H
#define VARAIBLE_H
struct Varaible
{
    Tokens *varType;
    int stackNum;
    int constant;
};
#endif

void traverse(Node *node);
void gen_mips_target(Node *op, string filename = "");
string gen_opertors(Node *op, map<string, int> &map);
string gen_integer_op(Node *op, map<string, Varaible *> &map);
string gen_float_op(Node *op, map<string, Varaible *> &map);
string allocateReg();
void freeReg();
void print_global();
float interptObjs(Node *op);
void prepare_interptMips(VaraibleDeclaration *var, map<string, Varaible *> &map, int size);
void gen_function(FunctionNode *function, map<string, int> &map);
// void gen_x86_target(Node *op, string filename = "");