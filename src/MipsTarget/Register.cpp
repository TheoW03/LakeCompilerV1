#include <iostream>
#include <string>
#include "../../src/MipsTarget/Register.h"
using namespace std;

RegisterAllocation::RegisterAllocation()
{
    for (int i = 0; i < 10; i++)
    {
        allocated_registers.push_back(0);
    }
    register_number = -1;
}
void RegisterAllocation::reset_registers()
{
    for (int i = 0; i < 10; i++)
    {
        allocated_registers[i] = 0;
    }
    register_number = -1;
}
void RegisterAllocation::send_save(string &global)
{
}
string RegisterAllocation::allocate_register(int is_important)
{
    if (register_number >= 9)
    {
        register_number = -1;
    }
    register_number++;
    if (is_important == 1)
    {
        allocated_registers[register_number] = 1;
    }
    return "$t" + to_string(register_number);
} // returns $t0-$t9 and sets the id to 1 if important
