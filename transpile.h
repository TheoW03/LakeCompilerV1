#include <iostream>
#include <string>
#include "tools.h"
#include "parser.h"
using namespace std;

void gencode(string filename="");
string gen_opertors(Node *op, ofstream& of);
string allocateReg();
