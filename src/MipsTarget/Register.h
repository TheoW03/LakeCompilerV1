#include <iostream>

using namespace std;

#ifndef REGISTERS_H
#define REGISTERS_H
struct Registers
{
    int array[10];
};
#endif

void saveRegister(int *array, string &global_string);
string allocateRegister(int *array, int weight);
void resetRegisters(int *array);
void bringSaveback(int *array, string &global_string);