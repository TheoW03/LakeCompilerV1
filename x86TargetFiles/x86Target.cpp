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


string gen_opertorsx86(Node *op, vector<string> &tabs){

}
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
    
    ofstream outfile("x86TargetFiles/x86TargetASM/"+filename);
    string word = "section .data \n section .text \n \t global main \n main: \n";
    wfx86(outfile, word);
    string exit = "\tpush rbp \n\t mov rax,0 \n \t pop rbp \n \t ret";
    wfx86(outfile, exit);
    // mov        rax,0        ; Exit code 0
    //  pop        rbp          ; Pop stack
    //  ret
    outfile.close();
}