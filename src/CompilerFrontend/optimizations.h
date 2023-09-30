
#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"


using namespace std;

int check_if_pureExpression(Node *op);

int constant_prop_integer(Node *op);
float constant_prop_float(Node *op);
int constant_prop_boolean(Node *op);