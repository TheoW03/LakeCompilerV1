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
string allocateReg()
{
    if(nextRegister > 9){
        nextRegister = 0;
    }
    return "$t"+to_string(nextRegister++);
}
string global_string = "";
string gen_opertors(Node *op)
{

    if (op == nullptr)
    {
        return "";
    }
    if (dynamic_cast<NumNode*>(op) != nullptr)
    {
        NumNode *pd = dynamic_cast<NumNode*>(op);
        string reg = allocateReg();
        global_string += "li "+reg+","+pd->num+"\n";
        return reg;
    }
    if (dynamic_cast<operatorNode*>(op) != nullptr)
    {
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t = pd->token->id;
        string resultReg = allocateReg();
        if (t == type::ADDITION)
        {
            // return
            global_string += "addi "+gen_opertors(op->left)+", "+gen_opertors(op->right)+", "+resultReg+"\n";
            return resultReg;
        }
    }
    //treat like register machine
    return "";
}
void print_global(){
    cout << global_string << endl;
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
