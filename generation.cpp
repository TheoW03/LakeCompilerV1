#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>

#include "parser.h"
#include "Lexxer.h"

using namespace std;
template <typename Base, typename T>

inline bool instanceof (T * ptr)
{
    cout << "T" << endl;

    cout << typeid(T).name() << endl;
    cout << "base" << endl;
    cout << typeid(Base).name() << endl;
    return (dynamic_cast<const Base *>(ptr) != nullptr);
}

Node::~Node()
{
    // implementation of the destructor goes here
}
struct VaraibleNode{
    Node* val;
    string reg;
};
string tabs_str(vector<string> &tabs)
{
    string t = "";
    for (int i = 0; i < tabs.size(); i++)
    {
        t += tabs[i];
    }
    return t;
}
void addtabs(vector<string> &tabs)
{
    tabs.push_back("\t");
}
void remove(vector<string> &tabs)
{
    tabs.pop_back();
}
int nextRegister = -1;
string allocateReg()
{
    if (nextRegister >= 9)
    {
        nextRegister = -1;
    }
    nextRegister++;
    return "$t" + to_string(nextRegister);
}
string global_string = "";
void freeReg()
{
    nextRegister--;
}


float interptObjs(Node* op){
    return 0.0f;
}
string gen_opertors(Node *op, vector<string> &tabs)
{

    if (op == nullptr)
    {
        return "";
    }
    // mem errro :')
    NumNode *pd;
    if ((pd = dynamic_cast<NumNode *>(op)) != nullptr)
    {
        string reg = allocateReg();
        global_string += tabs_str(tabs) + "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    if (dynamic_cast<operatorNode *>(op) != nullptr)
    {
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t = pd->token->id;
        string resultReg = allocateReg();
        if (t == type::ADDITION)
        {
            // return
            string left = gen_opertors(op->left, tabs);
            string right = gen_opertors(op->right, tabs);
            global_string += tabs_str(tabs) + "add " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            // return
            string left = gen_opertors(op->left, tabs);
            string right = gen_opertors(op->right, tabs);
            global_string += tabs_str(tabs) + "sub " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MULTIPLY)
        {
            // return
            string left = gen_opertors(op->left, tabs);
            string right = gen_opertors(op->right, tabs);
            global_string += tabs_str(tabs) + "mult " + left + ", " + right + " \n";
            global_string += tabs_str(tabs) + "mflo " + resultReg + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::DIVISION)
        {
            // return
            string left = gen_opertors(op->left, tabs);
            string right = gen_opertors(op->right, tabs);
            global_string += tabs_str(tabs) + "div " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MOD)
        {
            string left = gen_opertors(op->left, tabs);
            string right = gen_opertors(op->right, tabs);
            global_string += tabs_str(tabs) + "div " + resultReg + "," + left + ", " + right + " \n";
            global_string += tabs_str(tabs) + "mfhi " + resultReg + "\n";
            freeReg();
            freeReg();
            return resultReg;
            //           div $t4,$v0, $t3
            // div $t4, $t3, 10
            // mfhi $t3
        }
    }
    // treat like register machine
    return "";
}
void print_global()
{
    cout << global_string << endl;
}
void traverse(Node *node)
{
    if (node == nullptr)
    {
        return;
    }
    NumNode *pd;
    if ((pd = dynamic_cast<NumNode *>(node)) != nullptr)
    {
        NumNode *a = static_cast<NumNode *>(node);
        cout << a->num << endl;
    }
    else
    {
        cout << "null" << endl;
    }

    traverse(node->left);
    traverse(node->right);

    // traverse(node->right);
}

void wf(ofstream &outfile, string word)
{
    outfile << word << endl;
}

void gencode(Node *op, string filename = "")
{
    if (filename == "")
    {
        filename = "out.s";
    }
    ofstream outfile(filename);
    // FILE* fp = fopen("output.s", "w");
    FunctionNode* pd = dynamic_cast<FunctionNode*>(op);
    string word = ".data \n .text \n "+pd->nameOfFunction->buffer+"\n";
    wf(outfile, word);
    
    // vector<string> tab;
    // addtabs(tab);

    // string reg_result = gen_opertors(op, tab);
    // cout << reg_result;
    // wf(outfile, global_string);
    // string printConsole = tabs_str(tab) + "li $v0, 1 \n" + tabs_str(tab) + "move $a0," + reg_result + "\n" + tabs_str(tab) + "syscall # prints to console\n";
    // wf(outfile, printConsole);
    // write everything in
    // string exitStuff = tabs_str(tab) + "li $v0, 10 \n" + tabs_str(tab) + "syscall # exited program pop into QtSpim and it should work";
    // wf(outfile, exitStuff);
    outfile.close();
}

// string gen_print()
// {
//     return "";
// }
