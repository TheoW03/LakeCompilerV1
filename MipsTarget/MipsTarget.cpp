#include <iostream>
#include <string>
#include <map>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"
#include "../compilerFrontend/optimizations.h"
namespace fs = std::filesystem;

using namespace std;
template <typename Base, typename T>

inline bool instanceof (T * ptr)
{
    cout << "T" << endl;

    cout << typeid(T).name() << endl;
    cout << "base" << endl;
    cout << typeid(Base).name() << endl;
    return (dynamic_cast<Base *>(ptr) != nullptr);
}
template <typename Base, typename T>
T *cast_to(T *ptr)
{
    return (dynamic_cast<Base *>(ptr));
}

int max_size = 0;

Node::~Node()
{
    // implementation of the destructor goes here
}
struct VaraibleNode
{
    Node *val;
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
    if (nextRegister < 0)
    {
        nextRegister = 0;
    }
}

float interptObjs(Node *op)
{
    return 0.0f;
}

string gen_opertors(Node *op, vector<string> &tabs, map<string, int> &map)
{

    if (op == nullptr)
    {
        cout << "null \n";
        return "";
    }
    // mem errro :')
    NumNode *pd;
    if ((pd = dynamic_cast<NumNode *>(op)) != nullptr)
    {
        cout << "works in num \n";

        string reg = allocateReg();
        global_string += tabs_str(tabs) + "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    varaibleNode *pd1 = dynamic_cast<varaibleNode *>(op);
    if (pd1 != nullptr)
    {
        cout << "works in var \n";
        string reg = allocateReg();

        if (map.find(pd1->varailbe->buffer) == map.end())
        {
            cerr << pd1->varailbe->buffer + " doesnt exist as a var" << endl;

            return "";
        }
        else
        {
            global_string += tabs_str(tabs) + "lw " + reg + "," + to_string(map[pd1->varailbe->buffer]) + "($sp) \n";
            return reg;
        }
    }
    if (dynamic_cast<operatorNode *>(op) != nullptr)
    {
        cout << "is in op node \n";
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t;
        if (pd->token != nullptr)
        {
            t = pd->token->id;
            cout << "token op: " + pd->token->buffer << endl;
        }
        string resultReg = allocateReg();
        if (t == type::ADDITION)
        {
            // return
            string left = gen_opertors(op->left, tabs, map);
            string right = gen_opertors(op->right, tabs, map);
            global_string += tabs_str(tabs) + "add " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            // return
            string left = gen_opertors(op->left, tabs, map);
            string right = gen_opertors(op->right, tabs, map);
            global_string += tabs_str(tabs) + "sub " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MULTIPLY)
        {
            // return
            string left = gen_opertors(op->left, tabs, map);
            string right = gen_opertors(op->right, tabs, map);
            global_string += tabs_str(tabs) + "mult " + left + ", " + right + " \n";
            global_string += tabs_str(tabs) + "mflo " + resultReg + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::DIVISION)
        {
            // return
            string left = gen_opertors(op->left, tabs, map);
            string right = gen_opertors(op->right, tabs, map);
            global_string += tabs_str(tabs) + "div " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MOD)
        {
            string left = gen_opertors(op->left, tabs, map);
            string right = gen_opertors(op->right, tabs, map);
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
    return "";
    // treat like register machine
}
void print_global()
{
    cout << global_string << endl;
}
void traverse(Node *node)
{
    if (node == nullptr)
    {
        cout << "null" << endl;
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

/**
 * @brief
 *
 * @param outfile
 * @param word
 */
void wf(ofstream &outfile, string word)
{
    outfile << word << endl;
}


// here includethe size of var.
//and type load into struct and it will return that
void prepare_interptMips(varaibleNode *var, map<string, int> &map)
{
    max_size += 4;
    map[var->varailbe->buffer] = max_size;
}
void gen_function(FunctionNode *function, map<string, int> &map)
{
    vector<Node *> state = function->statements;

    for (int i = 0; i < state.size(); i++)
    {
        varaibleNode *pd1 = dynamic_cast<varaibleNode *>(state[i]);
        if (pd1 != nullptr)
        {
            if (map.find(pd1->varailbe->buffer) == map.end())
            {
                prepare_interptMips(pd1, map);
            }
        }
        else
        {
            cout << "null ptr \n";
        }
    }
}

int convert_toFixPoint(float num)
{
    return (int)num / 6536;
}
/**
 * @brief
 *
 * @param op
 * @param filename
 *
 * how this wil work
 *
 * It will be give a vector of Functions
 * and then go through each node, and generate ASM using the code
 * and yeh.
 */
void gen_mips_target(Node *op, string filename = "")
{
    map<string, Node *> vars;
    string dirname = "MipsTarget/MipsTargetASM/";
    int status = fs::create_directories(dirname);

    if (filename == "")
    {
        filename = "out.s";
    }

    ofstream outfile("MipsTarget/MipsTargetASM/" + filename);
    // FILE* fp = fopen("output.s", "w");
    FunctionNode *pd = dynamic_cast<FunctionNode *>(op);
    vector<Node *> state = pd->statements;
    map<string, int> map;
    map["."] = 0;
    string word = ".data \n .text \n";

    wf(outfile, word);
// where to iterate on list of vectors
#pragma region iterate vector of functions sarts here
    string function_name = pd->nameOfFunction->buffer + ": \n";
    wf(outfile, function_name);
    gen_function(pd, map);

    vector<string> tab;
    addtabs(tab);
    string setupstack = tabs_str(tab) + "addi $sp, $sp,-" + to_string(max_size) + " # Move the stack pointer down by 8 bytes\n";
    wf(outfile, setupstack);
    for (int i = 0; i < state.size(); i++)
    {
        varaibleNode *pd1 = dynamic_cast<varaibleNode *>(state[i]);
        funcCallNode *pd2 = dynamic_cast<funcCallNode *>(state[i]);

        if (pd1 != nullptr)
        {
            // expression tree at its finest
            if (check_if_pureExpression(pd1->expression) == 0)
            {
                string allocr = allocateReg();
                string a = tabs_str(tab) + "li " + allocr + "," + to_string(constant_prop(pd1->expression)) + "\n";
                string add = tabs_str(tab) + "sw " + allocr + "," + to_string(map[pd1->varailbe->buffer]) + "($sp) \n";
                wf(outfile, a);
                wf(outfile, add);
            }
            else
            {

                string add = tabs_str(tab) + "sw " + gen_opertors(pd1->expression, tab, map) + "," + to_string(map[pd1->varailbe->buffer]) + "($sp) \n";
                cout << "string: " + global_string << endl;
                wf(outfile, global_string);
                global_string = "";
                wf(outfile, add);
            }
            freeReg();
        }
#pragma region function calls
        else if (pd2 != nullptr)
        {
            if (pd2->funcCall->id == type::PRINT)
            {
                vector<Node *> para = pd2->params;
                string gen_code = tabs_str(tab) + "li $v0, 1 \n"; // remeber to make type depenedent
                for (int i = 0; i < para.size(); i++)
                {
                    global_string = "";
                    if (check_if_pureExpression(para[i]) == 0)
                    {
                        string allocr = allocateReg();
                        string a = tabs_str(tab) + "li " + allocr + "," + to_string(constant_prop(para[i])) + "\n";
                        gen_code += tabs_str(tab) + "move $a0, " + allocr + "\n";
                    }
                    else
                    {
                        string reg = gen_opertors(para[i], tab, map);
                        wf(outfile, global_string);

                        // string a = tabs_str(tab) + "li " + allocr + "," + to_string(solve(pd1->expression)) + "\n";
                        gen_code += tabs_str(tab) + "move $a0, " + reg + "\n";
                        global_string = "";
                        // string add = tabs_str(tab) + "sw " + gen_opertors(pd1->expression, tab, map) + "," + to_string(map[pd1->varailbe->buffer]) + "($sp) \n";
                        // cout << "string: " + global_string << endl;
                        // wf(outfile, global_string);
                        // global_string = "";
                        // wf(outfile, add);
                    }

                    // if (map.find(a[i]->buffer) != map.end())
                    // {
                    //     // li      $v0,    1               # system call for print integer
                    //     // move    $a0,    $t1             # integer value to print
                    //     // syscall
                    //     string reg = allocateReg();

                    // gen_code += tabs_str(tab) + "lw " + reg + "," +  + "($sp) \n";
                    //     gen_code += tabs_str(tab) + "move $a0, " + reg + "\n";
                    // }
                }
                wf(outfile, gen_code);
                wf(outfile, tabs_str(tab) + "syscall \n");
                freeReg();
            }
            else if (pd2->funcCall->id == type::EXIT)
            {
                string ext = tabs_str(tab)+"li $v0, 10 \n"+tabs_str(tab)+"syscall \n";
                wf(outfile, ext);
            }
            else
            { // for custom function calls
            }
        }
#pragma endregion // function calls
        else
        {
            cout << "null ptr \n";
        }
    }
    string exitStack = tabs_str(tab) + "addi $sp, $sp," + to_string(max_size) + " # Move the stack pointer down by 8 bytes\n" + tabs_str(tab) + "jr $ra \n";
    wf(outfile, exitStack);
#pragma endregion // iterate function
    // traverse(op);
    // gen_opertors(op,tab);
    // traverse(op);
    // if (check_if_pureExpression(op) == 1)
    // {
    //     int numSolve = solve(op);
    //     string register1 = allocateReg();
    //     string store = tabs_str(tab) + "li " + register1 + ", " + to_string(numSolve);
    //     wf(outfile, store);
    // }
    // traverse(op);
    // string reg_result = gen_opertors(op, tab);
    // cout << reg_result;
    // wf(outfile, global_string);
    // string printConsole = tabs_str(tab) + "li $v0, 1 \n" + tabs_str(tab) + "move $a0," + reg_result + "\n" + tabs_str(tab) + "syscall # prints to console\n";
    // wf(outfile, printConsole);
    // // write everything in
    string exitStuff = tabs_str(tab) + "li $v0, 10 \n" + tabs_str(tab) + "syscall # exited program pop into QtSpim and it should work";
    wf(outfile, exitStuff);
    outfile.close();
}
// void gen_x86_target(Node *op, string filename = "")
// {
//     if (filename == "")
//     {
//         filename = "out.s";
//     }
//     ofstream outfile(filename);
//     string word = "section .data \n section .text \n \t global main \n main: \n";
//     wf(outfile, word);
//     string exit = "\tpush rbp \n\t mov rax,0 \n \t pop rbp \n \t ret";
//     wf(outfile, exit);

//     // mov        rax,0        ; Exit code 0
//     //  pop        rbp          ; Pop stack
//     //  ret
//     outfile.close();
// }
// string gen_print()
// {
//     return "";
// }
