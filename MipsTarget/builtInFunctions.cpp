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
#pragma endregion

#pragma region print

Print::Print()
{
}
void Print::execute_code_integer(string &gen_string, string registers)
{
    gen_string += "li  $v0, 1 \n move $a0, " + registers + "\n syscall \n";
}
void Print::execute_code_float(string &gen_string, string registers = "")
{
    string wholeNumReg = allocateReg();
    string fracReg = allocateReg();

    gen_string += "srl " + wholeNumReg + "," + registers + ", 16 \n";
    gen_string += "andi " + fracReg + ", " + registers + ", 0xFFFF \n";
    gen_string += "mul " + fracReg + ", " + fracReg + ", 10000 \n";
    gen_string += "mflo " + fracReg + "\n";
    gen_string += "div " + fracReg + ", " + fracReg + ", 0xFFFF \n";

    execute_code_integer(gen_string, wholeNumReg);

    gen_string += "li $a0, 46 \n li $v0, 11 \n syscall \n";
    execute_code_integer(gen_string, fracReg);
}
#pragma endregion

#pragma region exit
Exit::Exit()
{
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