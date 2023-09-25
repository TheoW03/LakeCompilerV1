#include <iostream>
#include "../MipsTarget/builtInFunction.h"
#include "../compilerFrontend/parser.h"
#include "../compilerFrontend/Lexxer.h"

using namespace std;

#pragma region builtin

void builtInFunction::execute_code_integer(string &gen_string, string registers)
{
}
void builtInFunction::execute_code_float(string &gen_string, string registers)
{
}
void builtInFunction::setup_params(vector<Node *> params, string &gen_string, map<string, Varaible *> map)
{
}
#pragma endregion

#pragma region print

Print::Print()
{
}

void Print::setup_params(vector<Node *> params, string &gen_string, map<string, Varaible *> map)
{
    if (instanceof <stringNode *>(params[0]))
    {

        // change to a parser for the 1st param later

        // this is very temporary and will try to add better system once i think of it
        // or once i add support for strings/string concat

        stringNode *a = dynamic_cast<stringNode *>(params[0]);
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
                gen_string += "lw " + reg + ", " + to_string(map[dynamic_cast<VaraibleReference *>(params[1])->varailbe->buffer]->stackNum) + " ($sp) \n";

                if (map[dynamic_cast<VaraibleReference *>(params[1])->varailbe->buffer]->varType->id == type::FLOAT)
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
                gen_string += "lw " + reg + ", " + to_string(map[dynamic_cast<VaraibleReference *>(params[1])->varailbe->buffer]->stackNum) + " ($sp) \n";
                if (map[dynamic_cast<VaraibleReference *>(params[1])->varailbe->buffer]->varType->id == type::INT)
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
    }
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

void Exit::setup_params(vector<Node *> params, string &gen_string, map<string, Varaible *> map)
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
#pragma endregion