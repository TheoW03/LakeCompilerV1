#include <iostream>
#include <string>
using namespace std;

int nextRegisters = 0;

struct Registers
{
    int array[10];
};
struct AllocatedRegister
{
    string allocated_register;
    int weight;
};
void resetRegisters(int *array)
{
    for (int i = 0; i < 10; i++)
    {
        array[i] = 0;
    }
    nextRegisters = 0;
}
string allocateRegister(int *array, int weight)
{
    if (nextRegisters >= 9)
    {
        nextRegisters = -1;
    }

    nextRegisters++;
    array[nextRegisters] = weight;
    return "$t" + to_string(nextRegisters);
}
void saveRegister(int *array, string &global_string)
{
    for (int i = 0; i < sizeof(array); i++)
    {
        if (array[i] == 2)
        {
            global_string += "move $s" + to_string(i) + ", $t" + to_string(i) + "\n";
        }
    }
}
void bringSaveback(int *array, string &global_string)
{
    for (int i = 0; i < sizeof(array); i++)
    {
        if (array[i] == 2)
        {
            global_string += "move $t" + to_string(i) + ", $s" + to_string(i) + "\n";
        }
    }
}