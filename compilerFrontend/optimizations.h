
#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"

using namespace std;

int check_if_pureExpression(Node *op);

int constant_prop_integer(Node *op);
float constant_prop_float(Node *op);
int constant_prop_boolean(Node *op);