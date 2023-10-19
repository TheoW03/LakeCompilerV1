#include <iostream>
#include <vector>
using namespace std;

#ifndef REGISTER_ALLOCATION_H
#define REGISTER_ALLOCATION_H
class RegisterAllocation
{
public:
    RegisterAllocation();                               // Constructor declaration
    vector<int> allocated_registers;                    // filled with 1 or 0 to reprsent registers
    int register_number;                                // allocated register
    void reset_registers();                             // sets allocated register to 0 and save
    void send_save(string &global);                     // saves registers on stack
    void return_save(string &global);                   // returns save
    string allocate_register(int is_important);         // returns $t0-$t9 and sets the id to 1 if important
    void downgrade_register(string id); // downgrade register to 0
};
#endif