#include <iostream>
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"

#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/UtilFunctions.h"


using namespace std;



#ifndef TYPE_OF_H
#define TYPE_OF_H
struct type_of
{
    int value;
    type type_value;
};
#endif

type_of traverse_tree_integer(unique_ptr<Node> op);




