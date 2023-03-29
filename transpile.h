#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <typeinfo>

#include "tools.h"
#include "parser.h"
using namespace std;

#ifndef INSTANCEOF_H
#define INSTANCEOF_H
template<typename Base, typename T>
inline bool instanceof(const T *ptr);
#endif

void traverse(Node *node);
void gencode(string filename="");
string gen_opertors(Node *op, ofstream& of);
string allocateReg();
