#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <vector>
#include <map>

#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/MipsTarget/UtilFunctions.h"
#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/ExpressionTree.h"
using namespace std;

#pragma once
void gen_mips_target(vector<FunctionNode *> op, string filename = "");
