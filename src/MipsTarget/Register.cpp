#include <iostream>
#include <string>
#include <map>
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
    cout << allocated_registers.size() << endl;
    cout << register_number << endl;
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
    cout << "id: " << id << endl;
    if (id == "")
    {
        return;
    }
    map<char, int> charToNumberMap;
    // Insert values into the map
    charToNumberMap['0'] = 0;
    charToNumberMap['1'] = 1;
    charToNumberMap['2'] = 2;
    charToNumberMap['3'] = 3;
    charToNumberMap['4'] = 4;
    charToNumberMap['5'] = 5;
    charToNumberMap['6'] = 6;
    charToNumberMap['7'] = 7;
    charToNumberMap['8'] = 8;
    charToNumberMap['9'] = 9;
    // string loc =
    int location = charToNumberMap[id[2]];
    // int location = stoi(to_string(id[2]));
    // cout << id[2] << endl;
    // cout << loc << endl;
    allocated_registers[location] = 0;
}