#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <typeinfo>

#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"
using namespace std;

string allocate_register();
void freeRegister();
string gen_opertorsx86(Node *op, vector<string> &tabs);
void gen_x86_target(Node *op, string filename = "");
void wfx86(ofstream &outfile, string word);
