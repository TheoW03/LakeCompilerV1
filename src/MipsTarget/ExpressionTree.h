#include <iostream>
#include <string>
#include <map>

#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"

#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/UtilFunctions.h"

using namespace std;
string gen_string(Node *op, Scope_Monitor &scope_monitor, string &global_string);
int gen_integer_op(Node *op, Scope_Monitor &scope_monitor, string &global_string, string &register_result);
float gen_float_op(Node *op, Scope_Monitor &scope_monitor, string &global_string, string &register_result);
string gen_char_op(Node *op, Scope_Monitor &scope_monitor, string &global_string);

void increase_numofbranch();
int getnOfBranch();
#pragma once
string handle_boolean(Node *op, Scope_Monitor &scope_monitor, string &global_string, int isLoop = 0);
void handle_function_calls(vector<VaraibleDeclaration *> function_params, vector<Node *> params, Scope_Monitor &scope_monitor, string &global_string);
void update_var_values(Tokens type, Node *expression, string &global_string, string &reg, Scope_Monitor &scope_monitor);