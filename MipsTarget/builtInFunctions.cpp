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
    gen_string += "li $a0, 10 \n li $v0, 11 \n syscall \n"; // new line
}
void Print::execute_code_float(string &gen_string, string registers = "")
{

    /*
    slt     $t2,    $t1,    0
    mul     $t2,    $t2,    45
    mflo    $t3
    li      $a0,    $t2
    li      $v0,    11
    syscall
    */
    string wholeNumReg = allocateReg();
    string fracReg = allocateReg();
    string negatveReg = allocateReg();
    gen_string += "slt " + negatveReg + ", " + registers + ", 0 \n";
    gen_string += "mul " + negatveReg + ", " + negatveReg + ", 45 \n";
    gen_string += "mflo " + negatveReg + "\n";
    gen_string += "move $a0, "+negatveReg +"\n li $v0, 11 \n syscall \n"; 


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