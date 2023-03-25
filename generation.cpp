#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include "parser.h"
using namespace std;

int nextRegister = 0;
string registers[] = {"$t0", "$t1", "$t2", "$t3"};
string allocateReg()
{
    return registers[nextRegister++];
}
string gen_opertors(Node *op, ofstream &out)
{
    NullStruct NullStruct1;
    NumNode numNode;
    operatorNode opNode;

    if (op == nullptr)
    {
        return "";
    }
    if (typeid(op) == typeid(numNode))
    {
        NumNode *pd = static_cast<NumNode *>(op); // downcast
        return allocateReg();
    }
    gen_opertors(op->left, out);
    gen_opertors(op->right, out);
    if (typeid(op) == typeid(opNode))
    {
        operatorNode *pd = static_cast<operatorNode *>(op); // downcast
        type t = pd->token.id;
        if (t == type::ADDITION)
        {
            // return
            return "";
        }
    }
    return "";
}

void wf(ofstream &outfile, string word)
{
    outfile << word << endl;
}
void gencode(string filename = "")
{
    if (filename == "")
    {
        filename = "out.s";
    }
    ofstream outfile(filename);
    // FILE* fp = fopen("output.s", "w");
    string word = ".data \n .text \n main: \n";
    wf(outfile, word);
    // write everything in
    string exitStuff = "\t li $v0, 10 \n \t syscall # exited program pop into QtSpim and it should work";
    wf(outfile, exitStuff);
    outfile.close();
}

// string gen_print()
// {
//     return "";
// }
