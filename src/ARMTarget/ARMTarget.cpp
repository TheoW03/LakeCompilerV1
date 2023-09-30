#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <experimental/filesystem>

#include <cstdio>
#include <cstdlib>
#include <sys/stat.h>
#include <filesystem>
#include <typeinfo>

#include "../../src/compilerFrontend/Lexxer.h"
#include "../../src/compilerFrontend/parser.h"
#include "../../src/MipsTarget/UtilFunctions.h"
using namespace std;
namespace fs = std::filesystem;

int currentRegister = -1;
string allocate_register()
{

    if (currentRegister >= 9)
    {
        currentRegister = -1;
    }
    currentRegister++;
    return "R" + to_string(currentRegister);
}
void free_reg()
{

    currentRegister--;
    if (currentRegister < 0)
    {
        currentRegister = 0;
    }
}
string global_string1 = "";
string gen_ARM_operators(Node *op)
{
    if (op == nullptr)
    {
        return "";
    }
    if (instanceof <IntegerNode *>(op))
    {
        string allocateReg = allocate_register();
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        global_string1 += "MOV " + allocateReg + ", " + pd->num + "\n";

        return allocateReg;
    }
    if (instanceof <OperatorNode *>(op))
    {
        OperatorNode *pd = dynamic_cast<OperatorNode *>(op);
        type t = pd->token->id;
        string resultReg = allocate_register();
        if (t == type::ADDITION)
        {
            string left = gen_ARM_operators(op->left);

            string right = gen_ARM_operators(op->right);
            global_string1 += "ADD " + resultReg + "," + left + ", " + right + " \n";
            free_reg();
            free_reg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            string left = gen_ARM_operators(op->left);

            string right = gen_ARM_operators(op->right);
            global_string1 += "SUB " + resultReg + "," + left + ", " + right + " \n";
            free_reg();
            free_reg();
            return resultReg;
        }

        if (t == type::MULTIPLY)
        {
            string left = gen_ARM_operators(op->left);

            string right = gen_ARM_operators(op->right);
            global_string1 += "MUL " + resultReg + "," + left + ", " + right + " \n";
            free_reg();
            free_reg();
            return resultReg;
        }
    }

    return "";
}
/**
 * @brief writes to file
 *
 * @param outfile
 * @param word
 */
void wfARM(ofstream &outfile, string word)
{
    outfile << word << endl;
}
void gen_ARM_target(Node *op, string filename = "")
{

    string dirname = "ARMTarget/ARMTargetOut";
    int status = fs::create_directories(dirname);

    if (filename == "")
    {
        filename = "out.s";
    }
    ofstream outfile(dirname + "/" + filename);
    gen_ARM_operators(op);
    wfARM(outfile, global_string1);

    // string word = "section .data \n format  db \"%d\", 10, 0 \n section .text \n extern printf \n \t global main \n main: \n ";
    // wfx86(outfile, word);
    // vector<string> tab;
    // string resultReg = gen_opertorsx86(op, tab);

    // string start = "\t push rbp \n\t mov rax,0 ";
    // wfx86(outfile, start);
    // wfx86(outfile, global_string1);
    // string printans = "mov rdi,format \n mov esi, " + resultReg + "\n xor " + resultReg + "," + resultReg + "\n call printf\n";
    // wfx86(outfile, printans);
    // wfx86(outfile, "\n \t pop rbp \n \t ret");
    // // mov        rax,0        ; Exit code 0
    // //  pop        rbp          ; Pop stack
    // //  ret
    outfile.close();
}