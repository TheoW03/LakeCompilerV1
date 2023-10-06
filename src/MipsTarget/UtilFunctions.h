#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo>
#include <vector>

#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"

using namespace std;

// #pragma once
#ifndef TEMPLATEFUNCTION_H
#define TEMPLATEFUNCTION_H
template <typename Base, typename T>
bool instanceof (T * ptr)
{
    return dynamic_cast<Base>(ptr);
}
#endif