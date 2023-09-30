#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <vector>
#include <map>

#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/CompilerFrontend/optimizations.h"
#include "../../src/MipsTarget/UtilFunctions.h"
#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/ExpressionTree.h"
using namespace std;


// #ifndef INSTANCEOF_H
// #define INSTANCEOF_H
// template <typename Base, typename T>
// bool instanceof (T * ptr);
// #endif



// #ifndef VARAIBLE_H
// #define VARAIBLE_H
// struct Varaible
// {
//     Tokens *varType;
//     int stackNum;
//     int constant;
// };
// #endif

// void traverse(Node *node);
// string gen_opertors(Node *op, map<string, int> &map);
// string gen_integer_op(Node *op, map<string, Varaible *> &map);
// string gen_float_op(Node *op, map<string, Varaible *> &map);
// string allocateReg();
// void freeReg();
// void print_global();
// float interptObjs(Node *op);
// void prepare_interptMips(VaraibleDeclaration *var, map<string, Varaible *> &map, int size);
#pragma once
void gen_mips_target(vector<FunctionNode *> op, string filename = "");

// void gen_mips_target(vector<FunctionNode *> op, string filename = "");
// void gen_x86_target(Node *op, string filename = "");
