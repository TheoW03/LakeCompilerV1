#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/MipsTarget/UtilFunctions.h"

#include "../../src/CompilerFrontend/parser.h"
// #include "../../src/CompilerFrontend/optimizations.h"
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

void gen_function(vector<Node *> state, int &stackNum)
{
    if (state.size() == 0)
    {
        cout << "0" << endl;
        return;
    }

    for (int i = 0; i < state.size(); i++)
    {
        if (instanceof <IfSatementNode *>(state[i]))
        {
            IfSatementNode *pd = dynamic_cast<IfSatementNode *>(state[i]);
            gen_function(pd->statements, stackNum);
        }
        else if (instanceof <LoopNode *>(state[i]))
        {
            LoopNode *pd = dynamic_cast<LoopNode *>(state[i]);
            gen_function(pd->statements, stackNum);
        }
        else if (instanceof <ForLoopNode *>(state[i]))
        {

            ForLoopNode *pd = dynamic_cast<ForLoopNode *>(state[i]);
            cout << "for" << endl;
            stackNum += 4;
            cout << stackNum << endl;

            gen_function(pd->statements, stackNum);
            cout << stackNum << endl;
        }
        else if (instanceof <VaraibleDeclaration *>(state[i]))
        {
            VaraibleDeclaration *pd = dynamic_cast<VaraibleDeclaration *>(state[i]);
            stackNum += 4;
        }
    }
}

void statementsGen(Node *statement, FunctionNode *function, Scope_Monitor *&scope_monitor, ofstream &outfile)
{
    map<type, builtInFunction *> functions;

    functions[type::PRINT] = new Print();
    functions[type::EXIT] = new Exit();

    if (instanceof <VaraibleDeclaration *>(statement))
    {
        VaraibleDeclaration *pd = dynamic_cast<VaraibleDeclaration *>(statement);
        stack_number += 4;

        Varaible *type1 = add_to_var(pd, scope_monitor->scope, stack_number);
        if (type1 == nullptr)
        {
            return;
        }
        global_string = "";
        string reg = "";
        update_var_values(type1->varType, pd->expression, global_string, reg, scope_monitor);
        global_string += "sw " + reg + "," + to_string(type1->stackNum) + "($sp) \n";
        wf(outfile, global_string);
        global_string = "";
    }
    else if (instanceof <VaraibleReference *>(statement))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(statement);
        vector<Scope_dimension *> Scope_dimension = scope_monitor->scope;

        Varaible *type1 = get_varaible(pd, Scope_dimension);

        if (type1 == nullptr)
        {
            cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
            exit(1);
            return;
        }
        if (type1->constant == 1)
        {
            cout << "err: is constant" << endl;
            exit(1);
            return;
        }
        global_string = "";
        string reg = "";
        update_var_values(type1->varType, pd->expression, global_string, reg, scope_monitor);
        global_string += "sw " + reg + "," + to_string(type1->stackNum) + "($sp) \n";
        wf(outfile, global_string);
        global_string = "";
    }
    else if (instanceof <funcCallNode *>(statement))
    {
        funcCallNode *pd = dynamic_cast<funcCallNode *>(statement);
        if (functions.find(pd->funcCall->id) != functions.end())
        {
            vector<Node *> para = pd->params;
            builtInFunction *func = functions[pd->funcCall->id];
            if (func != nullptr)
            {
                string gen_string = "";
                func->setup_params(para, gen_string, scope_monitor);
                wf(outfile, gen_string);
            }
        }
        else
        {
            vector<Node *> para = pd->params;

            if (scope_monitor->f.find(pd->funcCall->buffer) == scope_monitor->f.end())
            {
                cerr << pd->funcCall->buffer + " is not a function" << endl;
                exit(EXIT_FAILURE);
                return;
            }
            vector<VaraibleDeclaration *> param = scope_monitor->f[pd->funcCall->buffer]->params;
            global_string = "";
            handle_function_calls(param, para, scope_monitor, global_string);
            global_string += "sw $ra,4($sp) \n";
            global_string += "jal " + pd->funcCall->buffer + "\n";
            global_string += "lw $ra,4($sp) \n";
            wf(outfile, global_string);
            global_string = "";
        }
    }
    else if (instanceof <IfSatementNode *>(statement))
    {
        global_string = "";
        IfSatementNode *pd = dynamic_cast<IfSatementNode *>(statement);
        if (pd->statements.size() == 0)
        {
            return;
        }
        handle_boolean(pd->condition, scope_monitor, global_string);
        reset_registers();
        wf(outfile, global_string);

        global_string = "";
        int ifBranch = getnOfBranch();
        increase_numofbranch();
        allocate_Scope(scope_monitor->scope);
        for (int i = 0; i < pd->statements.size(); i++)
        {

            statementsGen(pd->statements[i], function, scope_monitor, outfile); // write a new function for this T~T
        }
        increase_numofbranch();
        int elseBranch = getnOfBranch();
        increase_numofbranch();
        deallocate_Scope(scope_monitor->scope);
        if (pd->Else != nullptr)
        {
            cout << scope_monitor->scope.size() << endl;
            allocate_Scope(scope_monitor->scope);
            global_string = "j L" + to_string(elseBranch) + "#a \n";
            wf(outfile, global_string);
            global_string = "";
        }
        global_string = "L" + to_string(ifBranch) + ": \n";
        wf(outfile, global_string);
        global_string = "";
        if (pd->Else != nullptr)
        {

            vector<Node *> statementsElse = pd->Else->statements;
            for (int i = 0; i < statementsElse.size(); i++)
            {
                statementsGen(statementsElse[i], function, scope_monitor, outfile);
            }
            // go through statements
            global_string = "L" + to_string(elseBranch) + ": \n";
            wf(outfile, global_string);
            global_string = "";
            deallocate_Scope(scope_monitor->scope);
        }
    }
    else if (instanceof <LoopNode *>(statement))
    {

        // wip
        global_string = "";
        LoopNode *pd = dynamic_cast<LoopNode *>(statement);
        if (pd->statements.size() == 0)
        {
            return;
        }
        increase_numofbranch();
        int b = getnOfBranch();
        global_string += "b L" + to_string(b) + "\n";
        increase_numofbranch();
        global_string += "L" + to_string(getnOfBranch()) + ": \n # loop"; // condition
        wf(outfile, global_string);
        global_string = "";

        handle_boolean(pd->condition, scope_monitor, global_string, 1);
        reset_registers();

        string condition = global_string;
        increase_numofbranch();
        global_string = "";
        allocate_Scope(scope_monitor->scope);
        for (int i = 0; i < pd->statements.size(); i++)
        {

            statementsGen(pd->statements[i], function, scope_monitor, outfile); // write a new function for this T~T
        }
        deallocate_Scope(scope_monitor->scope);
        global_string += "L" + to_string(b) + ": \n # condition";
        wf(outfile, global_string);
        wf(outfile, condition);
        global_string = "";

        // wf(outfile, global_string);
    }
    else if (instanceof <ForLoopNode *>(statement))
    {
        global_string = "";
        ForLoopNode *pd = dynamic_cast<ForLoopNode *>(statement);
        allocate_Scope(scope_monitor->scope);
        statementsGen(pd->incrimentorVar, function, scope_monitor, outfile); // write a new function for this T~T
        allocate_Scope(scope_monitor->scope);
        increase_numofbranch();
        int b = getnOfBranch();
        global_string += "b L" + to_string(b) + "\n";
        increase_numofbranch();
        global_string += "L" + to_string(getnOfBranch()) + ": \n # loop"; // condition
        wf(outfile, global_string);
        global_string = "";

        handle_boolean(pd->condition, scope_monitor, global_string, 1);
        string condition = global_string;
        increase_numofbranch();
        global_string = "";
        for (int i = 0; i < pd->statements.size(); i++)
        {

            statementsGen(pd->statements[i], function, scope_monitor, outfile); // write a new function for this T~T
        }

        global_string += "L" + to_string(b) + ": \n # condition";
        wf(outfile, global_string);
        wf(outfile, condition);

        global_string = "";
        deallocate_Scope(scope_monitor->scope);
        deallocate_Scope(scope_monitor->scope);
    }
    else if (instanceof <ReturnStatment *>(statement))
    {
        Tokens *returnType = function->returnType;
        ReturnStatment *pd = dynamic_cast<ReturnStatment *>(statement);
        global_string = "";
        string reg = "";
        if (returnType == nullptr)
        {
            cout << "no return type" << endl;
            exit(1);
        }
        update_var_values(returnType, pd->expression, global_string, reg, scope_monitor);
        global_string += "move $v0 ," + reg + "\n";
        global_string += "addi $sp, $sp," + to_string(max_size) + " # Move the stack pointer up by " + to_string(max_size) + " bytes\n  jr $ra \n";
        wf(outfile, global_string);
        global_string = "";
        
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
void gen_mips_target(vector<FunctionNode *> op, string filename)
{
    map<string, Node *> vars;
    map<type, builtInFunction *> functions;

    functions[type::PRINT] = new Print();
    functions[type::EXIT] = new Exit();

    map<string, FunctionNode *> f;
    f["."] = nullptr;

    string dirname = "src/MipsTarget/MipsTargetASM/";
    int status = fs::create_directories(dirname);

    if (filename == "")
    {
        filename = "out.s";
    }

    ofstream outfile(dirname + filename);
    string word = ".data \n .text \n";

    wf(outfile, word);
    for (int i = 0; i < op.size(); i++)
    {

        f[op[i]->nameOfFunction->buffer] = op[i];
    }

    for (int i = 0; i < op.size(); i++)
    {

        // FILE* fp = fopen("output.s", "w");
        FunctionNode *pd = dynamic_cast<FunctionNode *>(op[i]);

        vector<Node *> state = pd->statements;
        map<string, Varaible *> map;
        vector<Scope_dimension *> scope;
        Scope_dimension *l = new Scope_dimension;
        max_size = 44;
        stack_number = 44;
        map["."] = 0;
        l->vars = map;
        allocate_Scope(scope);

// where to iterate on list of vectors
#pragma region iterate vector of functions sarts here
        string function_name = pd->nameOfFunction->buffer + ": \n";
        wf(outfile, function_name);
        vector<VaraibleDeclaration *> params = pd->params;
        if (pd->nameOfFunction->buffer != "main")
        {
            for (int i = 0; i < params.size(); i++)
            {
                max_size += 4;
            }
        }
        gen_function(state, max_size);

        vector<string> tab;
        string setupstack = "";
        if (max_size != 0)
        {
            setupstack = "addi $sp, $sp,-" + to_string(max_size) + " # Move the stack pointer down by " + to_string(max_size) + " bytes\n";
        }
        else
        {
            setupstack = "addi $sp, $sp, " + to_string(max_size) + " # Move the stack pointer down by " + to_string(max_size) + " bytes\n";
        }
        wf(outfile, setupstack);
        if (pd->nameOfFunction->buffer != "main")
        {
            vector<Varaible *> var;
            for (int i = 0; i < params.size(); i++)
            {
                stack_number += 4;
                var.push_back(add_to_var(params[i], scope, stack_number));
                // prepare_interptMips(params[i], map, 4);
            }
            string add;
            for (int i = 0; i < params.size(); i++)
            {
                add += "sw " + allocate_argumentRegister() + "," + to_string(var[i]->stackNum) + "($sp) \n";
            }
            reset_arg_register();
            wf(outfile, add);
        }
        Scope_Monitor *monitor = new Scope_Monitor;
        monitor->scope = scope;
        monitor->f = f;
        RegisterAllocation *rg = new RegisterAllocation();
        rg->reset_registers();
        monitor->rg = rg;

        for (int i = 0; i < state.size(); i++)
        {
            statementsGen(state[i], pd, monitor, outfile);
        }
        if (pd->returnType == nullptr)
        {
            cout << "null ptr" << endl;
            string exitStack = "addi $sp, $sp," + to_string(max_size) + " # Move the stack pointer up by " + to_string(max_size) + " bytes\n  jr $ra \n";
            wf(outfile, exitStack);
        }

        if (pd->nameOfFunction->buffer == "main")
        {
            string exitStuff = "li $v0, 10 \n syscall # exited program pop into QtSpim and it should work";
            wf(outfile, exitStuff);
        }
#pragma endregion // iterate function
    }
    outfile.close();
}
