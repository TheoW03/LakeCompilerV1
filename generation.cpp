#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>

#include "parser.h"
#include "tools.h"
using namespace std;
template <typename Base, typename T>

inline bool instanceof (T *ptr)
{
    cout << "T" << endl;

    cout << typeid(T).name() << endl;
    cout << "base" << endl;
    cout << typeid(Base).name() << endl;
    return (dynamic_cast<const Base*>(ptr) != nullptr);
}
Node::~Node()
{
    // implementation of the destructor goes here
}
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
        type t = pd->token->id;
        if (t == type::ADDITION)
        {
            // return
            return "";
        }
    }
    return "";
}
void traverse(Node *node)
{
    if (node == nullptr)
    {
        return;
    }
    if (dynamic_cast<operatorNode*>(node)!=nullptr)
    {
        // operatorNode* n = dynamic_cast<operatorNode*>(node);
        // Tokens* a = n->token;
        // cout << a->buffer;
        // cout << "op \n";
    }
    if (dynamic_cast<NumNode*>(node) != nullptr)
    {

        // cout << "num \n";
    }
    traverse(node->left);
    traverse(node->right);

    // traverse(node->right);
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
