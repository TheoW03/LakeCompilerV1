#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>

#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/MipsTarget/UtilFunctions.h"

using namespace std;


// template <typename Base, typename T>
// Base *cast_to(T *ptr)
// {
//     // return dynamic_cast<Base>(ptr);
// }
