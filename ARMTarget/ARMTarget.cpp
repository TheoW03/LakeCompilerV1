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

#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"

using namespace std;
namespace fs = std::filesystem;

string global_string1 = "";

int currentRegister = -1;
string allocate_register()
{
    currentRegister++;
    if (currentRegister > 3)
    {
        currentRegister = 0;
    }
    cout << "add: ";
    cout << to_string(currentRegister) << endl;
    string registers[] = {"eax", "ebx", "ecx", "edx"};

    global_string1 += "xor " + registers[currentRegister] + "," + registers[currentRegister] + "\n";
    return registers[currentRegister];
}
void freeRegister()
{
    string registers[] = {"eax", "ebx", "ecx", "edx"};
    int b = currentRegister;
    if (b < 0)
    {
        b = 0;
    }
    global_string1 += "xor " + registers[b] + "," + registers[b] + "\n";
    currentRegister--;\
    if (currentRegister < 0)
    {

        currentRegister = -1;
    }
    cout << "this is from freereg";

    cout << "free: " + to_string(currentRegister) << endl;
}
string gen_opertorsx86(Node *op, vector<string> &tabs)
{
    if (op == nullptr)
    {
        return "";
    }
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

        string reg = allocate_register();
        global_string1 += "mov " + reg + "," + pd->num + "\n";
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
        string resultReg = allocate_register();
        if (t == type::ADDITION)
        {
            // return
            string left = gen_opertorsx86(op->left, tabs);
            string right = gen_opertorsx86(op->right, tabs);
            if (resultReg == left)
            {
                left = "0";
            }
            global_string1 += "add " + resultReg + "," + left + "\n";
            if (resultReg == right)
            {
                right = "0";
            }
            global_string1 += "add " + resultReg + "," + right + "\n";
            freeRegister();
            freeRegister();
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
void wfx86(ofstream &outfile, string word)
{
    outfile << word << endl;
}
void gen_x86_target(Node *op, string filename = "")
{

    string dirname = "x86TargetFiles/x86TargetASM";
    int status = fs::create_directories(dirname);

    if (filename == "")
    {
        filename = "out.s";
    }

    ofstream outfile("x86TargetFiles/x86TargetASM/" + filename);
    string word = "section .data \n format  db \"%d\", 10, 0 \n section .text \n extern printf \n \t global main \n main: \n ";
    wfx86(outfile, word);
    vector<string> tab;
    string resultReg = gen_opertorsx86(op, tab);

    string start = "\t push rbp \n\t mov rax,0 ";
    wfx86(outfile, start);
    wfx86(outfile, global_string1);
    string printans = "mov rdi,format \n mov esi, " + resultReg + "\n xor " + resultReg + "," + resultReg + "\n call printf\n";
    wfx86(outfile, printans);
    wfx86(outfile, "\n \t pop rbp \n \t ret");
    // mov        rax,0        ; Exit code 0
    //  pop        rbp          ; Pop stack
    //  ret
    outfile.close();
}