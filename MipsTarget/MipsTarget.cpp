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
#include "../MipsTarget/builtInFunction.h"

namespace fs = std::filesystem;

#define OFFSET_HEXA 0x10000
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

struct Varaible
{
    Tokens *varType;
    int stackNum;
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

#pragma region this is for the expression tree

string gen_float_op(Node *op, vector<string> &tabs, map<string, Varaible *> &map)
{
    IntegerNode *pd;
    if ((pd = dynamic_cast<IntegerNode *>(op)) != nullptr)
    {
        cout << "works in num \n";
        string reg = allocateReg();
        int num = stoi(pd->num) * OFFSET;
        global_string += tabs_str(tabs) + "li " + reg + "," + to_string(num) + "\n";
        return reg;
    }
    FloatNode *pd2;
    if ((pd2 = dynamic_cast<FloatNode *>(op)) != nullptr)
    {
        string reg = allocateReg();
        global_string += tabs_str(tabs) + "li " + reg + "," + pd2->num + "\n";
        return reg;
    }

    varaibleNode *pd1 = dynamic_cast<varaibleNode *>(op);
    if (pd1 != nullptr)
    {
        // type a = map[pd1->varailbe->buffer]->varType->id;
        if (map[pd1->varailbe->buffer]->varType->id == type::INT) // go back later :')
        {

            string reg = allocateReg();
            string reg2 = allocateReg();
            string resultReg = allocateReg();

            global_string += tabs_str(tabs) + "lw " + reg + "," + to_string(map[pd1->varailbe->buffer]->stackNum) + "($sp) \n";

            global_string += tabs_str(tabs) + "li " + reg2 + "," + to_string(OFFSET) + "\n";

            global_string += tabs_str(tabs) + "mult " + reg + "," + reg2 + " \n";
            global_string += tabs_str(tabs) + "mflo " + resultReg + " \n";
            freeReg();
            freeReg();

            return resultReg;
        }
        else
        {
            string reg = allocateReg();
            global_string += tabs_str(tabs) + "lw " + reg + "," + to_string(map[pd1->varailbe->buffer]->stackNum) + "($sp) \n";
            return reg;
        }
        string reg = allocateReg();
        global_string += tabs_str(tabs) + "lw " + reg + "," + to_string(map[pd1->varailbe->buffer]->stackNum) + "($sp) \n";
        return reg;
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
            string left = gen_float_op(op->left, tabs, map);

            string right = gen_float_op(op->right, tabs, map);
            global_string += tabs_str(tabs) + "add " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            // return
            string left = gen_float_op(op->left, tabs, map);
            cout << "sub";

            string right = gen_float_op(op->right, tabs, map);

            global_string += tabs_str(tabs) + "sub " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MULTIPLY)
        {
            // return
            string left = gen_float_op(op->left, tabs, map);
            string right = gen_float_op(op->right, tabs, map);
            global_string += tabs_str(tabs) + "mult " + left + ", " + right + " \n";
            global_string += tabs_str(tabs) + "mflo " + resultReg + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::DIVISION)
        {
            // return
            string left = gen_float_op(op->left, tabs, map);
            string right = gen_float_op(op->right, tabs, map);
            global_string += tabs_str(tabs) + "div " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MOD)
        {
            string left = gen_float_op(op->left, tabs, map);
            string right = gen_float_op(op->right, tabs, map);
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
}
string gen_integer_op(Node *op, vector<string> &tabs, map<string, Varaible *> &map)
{

    if (op == nullptr)
    {
        cout << "null \n";
        return "";
    }

    IntegerNode *pd;
    if ((pd = dynamic_cast<IntegerNode *>(op)) != nullptr)
    {
        cout << "works in num \n";

        string reg = allocateReg();
        global_string += tabs_str(tabs) + "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    FloatNode *pd2;
    if ((pd2 = dynamic_cast<FloatNode *>(op)) != nullptr)
    {
        string reg = allocateReg();
        float fixedpoint = (float)stoi(pd->num) / OFFSET;
        int fp2 = (int)fixedpoint;
        string num = to_string(fp2);
        global_string += tabs_str(tabs) + "li " + reg + "," + pd->num + "\n";
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
            cout << "out: " << map[pd1->varailbe->buffer] << endl;
            if (map[pd1->varailbe->buffer]->varType->id == type::FLOAT)
            {
                string reg = allocateReg();
                string resultReg = allocateReg();

                global_string += tabs_str(tabs) + "lw " + reg + "," + to_string(map[pd1->varailbe->buffer]->stackNum) + "($sp) \n";
                freeReg();
                return reg;
            }
            else
            {
                string reg = allocateReg();
                global_string += tabs_str(tabs) + "lw " + reg + "," + to_string(map[pd1->varailbe->buffer]->stackNum) + "($sp) \n";
                return reg;
            }
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
            string left = gen_integer_op(op->left, tabs, map);
            string right = gen_integer_op(op->right, tabs, map);
            global_string += tabs_str(tabs) + "add " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            // return
            string left = gen_integer_op(op->left, tabs, map);
            string right = gen_integer_op(op->right, tabs, map);
            global_string += tabs_str(tabs) + "sub " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MULTIPLY)
        {
            // return
            string left = gen_integer_op(op->left, tabs, map);
            string right = gen_integer_op(op->right, tabs, map);
            global_string += tabs_str(tabs) + "mult " + left + ", " + right + " \n";
            global_string += tabs_str(tabs) + "mflo " + resultReg + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::DIVISION)
        {
            // return
            string left = gen_integer_op(op->left, tabs, map);
            string right = gen_integer_op(op->right, tabs, map);
            global_string += tabs_str(tabs) + "div " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MOD)
        {
            string left = gen_integer_op(op->left, tabs, map);
            string right = gen_integer_op(op->right, tabs, map);
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
#pragma endregion

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
// and type load into struct and it will return that
void prepare_interptMips(varaibleNode *var, map<string, Varaible *> &map)
{
    max_size += 4;
    Varaible *a = new Varaible;
    a->stackNum = max_size;
    a->varType = var->typeOfVar;
    map[var->varailbe->buffer] = a;
}
void gen_function(FunctionNode *function, map<string, Varaible *> &map)
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
void gen_mips_target(Node *op, string filename)
{
    map<string, Node *> vars;
    map<type, builtInFunction *> functions;

    functions[type::PRINT] = new Print();
    functions[type::EXIT] = new Exit();

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
    map<string, Varaible *> map;
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
    string setupstack = tabs_str(tab) + "addi $sp, $sp,-" + to_string(max_size) + " # Move the stack pointer down by " + to_string(max_size) + " bytes\n";
    wf(outfile, setupstack);
    for (int i = 0; i < state.size(); i++)
    {
        varaibleNode *pd1 = dynamic_cast<varaibleNode *>(state[i]);
        funcCallNode *pd2 = dynamic_cast<funcCallNode *>(state[i]);

        if (pd1 != nullptr)
        {
            // expression tree at its finest
            cout << check_if_pureExpression(pd1->expression) << endl;
            if (check_if_pureExpression(pd1->expression) == 0)
            {
                Varaible *type1 = map[pd1->varailbe->buffer];
                if (type1 == nullptr)
                {
                    return;
                }
                string allocr = allocateReg();
                if (type1->varType->id == type::FLOAT)
                {
                    float constantF = (constant_prop_float(pd1->expression));
                    int work1 = (int)(constantF * OFFSET);
                    string a = tabs_str(tab) + "li " + allocr + "," + to_string(work1) + "\n";
                    wf(outfile, a);
                }
                else if (type1->varType->id == type::INT)
                {
                    string a = tabs_str(tab) + "li " + allocr + "," + to_string(constant_prop_integer(pd1->expression)) + "\n";
                    wf(outfile, a);
                }
                cout << pd1->varailbe->buffer << endl;
                cout << to_string(map[pd1->varailbe->buffer]->stackNum) << endl;
                cout << "hi \n";
                string add = tabs_str(tab) + "sw " + allocr + "," + to_string(map[pd1->varailbe->buffer]->stackNum) + "($sp) \n";
                wf(outfile, add);
            }
            else
            {
                Varaible *type1 = map[pd1->varailbe->buffer];
                if (type1 == nullptr)
                {
                    return;
                }
                cout << "else \n";
                string add = "";
                if (type1->varType->id == type::FLOAT)
                {
                    string reg = gen_float_op(pd1->expression, tab, map);
                    add += tabs_str(tab) + "sw " + reg + "," + to_string(map[pd1->varailbe->buffer]->stackNum) + "($sp) \n";
                }
                else
                {
                    add = tabs_str(tab) + "sw " + gen_integer_op(pd1->expression, tab, map) + "," + to_string(map[pd1->varailbe->buffer]->stackNum) + "($sp) \n";
                }
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
            if (functions.find(pd2->funcCall->id) != functions.end())
            {
                vector<Node *> para = pd2->params;
                for (int i = 0; i < para.size(); i++)
                {
                    builtInFunction *func = functions[pd2->funcCall->id];

                    string reg = "";
                    string gen_code = "";
                    if (check_if_pureExpression(para[i]) == 0)
                    {
                        reg = allocateReg();
                        string a = tabs_str(tab) + "li " + reg + "," + to_string(constant_prop_float(para[i])) + "\n";
                        wf(outfile, a);
                        func->execute_code_float(gen_code, reg);

                        wf(outfile, gen_code);
                        // gen_code = "";
                    }
                    else
                    {
                        reg = gen_float_op(para[i], tab, map);
                        wf(outfile, global_string);
                        global_string = "";

                        // string a = tabs_str(tab) + "li " + allocr + "," + to_string(solve(pd1->expression)) + "\n";
                        func->execute_code_float(gen_code, reg);
                        wf(outfile, gen_code);
                        // gen_code = "";
                        // string add = tabs_str(tab) + "sw " + gen_opertors(pd1->expression, tab, map) + "," + to_string(map[pd1->varailbe->buffer]) + "($sp) \n";
                        // cout << "string: " + global_string << endl;
                        // wf(outfile, global_string);
                        // global_string = "";
                        // wf(outfile, add);
                    }
                }
            }
            else
            {
            }
#pragma endregion // function calls
        }
    }
    string exitStack = tabs_str(tab) + "addi $sp, $sp," + to_string(max_size) + " # Move the stack pointer down by 8 bytes\n" + tabs_str(tab) + "jr $ra \n";
    wf(outfile, exitStack);
#pragma endregion // iterate function
    // write everything in
    string exitStuff = tabs_str(tab) + "li $v0, 10 \n" + tabs_str(tab) + "syscall # exited program pop into QtSpim and it should work";
    wf(outfile, exitStuff);
    outfile.close();
}
void a()
{
    gen_mips_target(nullptr);
}
