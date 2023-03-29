#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include "tools.h"
#include "parser.h"
using namespace std;
void traverse(Node *node);
void gencode(string filename="");
string gen_opertors(Node *op, ofstream& of);
string allocateReg();
