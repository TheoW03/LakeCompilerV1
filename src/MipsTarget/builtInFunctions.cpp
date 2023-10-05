#include <iostream>
#include <string>
#include <vector>

#include "../../src/MipsTarget/builtInFunction.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/MipsTarget/UtilFunctions.h"
#include "../../src/CompilerFrontend/optimizations.h"
#include "../../src/MipsTarget/MipsTarget.h"
#include "../../src/MipsTarget/VaraibleScope.h"

using namespace std;

#pragma region builtin

void builtInFunction::execute_code_integer(string &gen_string, string registers)
{
}
void builtInFunction::execute_code_float(string &gen_string, string registers)
{
}
void builtInFunction::execute_code_char(string &gen_string, string registers)
{
}
void builtInFunction::setup_params(vector<Node *> params, string &gen_string, vector<Scope_dimension *> &scope)
{
}
#pragma endregion

#pragma region print

Print::Print()
{
}

void Print::setup_params(vector<Node *> params, string &gen_string, vector<Scope_dimension *> &scope)
{
    if (instanceof <StringNode *>(params[0]))
    {

        // change to a parser for the 1st param later

        // this is very temporary and will try to add better system once i think of it
        // or once i add support for strings/string concat

        StringNode *a = dynamic_cast<StringNode *>(params[0]);
        if (a->stringBuffer == "%d")
        {

            if (check_if_pureExpression(params[1]) == 0)
            {
                string reg = allocateReg();
                gen_string += "li " + reg + "," + to_string(constant_prop_integer(params[1])) + "\n";
                execute_code_integer(gen_string, reg);
            }
            else
            {
                string reg = allocateReg();
                VaraibleReference *pd = dynamic_cast<VaraibleReference *>(params[1]);
                Varaible *a = get_varaible(pd, scope);
                if (a == nullptr)
                {
                    cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
                    exit(0);
                    return;
                }
                // gen_string += "lw " + reg + ", " + to_string(map[dynamic_cast<VaraibleReference *>(params[1])->varailbe->buffer]->stackNum) + " ($sp) \n";
                gen_string += "lw " + reg + ", " + to_string(a->stackNum) + "($sp) \n";
                if (a->varType->id == type::FLOAT)
                {
                    string reg2 = allocateReg();
                    gen_string += "li " + reg2 + "," + to_string(OFFSET) + "\n";
                    gen_string += "div " + reg + "," + reg + "," + reg2 + " \n";
                    freeReg();
                }

                execute_code_integer(gen_string, reg);
                freeReg();
            }
        }
        else if (a->stringBuffer == "%f")
        {
            if (check_if_pureExpression(params[1]) == 0)
            {
                string reg = allocateReg();
                // int conversion =
                int fPoint = (int)(constant_prop_float(params[1]) * OFFSET);
                gen_string += "li " + reg + "," + to_string(fPoint) + "\n";
                execute_code_float(gen_string, reg);
                freeReg();
            }
            else
            {
                string reg = allocateReg();
                VaraibleReference *pd = dynamic_cast<VaraibleReference *>(params[1]);
                Varaible *a = get_varaible(pd, scope);
                if (a == nullptr)
                {
                    cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
                    exit(0);
                    return;
                }
                gen_string += "lw " + reg + ", " + to_string(a->stackNum) + "($sp) \n";
                // gen_string += "lw " + reg + ", " + to_string(map[dynamic_cast<VaraibleReference *>(params[1])->varailbe->buffer]->stackNum) + " ($sp) \n";
                // if (map[dynamic_cast<VaraibleReference *>(params[1])->varailbe->buffer]->varType->id == type::INT)
                if (a->varType->id == type::INT)
                {
                    string reg2 = allocateReg();
                    gen_string += "li " + reg2 + ", " + to_string(OFFSET) + "\n";
                    gen_string += "mult " + reg + ", " + reg2 + " \n";
                    gen_string += "mflo " + reg + "\n";
                    freeReg();
                }
                execute_code_float(gen_string, reg);
                freeReg();
            }
        }
        else if (a->stringBuffer == "%c")
        {
            if (check_if_pureExpression(params[1]) == 0)
            {
                string reg = allocateReg();
                gen_string += "li " + reg + "," + to_string(constant_prop_char(params[1])) + "\n";
                execute_code_char(gen_string, reg);
            }
            else
            {
                string reg = allocateReg();
                VaraibleReference *pd = dynamic_cast<VaraibleReference *>(params[1]);

                Varaible *a = get_varaible(pd, scope);
                if (a == nullptr)
                {
                    cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
                    exit(0);
                    return;
                }
                if (a->varType->id != type::CHAR && a->varType->id != type::INT)
                {
                    cout << "print char only works wih integers (0-255) or Chars" << endl;
                    exit(1);
                    return;
                }
                gen_string += "lw " + reg + ", " + to_string(a->stackNum) + "($sp) \n";
                execute_code_char(gen_string, reg);
            }
        }
    }
}
void Print::execute_code_char(string &gen_string, string registers)
{
    gen_string += "move $a0," + registers + "\n li $v0, 11 \n syscall \n"; // new line
}
void Print::execute_code_integer(string &gen_string, string registers)
{
    gen_string += "li  $v0, 1 \n move $a0, " + registers + "\n syscall \n";
    gen_string += "li $a0, 10 \n li $v0, 11 \n syscall \n"; // new line
}
void Print::execute_code_float(string &gen_string, string registers = "")
{

    string wholeNumReg = allocateReg();
    string fracReg = allocateReg();
    string negatveReg = allocateReg();

    // neagtive stuff
    gen_string += "slt " + negatveReg + ", " + registers + ", 0 \n";
    gen_string += "mul " + negatveReg + ", " + negatveReg + ", 45 \n";
    gen_string += "mflo " + negatveReg + "\n";

    gen_string += "move $a0, " + negatveReg + "\n li $v0, 11 \n syscall \n";

    gen_string += "abs " + registers + ", " + registers + "\n"; // removes -
    gen_string += "div " + wholeNumReg + "," + registers + "," + to_string(OFFSET) + "\n";

    // decimal point calculation
    // mask with offset and the mask divided by offset multiplied by 1000
    gen_string += "andi " + fracReg + ", " + registers + ", 0xFFFF \n";
    gen_string += "mul " + fracReg + ", " + fracReg + ", 10000 \n";
    gen_string += "mflo " + fracReg + "\n";
    gen_string += "div " + fracReg + ", " + fracReg + ",65535 \n";

    // print
    gen_string += "li  $v0, 1 \n move $a0, " + wholeNumReg + "\n syscall \n"; // whole number or before the decimal point
    gen_string += "li $a0, 46 \n li $v0, 11 \n syscall \n";                   //.
    gen_string += "li  $v0, 1 \n move $a0, " + fracReg + "\n syscall \n";     // decimal point num

    gen_string += "li $a0, 10 \n li $v0, 11 \n syscall \n"; // new line
    freeReg();
    freeReg();
    freeReg();
}
#pragma endregion

#pragma region exit
Exit::Exit()
{
}

void Exit::setup_params(vector<Node *> params, string &gen_string, vector<Scope_dimension *> &scope)
{
    if (params.size() == 0)
    {
        execute_code_integer(gen_string, "");
    }
}
void Exit::execute_code_integer(string &gen_string, string registers)
{
    gen_string += "li $v0, 10 \n syscall";
}
void Exit::execute_code_float(string &gen_string, string registers)
{
    gen_string += "li $v0, 10 \n syscall";
}
void Exit::execute_code_char(string &gen_string, string registers)
{
    gen_string += "li $v0, 10 \n syscall";
}
#pragma endregion