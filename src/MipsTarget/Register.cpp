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
void RegisterAllocation::return_save(string &global)
{
    int s = 8;
    for (int i = 0; i < 10; i++)
    {
        if (allocated_registers[i] == 1)
        {
            global += "lw $t" + to_string(i) + "," + to_string(s) + "($sp) \n";
        }
        s += 4;
    }
}
void RegisterAllocation::send_save(string &global)
{
    int s = 8;
    for (int i = 0; i < 10; i++)
    {
        if (allocated_registers[i] == 1)
        {
            global += "sw $t" + to_string(i) + "," + to_string(s) + "($sp) \n";
        }
        s += 4;
    }
}
string RegisterAllocation::allocate_register(int is_important)
{
    if (register_number >= 9)
    {
        register_number = -1;
    }
    register_number++;
    allocated_registers[register_number] = is_important;
    return "$t" + to_string(register_number);
} // returns $t0-$t9 and sets the id to 1 if important
void RegisterAllocation::downgrade_register(string id)
{
    int location = stoi(to_string(id[2]));
    allocated_registers[location] = 0;
}