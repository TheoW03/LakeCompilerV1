#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <typeinfo>

#include "Lexxer.h"
#include "parser.h"
using namespace std;

#ifndef INSTANCEOF_H
#define INSTANCEOF_H
template <typename Base, typename T>
inline bool instanceof (const T *ptr);
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
void gencode(Node *op, string filename = "");
string gen_opertors(Node *op);
string allocateReg();
void print_global();
float interptObjs(Node* op);