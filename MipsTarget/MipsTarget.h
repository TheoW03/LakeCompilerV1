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
inline bool instanceof (const T *ptr);
#endif

#ifndef CAST_TO_H
#define CAST_TO_H
template <typename Base, typename T>
T* cast_to(T* ptr);
#endif

#ifndef VARAIBLENODE_H
#define VARAIBLENODE_H
struct VaraibleNode
{
    Node *val;
    string reg;
};
#endif

void traverse(Node *node);
void gen_mips_target(Node *op, string filename = "");
string gen_opertors(Node *op, vector<string> &tabs, map<string, int> &map);
string allocateReg();
void print_global();
float interptObjs(Node* op);
void prepare_interptMips(varaibleNode* var, map<string, int> &map);
void gen_function(FunctionNode *function, map<string, int> &map);
// void gen_x86_target(Node *op, string filename = "");