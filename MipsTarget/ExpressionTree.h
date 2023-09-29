#include <iostream>
#include <string>
#include <map>

#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"

#include "../MipsTarget/VaraibleScope.h"
#include "../MipsTarget/UtilFunctions.h"

using namespace std;
string gen_string(Node *op, vector<string> &tabs, map<string, Varaible *> &map, string &global_string);
string gen_integer_op(Node *op, map<string, Varaible *> &map, string &global_string);
string gen_float_op(Node *op, map<string, Varaible *> &map, string &global_string);
void increase_numofbranch();
int getnOfBranch();
#pragma once
string handle_boolean(Node *op, map<string, Varaible *> map, string &global_string, int isLoop = 0);