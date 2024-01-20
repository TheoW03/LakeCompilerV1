#include <iostream>
#include <string>
#include <vector>
#include "src/MipsTarget/MipsTarget.h"
#include "src/CompilerFrontend/Lexxer.h"
#include "src/CompilerFrontend/parser.h"
#include <cmath>
#include <functional>
#include <filesystem>

#include "src/LLVMBackend/LLVMTarget.h"
using namespace std;

namespace fs = std::filesystem;

#pragma endregion

void handle_cli(char *argv[], int argc)
{
	if (argc == 1)
	{
		return;
	}
	char *file = argv[1];

	vector<Tokens> a = lex(readFile(file));
	cout << "lexxed sucessfully" << endl;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-token") == 0)
		{
			printList(a);
		}
	}

	vector<unique_ptr<FunctionNode>> c = move(parse(a));

	cout << "parsed successfully" << endl;
	string file1 = "";
	for (int i = 0; i < argc; i++)
	{

		if (strcmp(argv[i], "-o") == 0)
		{
			file1 = argv[i + 1];
		}
	}
	gen_mips_target(move(c), file1);

	cout << "compiled successfully" << endl;
}

void llvmtest()
{
	vector<Tokens> a = lex(readFile("test.lk"));
	// printList(a);
	vector<unique_ptr<FunctionNode>> c = move(parse(a));
	gen_LLVM(move(c), "");
}
int branchnym = 0;
int registers = -1;
string allocate_reg()
{
	registers++;
	if (registers >= 10)
	{
		registers = 0;
	}
	return "$t" + to_string(registers);
}
string false_branch = "";
string true_branch = "";
int num_ors = 0;

// expiremental
string traverse(unique_ptr<Node> op, string &global_string, int isLoop = 0)
{
	if (op == nullptr)
	{
		return "";
	}
	if (instanceof <IntegerNode *>(op.get()))
	{
		IntegerNode *pd = dynamic_cast<IntegerNode *>(op.get());
		string reg = allocate_reg();
		int num = stoi(pd->num) * OFFSET;
		global_string += "li " + reg + "," + to_string(num) + "\n";
		return reg;
	}
	if (instanceof <FloatNode *>(op.get()))
	{
		FloatNode *pd = dynamic_cast<FloatNode *>(op.get());
		string reg = allocate_reg();
		int num = stoi(pd->num);
		global_string += "li " + reg + "," + to_string(num) + "\n";
		return reg;
	}
	if (instanceof <BoolExpressionNode *>(op.get()))
	{
		BoolExpressionNode *pd = dynamic_cast<BoolExpressionNode *>(op.get());
		false_branch = to_string(branchnym + 1);
		true_branch = to_string(branchnym);
		if (pd->op.value().id == type::OR)
		{

			string reg0 = traverse(move(pd->left), global_string, !isLoop);
			string reg1 = traverse(move(pd->right), global_string);
			return "";
		}
		else if (pd->op.value().id == type::AND)
		{

			string reg0 = traverse(move(pd->left), global_string);
			string reg1 = traverse(move(pd->right), global_string);
			return "";
		}

		string reg0 = traverse(move(pd->left), global_string);
		string reg1 = traverse(move(pd->right), global_string);
		if (pd->op.value().id == type::BOOL_EQ)
		{
			if (isLoop == 1)
			{
				global_string += "beq " + reg0 + "," + reg1 + "," + "L" + true_branch + "\n";
			}
			else
			{
				global_string += "bne " + reg0 + "," + reg1 + "," + "L" + false_branch + "\n";
			}

			return "";
		}
	}
	return "";
}

int main(int argc, char *argv[])
{
	llvmtest();
	// handle_cli(argv, argc);
	
	//  vector<Tokens> a = lex(readFile("test.lk"));
	// 	unique_ptr<Node> b = test(a);
	//	string global_string = ".data \n .text \n main: \n";
	//	 traverse(move(b), global_string);
	//	  string dirname = "src/MipsTarget/MipsTargetASM/";
	//    fs::create_directories(dirname);
	//
	//        string filename = "out.s";
	//
	//
	//    ofstream outfile(dirname + filename);
	//    global_string += "li $v0,10 \n syscall ";
	//		cout<<"numors: " << num_ors << endl;
	//    cout << global_string << endl;
	//    outfile.close();

	// gen_LLVM();
	return 0;
}
