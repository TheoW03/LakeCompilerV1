#include <iostream>
#include <string>
#include "parser.h"

using namespace std;


int nextRegister = 0;
string registers[] = {"$t0", "$t1", "$t2", "$t3"};

string gen_opertors(Node *op)
{
    NullStruct NullStruct1;
    NumNode numNode;
    operatorNode opNode;

    if (typeid(NullStruct1) == typeid(op))
    {
        return "";
    }
    if (typeid(op) == typeid(numNode))
    {
        NumNode *pd = static_cast<NumNode *>(op); // downcast
        return allocateReg();
    }
    gen_opertors(op->left);
    gen_opertors(op->right);
    if (typeid(op) == typeid(opNode))
    {
        operatorNode *pd = static_cast<operatorNode *>(op); // downcast
        type t = pd->token.id;
        if (t == type::ADDITION)
        {
            //return 
            return "";
        }
    }
    return "";
}

string allocateReg(){
    return registers[nextRegister++];
}
// string gen_print()
// {
//     return "";
// }
