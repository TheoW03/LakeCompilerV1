#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>

#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"
#include "../MipsTarget/UtilFunctions.h"

using namespace std;


// template <typename Base, typename T>
// Base *cast_to(T *ptr)
// {
//     // return dynamic_cast<Base>(ptr);
// }
