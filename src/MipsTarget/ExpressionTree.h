#include <iostream>
#include <string>
#include <map>

#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"

#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/UtilFunctions.h"

using namespace std;
string gen_string(Node *op, vector<Scope_dimension *> &scope, string &global_string);
string gen_integer_op(Node *op, vector<Scope_dimension *> &scope, string &global_string);
string gen_float_op(Node *op, vector<Scope_dimension *> &scope, string &global_string);
string gen_char_op(Node *op, vector<Scope_dimension *> &scope, string &global_string);

void increase_numofbranch();
int getnOfBranch();
#pragma once
string handle_boolean(Node *op, vector<Scope_dimension *> &scope, string &global_string, int isLoop = 0);