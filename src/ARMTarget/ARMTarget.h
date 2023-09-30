#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <typeinfo>

#include "../../src/compilerFrontend/Lexxer.h"
#include "../../src/compilerFrontend/parser.h"

using namespace std;


string gen_ARM_operators(Node *op);
void wfARM(ofstream &outfile, string word);
void gen_ARM_target(Node *op, string filename = "");
