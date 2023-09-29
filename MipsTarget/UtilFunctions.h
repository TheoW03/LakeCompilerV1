#include <iostream>
#include <string>
#include <fstream>
#include <typeinfo>
#include <vector>

#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"
#include "../compilerFrontend/optimizations.h"

using namespace std;

#ifndef CAST_TO_H
#define CAST_TO_H
template <typename Base, typename T>
bool instanceof1(T *ptr);
#endif



// #pragma once
#ifndef TEMPLATEFUNCTION_H
#define TEMPLATEFUNCTION_H
template <typename Base, typename T>
bool instanceof (T * ptr)
{
    return dynamic_cast<Base>(ptr);
}
#endif