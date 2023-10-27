#include <iostream>
#include <string>
#include <vector>

#include "../../src/MipsTarget/builtInFunction.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/MipsTarget/UtilFunctions.h"
// #include "../../src/CompilerFrontend/optimizations.h"
#include "../../src/MipsTarget/MipsTarget.h"
#include "../../src/MipsTarget/VaraibleScope.h"

using namespace std;

#pragma region builtin

void builtInFunction::execute_code_integer(string &gen_string, RegisterAllocation *register_context, string registers)
{
}
void builtInFunction::execute_code_float(string &gen_string, RegisterAllocation *register_context, string registers)
{
}
void builtInFunction::execute_code_char(string &gen_string, RegisterAllocation *register_context, string registers)
{
}
void builtInFunction::setup_params(vector<Node *> params, string &gen_string, Scope_Monitor *&scope_monitor)
{
}
#pragma endregion

#pragma region print

Print::Print()
{
}

void Print::setup_params(vector<Node *> params, string &gen_string, Scope_Monitor *&scope_monitor)
{
    if (instanceof <StringNode *>(params[0]))
    {

        // change to a parser for the 1st param later

        // this is very temporary and will try to add better system once i think of it
        // or once i add support for strings/string concat

        StringNode *a = dynamic_cast<StringNode *>(params[0]);
        if (a->stringBuffer == "%d")
        {
            string reg = "";
            int b = gen_integer_op(params[1], scope_monitor, gen_string, reg);
            if (reg == "")
            {
                reg = scope_monitor->rg->allocate_register(0);
                gen_string += "li " + reg + ", " + to_string(b) + " #0 \n";
            }
            execute_code_integer(gen_string, scope_monitor->rg, reg);
        }
        else if (a->stringBuffer == "%f")
        {
            string reg = "";
            int b = gen_float_op(params[1], scope_monitor, gen_string, reg);
            if (reg == "")
            {
                reg = scope_monitor->rg->allocate_register(0);
                gen_string += "li " + reg + ", " + to_string(b) + " \n";
            }
            execute_code_float(gen_string, scope_monitor->rg, reg);
        }
        else if (a->stringBuffer == "%c")
        {
            string reg = gen_char_op(params[1], scope_monitor, gen_string);
            execute_code_char(gen_string, scope_monitor->rg, reg);
        }
    }
}
void Print::execute_code_char(string &gen_string, RegisterAllocation *register_context, string registers)
{
    gen_string += "move $a0," + registers + "\n li $v0, 11 \n syscall \n"; // new line
}
void Print::execute_code_integer(string &gen_string, RegisterAllocation *register_context, string registers)
{
    gen_string += "li  $v0, 1 \n move $a0, " + registers + "\n syscall \n";
    gen_string += "li $a0, 10 \n li $v0, 11 \n syscall \n"; // new line
}
void Print::execute_code_float(string &gen_string, RegisterAllocation *register_context, string registers = "")
{

    string wholeNumReg = register_context->allocate_register(0);
    string fracReg = register_context->allocate_register(0);
    string negatveReg = register_context->allocate_register(0);

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

void Exit::setup_params(vector<Node *> params, string &gen_string, Scope_Monitor *&scope_monitor)
{
    if (params.size() == 0)
    {
        execute_code_integer(gen_string, scope_monitor->rg, "");
    }
}
void Exit::execute_code_integer(string &gen_string, RegisterAllocation *register_context, string registers)
{
    gen_string += "li $v0, 10 \n syscall";
}
void Exit::execute_code_float(string &gen_string, RegisterAllocation *register_context, string registers)
{
    gen_string += "li $v0, 10 \n syscall";
}
void Exit::execute_code_char(string &gen_string, RegisterAllocation *register_context, string registers)
{
    gen_string += "li $v0, 10 \n syscall";
}
#pragma endregion