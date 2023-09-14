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

using namespace std;

#define OFFSET_HEXA 0x10000

template <typename Base, typename T>
bool instanceof (T * ptr)
{
    return dynamic_cast<Base>(ptr);
}

int max_size = 0;

Node::~Node()
{
    // implementation of the destructor goes here
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
string gen_string(Node *op, vector<string> &tabs, map<string, Varaible *> &map)
{
    if (op == nullptr)
    {
        cout << "null \n";
        return "";
    }
    if (instanceof <stringNode *>(op))
    {
        stringNode *pd = dynamic_cast<stringNode *>(op);
        // allocate size

        /*
          li $v0,9      # allocate syscall code.
        li $a0,64    # 64 bytes allocated in heap.
        syscall
        sw $v0, p   # pointer p to  heap space

        li $t0,'a'      # first byte 'a' char
        sb $t0,0($v0)
        li $t0,'b'      # second byte 'b' char
        sb $t0,1($v0)
        li $t0, 'c'
        sb $t0, 2($v0)
        li $t0, 'd'
        sb $t0, 3($v0)

        */
        // and yehs
    }
}
string gen_float_op(Node *op, map<string, Varaible *> &map)
{
    if (op == nullptr)
    {
        cout << "null \n";
        return "";
    }
    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        cout << "works in num \n";
        string reg = allocateReg();
        int num = stoi(pd->num) * OFFSET;
        global_string += "li " + reg + "," + to_string(num) + "\n";
        return reg;
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        string reg = allocateReg();
        global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }

    // varaibleNode *pd1 = dynamic_cast<varaibleNode *>(op);
    if (instanceof <VaraibleReference *>(op))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);
        // type a = map[pd1->varailbe->buffer]->varType->id;
        if (map[pd->varailbe->buffer]->varType->id == type::INT)
        {

            string reg = allocateReg();
            string reg2 = allocateReg();
            string resultReg = allocateReg();

            global_string += "lw " + reg + "," + to_string(map[pd->varailbe->buffer]->stackNum) + "($sp) \n";

            global_string += "li " + reg2 + "," + to_string(OFFSET) + "\n";

            global_string += "mult " + reg + "," + reg2 + " \n";
            global_string += "mflo " + resultReg + " \n"; // scaling
            freeReg();
            freeReg();

            return resultReg;
        }
        else
        {
            string reg = allocateReg();
            global_string += "lw " + reg + "," + to_string(map[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            return reg;
        }
        string reg = allocateReg();
        global_string += "lw " + reg + "," + to_string(map[pd->varailbe->buffer]->stackNum) + "($sp) \n";
        return reg;
    }
    if (instanceof <operatorNode *>(op))
    {
        cout << "is in op node \n";
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t = pd->token->id;

        string resultReg = allocateReg();
        if (t == type::ADDITION)
        {
            // return
            string left = gen_float_op(op->left, map);

            string right = gen_float_op(op->right, map);
            global_string += "add " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            // return
            string left = gen_float_op(op->left, map);
            cout << "sub";

            string right = gen_float_op(op->right, map);

            global_string += "sub " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MULTIPLY)
        {
            // return
            string left = gen_float_op(op->left, map);
            string right = gen_float_op(op->right, map);
            global_string += "mult " + left + ", " + right + " \n";
            global_string += "mflo " + resultReg + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::DIVISION)
        {
            // return
            string left = gen_float_op(op->left, map);
            string right = gen_float_op(op->right, map);
            global_string += "div " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MOD)
        {

            string left = gen_float_op(op->left, map);
            string right = gen_float_op(op->right, map);
            global_string += "div " + resultReg + "," + left + ", " + right + " \n";
            global_string += "mfhi " + resultReg + "\n";
            freeReg();
            freeReg();
            return resultReg;
            //           div $t4,$v0, $t3
            // div $t4, $t3, 10
            // mfhi $t3
        }
    }
}
string gen_integer_op(Node *op, map<string, Varaible *> &map)
{

    if (op == nullptr)
    {
        cout << "null \n";
        return "";
    }

    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        cout << "works in num \n";

        string reg = allocateReg();
        global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        string reg = allocateReg();
        float fixedpoint = (float)stoi(pd->num) / OFFSET;
        int fp2 = (int)fixedpoint;
        string num = to_string(fp2);
        global_string += "li " + reg + "," + num + "\n";
        return reg;
    }

    if (instanceof <VaraibleReference *>(op))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);

        cout << "works in var \n";
        string reg = allocateReg();

        if (map.find(pd->varailbe->buffer) == map.end())
        {
            cerr << pd->varailbe->buffer + " doesnt exist as a var" << endl;

            return "";
        }
        else
        {
            cout << "out: " << map[pd->varailbe->buffer] << endl;
            if (map[pd->varailbe->buffer]->varType->id == type::FLOAT)
            {
                string reg = allocateReg();
                string resultReg = allocateReg();

                global_string += "lw " + reg + "," + to_string(map[pd->varailbe->buffer]->stackNum) + "($sp) \n";
                global_string += "div " + reg + "," + reg + ", " + to_string(OFFSET) + " \n"; // scaling. I forgot i worked on this lmao :')

                freeReg();
                return reg;
            }
            else
            {
                string reg = allocateReg();
                global_string += "lw " + reg + "," + to_string(map[pd->varailbe->buffer]->stackNum) + "($sp) \n";
                return reg;
            }
        }
    }
    if (instanceof <operatorNode *>(op))
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
            string left = gen_integer_op(op->left, map);
            string right = gen_integer_op(op->right, map);
            global_string += "add " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            // return
            string left = gen_integer_op(op->left, map);
            string right = gen_integer_op(op->right, map);
            global_string += "sub " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MULTIPLY)
        {
            // return
            string left = gen_integer_op(op->left, map);
            string right = gen_integer_op(op->right, map);
            global_string += "mult " + left + ", " + right + " \n";
            global_string += "mflo " + resultReg + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::DIVISION)
        {
            // return
            string left = gen_integer_op(op->left, map);
            string right = gen_integer_op(op->right, map);
            global_string += "div " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MOD)
        {
            string left = gen_integer_op(op->left, map);
            string right = gen_integer_op(op->right, map);
            global_string += "div " + resultReg + "," + left + ", " + right + " \n";
            global_string += "mfhi " + resultReg + "\n";
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

int nOfBranch = 0;
string handle_boolean(Node *op, map<string, Varaible *> map, int isLoop = 0)
{
    if (op == nullptr)
    {

        return "";
        // c++ moment. casually coding and bam "cout is ambigous " T~T
    }
    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        cout << "works in num \n";
        string reg = allocateReg();
        int num = stoi(pd->num) * OFFSET;
        global_string += "li " + reg + "," + to_string(num) + "\n";
        return reg;
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        string reg = allocateReg();
        global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    if (instanceof <BooleanLiteralNode *>(op))
    {
        BooleanLiteralNode *pd = dynamic_cast<BooleanLiteralNode *>(op);
        string reg = allocateReg();
        if (pd->value->id == type::TRUE)
        {
            global_string += "li " + reg + ", 1 \n";
        }
        else
        {
            global_string += "li " + reg + ", 0 \n";
        }
        // global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    // varaibleNode *pd1 = dynamic_cast<varaibleNode *>(op);
    if (instanceof <VaraibleReference *>(op))
    {

        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);
        // type a = map[pd1->varailbe->buffer]->varType->id;
        if (map[pd->varailbe->buffer]->varType->id == type::INT)
        {

            string reg = allocateReg();
            string reg2 = allocateReg();
            string resultReg = allocateReg();

            global_string += "lw " + reg + "," + to_string(map[pd->varailbe->buffer]->stackNum) + "($sp) \n";

            global_string += "li " + reg2 + "," + to_string(OFFSET) + "\n";

            global_string += "mult " + reg + "," + reg2 + " \n";
            global_string += "mflo " + resultReg + " \n"; // scaling
            freeReg();
            freeReg();

            return resultReg;
        }
        else
        {
            string reg = allocateReg();
            global_string += "lw " + reg + "," + to_string(map[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            return reg;
        }
        string reg = allocateReg();
        global_string += "lw " + reg + "," + to_string(map[pd->varailbe->buffer]->stackNum) + "($sp) \n";
        return reg;
    }
    if (instanceof <BoolExpressionNode *>(op))
    {
        BoolExpressionNode *pd = dynamic_cast<BoolExpressionNode *>(op);
        if (pd->op->id == type::BOOL_EQ)
        {
            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "beq " + handle_boolean(pd->right, map, isLoop) + " ," + handle_boolean(pd->left, map, isLoop) + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "bne " + handle_boolean(pd->right, map) + " ," + handle_boolean(pd->left, map) + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
        if (pd->op->id == type::GT)
        {
            if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
            {
                return "";
            }
            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, map, isLoop) + " ," + handle_boolean(pd->left, map, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, map) + " ," + handle_boolean(pd->left, map) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
        if (pd->op->id == type::LT)
        {
            if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
            {
                return "";
            }
            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, map, isLoop) + " ," + handle_boolean(pd->left, map, isLoop) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, map) + " ," + handle_boolean(pd->left, map) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
        if (pd->op->id == type::LTE)
        {
            if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
            {
                return "";
            }

            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->left, map, isLoop) + " ," + handle_boolean(pd->right, map, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->left, map) + " ," + handle_boolean(pd->right, map) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
        if (pd->op->id == type::GTE)
        {

            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, map, isLoop) + " ," + handle_boolean(pd->left, map, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, map) + " ," + handle_boolean(pd->left, map) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
    }
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
// and type load into struct and it will return that
void prepare_interptMips(VaraibleDeclaration *var, map<string, Varaible *> &map, int size)
{
    // max_size += 4;
    Varaible *a = new Varaible;
    a->stackNum = max_size;
    a->varType = var->typeOfVar;
    map[var->varailbe->buffer] = a;
}
int stack_number = 0;

void gen_function(vector<Node *> state, int &stackNum)
{
    if (state.size() == 0)
    {
        return;
    }

    for (int i = 0; i < state.size(); i++)
    {
        if (instanceof <IfSatementNode *>(state[i]))
        {
            IfSatementNode *pd = dynamic_cast<IfSatementNode *>(state[i]);
            gen_function(pd->statements, stackNum);
        }
        else if (instanceof <LoopNode *>(state[i]))
        {
            LoopNode *pd = dynamic_cast<LoopNode *>(state[i]);
            gen_function(pd->statements, stackNum);
        }
        else if (instanceof <VaraibleDeclaration *>(state[i]))
        {
            VaraibleDeclaration *pd = dynamic_cast<VaraibleDeclaration *>(state[i]);
            stackNum += 4;
        }
        // if (instanceof <IfSatementNode *>(state[i]))
        // {
        //     IfSatementNode *pd = dynamic_cast<IfSatementNode *>(state[i]);
        //     gen_function(pd->statements, map);
        // }
        // else if (instanceof <LoopNode *>(state[i]))
        // {
        //     LoopNode *pd = dynamic_cast<LoopNode *>(state[i]);
        //     gen_function(pd->statements, map);
        // }
        // else if (instanceof <varaibleNode *>(state[i]))
        // {
        //     varaibleNode *pd = dynamic_cast<varaibleNode *>(state[i]);

        //     if (map.find(pd->varailbe->buffer) == map.end())
        //     {
        //         prepare_interptMips(pd, map);
        //     }
        // }
    }
}
void interptFunctions()
{
}
// void gen_function(FunctionNode *function, map<string, Varaible *> &map)
// {
//     vector<Node *> state = function->statements;

//     for (int i = 0; i < state.size(); i++)
//     {

//         if (instanceof <varaibleNode *>(state[i]))
//         {
//             varaibleNode *pd = dynamic_cast<varaibleNode *>(state[i]);

//             if (map.find(pd->varailbe->buffer) == map.end())
//             {
//                 prepare_interptMips(pd, map);
//             }
//         }
//         else
//         {
//             cout << "null ptr \n";
//         }
//     }
// }

// int convert_toFixPoint(float num)
// {
//     return (int)num / 6536;
// }
void statementsGen(Node *statement, map<string, Varaible *> &var, ofstream &outfile)
{
    map<type, builtInFunction *> functions;

    functions[type::PRINT] = new Print();
    functions[type::EXIT] = new Exit();

    cout << "a" << endl;

    if (instanceof <VaraibleDeclaration *>(statement))
    {
        VaraibleDeclaration *pd = dynamic_cast<VaraibleDeclaration *>(statement);

        prepare_interptMips(pd, var, 4);

        if (check_if_pureExpression(pd->expression) == 0)
        {

            Varaible *type1 = var[pd->varailbe->buffer];
            if (type1 == nullptr)
            {
                return;
            }
            string allocr = allocateReg();
            if (type1->varType->id == type::FLOAT)
            {
                float constantF = (constant_prop_float(pd->expression));
                int work1 = (int)(constantF * OFFSET);
                string a = "li " + allocr + "," + to_string(work1) + "\n";
                wf(outfile, a);
            }
            else if (type1->varType->id == type::INT)
            {
                string a = "li " + allocr + "," + to_string(constant_prop_integer(pd->expression)) + "\n";
                wf(outfile, a);
            }
            else
            {
                string a = "li " + allocr + "," + to_string(constant_prop_boolean(pd->expression)) + " \n";
                wf(outfile, a);
            }
            cout << pd->varailbe->buffer << endl;
            cout << to_string(var[pd->varailbe->buffer]->stackNum) << endl;
            cout << "hi \n";
            string add = "sw " + allocr + "," + to_string(var[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            wf(outfile, add);
        }
        else
        {
            Varaible *type1 = var[pd->varailbe->buffer];
            if (type1 == nullptr)
            {
                return;
            }
            cout << "else \n";
            string add = "";
            if (type1->varType->id == type::FLOAT)
            {
                string reg = gen_float_op(pd->expression, var);
                add += "sw " + reg + "," + to_string(var[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            }
            else if (type1->varType->id == type::INT)
            {
                add = "sw " + gen_integer_op(pd->expression, var) + "," + to_string(var[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            }
            else
            {
                add = "sw " + handle_boolean(pd->expression, var) + "," + to_string(var[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            }

            cout << "string: " + global_string << endl;
            wf(outfile, global_string);
            global_string = "";
            wf(outfile, add);
        }
    }
    if (instanceof <VaraibleReference *>(statement))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(statement);

        // else
        // {
        //     *pd = dynamic_cast<VaraibleReference *>(statement);
        //     expression = pd->expression;
        // }
        if (check_if_pureExpression(pd->expression) == 0)
        {

            Varaible *type1 = var[pd->varailbe->buffer];
            if (type1 == nullptr)
            {
                return;
            }
            string allocr = allocateReg();
            if (type1->varType->id == type::FLOAT)
            {
                float constantF = (constant_prop_float(pd->expression));
                int work1 = (int)(constantF * OFFSET);
                string a = "li " + allocr + "," + to_string(work1) + "\n";
                wf(outfile, a);
            }
            else if (type1->varType->id == type::INT)
            {
                string a = "li " + allocr + "," + to_string(constant_prop_integer(pd->expression)) + "\n";
                wf(outfile, a);
            }
            else
            {
                string a = "li " + allocr + "," + to_string(constant_prop_boolean(pd->expression)) + " \n";
                wf(outfile, a);
            }
            cout << pd->varailbe->buffer << endl;
            cout << to_string(var[pd->varailbe->buffer]->stackNum) << endl;
            cout << "hi \n";
            string add = "sw " + allocr + "," + to_string(var[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            wf(outfile, add);
        }
        else
        {
            Varaible *type1 = var[pd->varailbe->buffer];
            if (type1 == nullptr)
            {
                return;
            }
            cout << "else \n";
            string add = "";
            if (type1->varType->id == type::FLOAT)
            {
                string reg = gen_float_op(pd->expression, var);
                add += "sw " + reg + "," + to_string(var[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            }
            else if (type1->varType->id == type::INT)
            {
                add = "sw " + gen_integer_op(pd->expression, var) + "," + to_string(var[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            }
            else
            {
                add = "sw " + handle_boolean(pd->expression, var) + "," + to_string(var[pd->varailbe->buffer]->stackNum) + "($sp) \n";
            }

            cout << "string: " + global_string << endl;
            wf(outfile, global_string);
            global_string = "";
            wf(outfile, add);
        }
        freeReg();
    }

    else if (instanceof <funcCallNode *>(statement))
    {
        funcCallNode *pd = dynamic_cast<funcCallNode *>(statement);
        if (functions.find(pd->funcCall->id) != functions.end())
        {
            vector<Node *> para = pd->params;
            builtInFunction *func = functions[pd->funcCall->id];
            if (func != nullptr)
            {
                string gen_string = "";
                func->setup_params(para, gen_string, var);
                wf(outfile, gen_string);
            }
        }
        else
        {
            // load params into register a and do the rest
        }
    }
    else if (instanceof <IfSatementNode *>(statement))
    {
        global_string = "";
        IfSatementNode *pd = dynamic_cast<IfSatementNode *>(statement);
        if (pd->statements.size() == 0)
        {
            return;
        }
        handle_boolean(pd->condition, var);

        wf(outfile, global_string);

        global_string = "";
        int c = 1;

        for (int i = 0; i < pd->statements.size(); i++)
        {

            statementsGen(pd->statements[i], var, outfile); // write a new function for this T~T
            if (instanceof <IfSatementNode *>(pd->statements[i]))
            {
                c = 1;
            }
        }

        // handle statements this will be a recursive function later
        if (c == 1)
        {
            global_string = "L" + to_string(nOfBranch) + ": \n";
            wf(outfile, global_string);
            nOfBranch++;
        }
        c = 0;
        global_string = "";
    }
    else if (instanceof <LoopNode *>(statement))
    {

        // wip
        global_string = "";
        LoopNode *pd = dynamic_cast<LoopNode *>(statement);
        if (pd->statements.size() == 0)
        {
            return;
        }
        nOfBranch++;
        int b = nOfBranch;
        global_string += "b L" + to_string(b) + "\n";
        nOfBranch++;
        global_string += "L" + to_string(nOfBranch) + ": \n # loop"; // condition
        wf(outfile, global_string);
        global_string = "";

        handle_boolean(pd->condition, var, 1);
        string condition = global_string;
        nOfBranch++;
        global_string = "";
        for (int i = 0; i < pd->statements.size(); i++)
        {

            statementsGen(pd->statements[i], var, outfile); // write a new function for this T~T
        }
        global_string += "L" + to_string(b) + ": \n # condition";
        wf(outfile, global_string);
        wf(outfile, condition);

        global_string = "";

        // wf(outfile, global_string);
    }
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
    gen_function(state, max_size);

    vector<string> tab;
    addtabs(tab);
    string setupstack = "";
    if (max_size != 0)
    {
        setupstack = "addi $sp, $sp,-" + to_string(max_size) + " # Move the stack pointer down by " + to_string(max_size) + " bytes\n";
    }
    else
    {
        setupstack = "addi $sp, $sp, " + to_string(max_size) + " # Move the stack pointer down by " + to_string(max_size) + " bytes\n";
    }
    wf(outfile, setupstack);
    for (int i = 0; i < state.size(); i++)
    {
        statementsGen(state[i], map, outfile);
    }
    string exitStack = "addi $sp, $sp," + to_string(max_size) + " # Move the stack pointer up by " + to_string(max_size) + " bytes\n  jr $ra \n";
    wf(outfile, exitStack);
#pragma endregion // iterate function
    // write everything in
    string exitStuff = "li $v0, 10 \n syscall # exited program pop into QtSpim and it should work";
    wf(outfile, exitStuff);
    outfile.close();
}
void a()
{
    gen_mips_target(nullptr);
}
