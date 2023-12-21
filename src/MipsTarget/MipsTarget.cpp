#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/MipsTarget/UtilFunctions.h"

#include "../../src/CompilerFrontend/parser.h"
#include "../../src/MipsTarget/builtInFunction.h"
#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/ExpressionTree.h"
#include "../../src/MipsTarget/Register.h"

namespace fs = std::filesystem;

using namespace std;

#define OFFSET_HEXA 0x10000

Node::~Node()
{
    // implementation of the destructor goes here
}

string global_string = "";
int max_size = 0;
int stack_number = 0;

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

void gen_function(vector<shared_ptr<Node>> state, int &stackNum)
{
    if (state.size() == 0)
    {
        return;
    }

    for (size_t i = 0; i < state.size(); i++)
    {
        if (instanceof <IfSatementNode *>(state[i].get()))
        {
            IfSatementNode *pd = dynamic_cast<IfSatementNode *>(state[i].get());
            gen_function((pd->statements), stackNum);
            if (pd->Else != nullptr)
            {
                gen_function((pd->Else->statements), stackNum);
            }
        }
        else if (instanceof <LoopNode *>(state[i].get()))
        {
            LoopNode *pd = dynamic_cast<LoopNode *>(state[i].get());
            gen_function((pd->statements), stackNum);
        }
        else if (instanceof <ForLoopNode *>(state[i].get()))
        {

            ForLoopNode *pd = dynamic_cast<ForLoopNode *>(state[i].get());
            stackNum += 4;
            gen_function((pd->statements), stackNum);
        }
        else if (instanceof <VaraibleDeclaration *>(state[i].get()) 
			|| instanceof <ArrayDeclaration *>(state[i].get()))
        {
            stackNum += 4;
        }
    }
}

void statementsGen(shared_ptr<Node> statement, 
		shared_ptr<FunctionNode> function, Scope_Monitor &scope_monitor, 
		ofstream &outfile)
{
    map<type, builtInFunction *> functions;

    functions[type::PRINT] = new Print();
    functions[type::EXIT] = new Exit();

    if (instanceof <VaraibleDeclaration *>(statement.get()))
    {
        VaraibleDeclaration *pd = dynamic_cast<VaraibleDeclaration *>(statement.get());
        stack_number += 4;

        Varaible *type1 = add_to_var(pd, (scope_monitor.scope), stack_number);
        if (type1 == nullptr)
        {
            // delete pd;
            return;
        }
	
        global_string = "";
        string reg = "";
        update_var_values(type1->varType, move(pd->expression),
			global_string, reg, scope_monitor);

        global_string += "sw " + reg + "," + to_string(type1->stackNum) + "($fp) \n";
        wf(outfile, global_string);
        global_string = "";
        // delete pd;
    }
    else if (instanceof <ArrayDeclaration *>(statement.get()))
    {
        ArrayDeclaration *pd = dynamic_cast<ArrayDeclaration *>(statement.get());
        // delete pd;
    }
    else if (instanceof <VaraibleReference *>(statement.get()))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(statement.get());
        // vector<unique_ptr<Scope_dimension>> scopes = scope_monitor.scope;

        Varaible *type1 = get_varaible(pd, (scope_monitor.scope));

        if (type1 == nullptr)
        {
            cerr << pd->varaible.buffer + " doesnt exist as a var" << endl;
            exit(1);
            return;
        }
        if (type1->constant == 1)
        {
            cout << pd->varaible.buffer << " is constant" << endl;
            exit(1);
            return;
        }
        global_string = "";
        string reg = "";
        update_var_values(type1->varType, move(pd->expression), 
			global_string, reg, scope_monitor);
        global_string += "sw " + reg + "," + to_string(type1->stackNum) + "($fp) \n";
        wf(outfile, global_string);
        global_string = "";

        // delete pd;
    }
    else if (instanceof <funcCallNode *>(statement.get()))
    {
        funcCallNode *pd = dynamic_cast<funcCallNode *>(statement.get());
        if (functions.find(pd->funcCall.id) != functions.end())
        {
            vector<unique_ptr<Node>> para = move(pd->params);
            builtInFunction *func = functions[pd->funcCall.id];
            if (func != nullptr)
            {
                string gen_string = "";
                func->setup_params(move(para), gen_string, scope_monitor);
                wf(outfile, gen_string);
            }
        }
        else
        {
            vector<unique_ptr<Node>> para = move(pd->params);

            if (scope_monitor.f.find(pd->funcCall.buffer) == scope_monitor.f.end())
            {
                cerr << pd->funcCall.buffer + " is not a function" << endl;
                exit(EXIT_FAILURE);
                return;
            }
            vector<shared_ptr<VaraibleDeclaration>> param = 
		    (scope_monitor.f[pd->funcCall.buffer]->params);
            string a = scope_monitor.f[pd->funcCall.buffer]->hashed_functionName;
            global_string = "";

            handle_function_calls(move(param), 
			    move(para), 
			    scope_monitor, 
			    global_string);
            global_string += "sw $ra,4($sp) \n";
            global_string += "jal " + a + "\n";
            global_string += "lw $ra,4($sp) \n";
            global_string += "move $fp, $sp \n";
            wf(outfile, global_string);
            global_string = "";
        }
        // delete pd;
    }
    else if (instanceof <IfSatementNode *>(statement.get()))
    {
        global_string = "";
        IfSatementNode *pd = dynamic_cast<IfSatementNode *>(statement.get());
        if (pd->statements.size() == 0)
        {
            return;
        }

        handle_boolean(move(pd->condition), scope_monitor, global_string);

        reset_registers();
        wf(outfile, global_string);

        global_string = "";
        int ifBranch = getnOfBranch();
        increase_numofbranch();
        allocate_Scope((scope_monitor.scope));
        for (size_t i = 0; i < pd->statements.size(); i++)
        {

            statementsGen((pd->statements[i]), 
			    move(function), 
			    scope_monitor, 
			    outfile); // write a new function for this T~T
        }
        increase_numofbranch();
        int elseBranch = getnOfBranch();
        increase_numofbranch();
        deallocate_Scope((scope_monitor.scope));
        if (pd->Else != nullptr)
        {
            // cout << scope_monitor->scope.size() << endl;
            allocate_Scope((scope_monitor.scope));
            global_string = "j L" + to_string(elseBranch) + "#a \n";
            wf(outfile, global_string);
            global_string = "";
        }
        global_string = "L" + to_string(ifBranch) + ": \n";
        wf(outfile, global_string);
        global_string = "";
        if (pd->Else != nullptr)
        {

            vector<shared_ptr<Node>> statementsElse = pd->Else->statements;
            for (size_t i = 0; i < statementsElse.size(); i++)
            {
                statementsGen(move(statementsElse[i]), 
				move(function), 
				scope_monitor, 
				outfile);
            }
            // go through statements
            global_string = "L" + to_string(elseBranch) + ": \n";
            wf(outfile, global_string);
            global_string = "";
            deallocate_Scope((scope_monitor.scope));
        }
        // delete pd;
    }
    else if (instanceof <LoopNode *>(statement.get()))
    {

        // wip
        global_string = "";
        LoopNode *pd = dynamic_cast<LoopNode *>(statement.get());
        if (pd->statements.size() == 0)
        {
            return;
        }
        increase_numofbranch();
        int b = getnOfBranch();
        global_string += "b L" + to_string(b) + "\n";
        increase_numofbranch();    
        global_string += "L" + to_string(getnOfBranch()) +
			": \n # loop"; // condition
        wf(outfile, global_string);
        global_string = "";

        handle_boolean(move(pd->condition), scope_monitor, global_string, 1);
        reset_registers();

        string condition = global_string;
        increase_numofbranch();
        global_string = "";
        allocate_Scope((scope_monitor.scope));
        for (size_t i = 0; i < pd->statements.size(); i++)
        {
            statementsGen((pd->statements[i]), 
			    move(function), 
			    scope_monitor, 
			    outfile);
        }
        deallocate_Scope((scope_monitor.scope));
        global_string += "L" + to_string(b) + ": \n # condition";
        wf(outfile, global_string);
        wf(outfile, condition);
        global_string = "";
        // wf(outfile, global_string);
    }
    else if (instanceof <ForLoopNode *>(statement.get()))
    {
        global_string = "";
        ForLoopNode *pd = dynamic_cast<ForLoopNode *>(statement.get());
        allocate_Scope((scope_monitor.scope));
        statementsGen(move(pd->incrimentorVar), 
			move(function), 
			scope_monitor, 
			outfile); // write a new function for this T~T

        allocate_Scope((scope_monitor.scope));
        
	increase_numofbranch();
        
	int b = getnOfBranch();
        
	global_string += "b L" + to_string(b) + "\n";
        increase_numofbranch();
        global_string += "L" + to_string(getnOfBranch()) + ": \n # loop"; // condition
        wf(outfile, global_string);
        global_string = "";

         handle_boolean(move(pd->condition), scope_monitor, global_string, 1);
        string condition = global_string;
        increase_numofbranch();
        global_string = "";
        for (size_t i = 0; i < pd->statements.size(); i++)
        {

            statementsGen((pd->statements[i]), 
			    move(function), 
			    scope_monitor, 
			    outfile); // write a new function for this T~T
        }

        global_string += "L" + to_string(b) + ": \n # condition";
        wf(outfile, global_string);
        wf(outfile, condition);

        global_string = "";
        deallocate_Scope((scope_monitor.scope));
        deallocate_Scope((scope_monitor.scope));
        // delete pd;
    }
    else if (instanceof <ReturnStatment *>(statement.get()))
    {
        // Tokens *returnType = function->returnType;
        if (!function->returnType.has_value())
        {
            cout << "no return type" << endl;
            exit(1);
        }

        ReturnStatment *pd = dynamic_cast<ReturnStatment *>(statement.get());
        global_string = "";
        string reg = "";

        update_var_values(function->returnType.value(),
			move(pd->expression), 
			global_string, reg, 
			scope_monitor);
        global_string += "move $v0 ," + reg + "\n";
        global_string += "addi $sp, $sp," + to_string(max_size) + 
		" # Move the stack pointer up by " + to_string(max_size) + 
		" bytes\n  jr $ra \n";
        wf(outfile, global_string);
        global_string = "";

        // delete pd;
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
void gen_mips_target(vector<unique_ptr<FunctionNode>> op, string filename)
{
    map<string, Node *> vars;
    map<string, FunctionNode *> f;
    f["."] = nullptr;
    string dirname = "src/MipsTarget/MipsTargetASM/";
    fs::create_directories(dirname);

    if (filename == "")
    {
        filename = "out.s";
    }

    ofstream outfile(dirname + filename);
    string word = ".data \n .text \n";

    wf(outfile, word);
    for (size_t i = 0; i < op.size(); i++)
    {

        // unique_ptr<FunctionNode> ptrfunction = move(op[i]);
        f[op[i]->nameOfFunction.buffer] = op[i].get();
    }

    for (size_t i = 0; i < op.size(); i++)
    {

        shared_ptr<FunctionNode> pd = move(op[i]);

        // cout << pd->statements.size() << endl;
        vector<shared_ptr<Node>> state = (pd->statements);

        // map<string, Varaible *> map;

        vector<Scope_dimension> scope;
        allocate_Scope(scope);

        // Scope_dimension *scope_structure = new Scope_dimension;
        max_size = 44;
        stack_number = 44;
        // map["."] = 0;
        // scope_structure->vars = map;

        // where to iterate on list of vectors
        if (pd->nameOfFunction.buffer == "main")
        {
            string function_name = pd->nameOfFunction.buffer + ": \n";
            wf(outfile, function_name);
        }
        else
        {
            string function_name = pd->hashed_functionName + ": \n";
            wf(outfile, function_name);
        }

        vector<shared_ptr<VaraibleDeclaration>> params = (pd->params);
        if (pd->nameOfFunction.buffer != "main")
        {

            for (size_t i = 0; i < params.size(); i++)
            {
                max_size += 4;
            }
        }

        gen_function((state), max_size);

        vector<string> tab;
        string setupstack = "";
        if (max_size != 0)
        {
            setupstack = "addi $sp, $sp,-" + to_string(max_size) + 
		    " \n  # Move the stack pointer down by " + 
		    to_string(max_size) + " bytes\n";
        }
        else
        {
            setupstack = "addi $sp, $sp, " + to_string(max_size) + 
		    " # Move the stack pointer down by " + to_string(max_size) + 
		    " bytes\n";
        }
        setupstack += "move $fp, $sp\n";
        wf(outfile, setupstack);
        if (pd->nameOfFunction.buffer != "main")
        {
            vector<Varaible *> var;

            for (size_t i = 0; i < params.size(); i++)
            {
                stack_number += 4;
                Varaible *b = add_to_var(params[i].get(), (scope), stack_number);
                var.push_back(b);
            }
            string add;

            for (size_t i = 0; i < params.size(); i++)
            {
                add += "sw " + allocate_argumentRegister() + 
			"," + to_string(var[i]->stackNum) + "($fp) \n";
            }
            reset_arg_register();
            wf(outfile, add);
        }
        // Scope_Monitor *monitor = new Scope_Monitor;
        Scope_Monitor monitor;
        monitor.f = f;
        RegisterAllocation rg;
        rg.reset_registers();
        monitor.rg = rg;
        monitor.scope = scope;
        // monitor->scope = scope;
        // monitor->f = f;

        // monitor->rg = rg;

        shared_ptr<FunctionNode> sharedpd = move(pd);
        // cout << state.size() << endl;
        for (size_t i = 0; i < state.size(); i++)
        {
            statementsGen((state[i]), sharedpd, monitor, outfile);
        }
        if (!sharedpd->returnType.has_value())
        {
            // cout << "null ptr" << endl;
            string exitStack = "addi $sp, $sp," + to_string(max_size) + 
		    " # Move the stack pointer up by " + 
		    to_string(max_size) + 
		    " bytes\n  jr $ra \n";
            wf(outfile, exitStack);
        }

        if (sharedpd->nameOfFunction.buffer == "main")
        {
            string exitStuff = "li $v0, 10 \n syscall # exited program pop into QtSpim and it should work";
            wf(outfile, exitStuff);
        }
    }

    outfile.close();
}
