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
#include "../../src/CompilerFrontend/optimizations.h"
#include "../../src/MipsTarget/builtInFunction.h"
#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/ExpressionTree.h"

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
int gen_Expression(Node *op, string &gen_string, string &registerResult)
{
    if (op == nullptr)
    {
        return 0;
    }
    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        registerResult = "";
        return stoi(pd->num);
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        return (int)stoi(pd->num) / OFFSET;
    }
    if (instanceof <VaraibleReference *>(op))
    {
        registerResult = allocateReg();
        gen_string += "li" + registerResult + ", 10 \n";
        return 0;
    }
    if (instanceof <OperatorNode *>(op))
    {
        OperatorNode *pd = dynamic_cast<OperatorNode *>(op);
        map<type, string> operations;
        operations[type::ADDITION] = "add ";
        operations[type::SUBTRACT] = "sub ";
        operations[type::DIVISION] = "div ";
        operations[type::MOD] = "div ";
        operations[type::MULTIPLY] = "mult ";

        string registers = "";
        int a = gen_Expression(op->left, gen_string, registers);
        string registers2 = "";
        int b = gen_Expression(op->right, gen_string, registers2);
        if (operations.find(pd->token->id) != operations.end())
        {

            // return

            if (registers2 == "" && registers == "")
            {

                registerResult = allocateReg();
                string num = "";
                if (pd->token->id == type::ADDITION)
                {
                    int n = a + b;
                    num = to_string(n);
                }
                if (pd->token->id == type::SUBTRACT)
                {
                    int n = a - b;
                    num = to_string(n);
                }
                if (pd->token->id == type::DIVISION)
                {
                    if (b == 0)
                    {
                        num = to_string(a);
                    }
                    else
                    {
                        int n = a / b;
                        num = to_string(n);
                    }
                }
                if (pd->token->id == type::MOD)
                {
                    int n = a % b;
                    num = to_string(n);
                }
                if (pd->token->id == type::MULTIPLY)
                {
                    int n = a * b;
                    num = to_string(n);
                }
                gen_string += "li " + registerResult + ", " + num + "\n";
                return 0;
            }
            else
            {
                string rg = "";
                if (registers == "")
                {
                    registers = allocateReg();
                    gen_string += "li " + registers + ", " + to_string(a) + "\n";
                }
                else if (registers2 == "")
                {
                    registers2 = allocateReg();
                    gen_string += "li " + registers2 + ", " + to_string(b) + "\n";
                    /* code */
                }
                registerResult = allocateReg();
                if (pd->token->id == type::MULTIPLY)
                {
                    gen_string += operations[pd->token->id] + " " + registers + ", " + registers2 + "\n";
                    gen_string += "mflo " + registerResult + "\n";
                }
                else
                {
                    gen_string += operations[pd->token->id] + " " + registerResult + ", " + registers + ", " + registers2 + "\n";
                    if (pd->token->id == type::MOD)
                    {
                        gen_string += "mfhi " + registerResult + "\n";
                    }
                }
                return 0;
            }
        }

        return 0;
    }
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

void statementsGen(Node *statement, vector<Scope_dimension *> &scope, map<string, FunctionNode *> f, ofstream &outfile)
{
    map<type, builtInFunction *> functions;

    functions[type::PRINT] = new Print();
    functions[type::EXIT] = new Exit();
    if (instanceof <VaraibleDeclaration *>(statement))
    {
        VaraibleDeclaration *pd = dynamic_cast<VaraibleDeclaration *>(statement);
        cout << "var: " << pd->varaible->buffer << endl;

        // prepare_interptMips(pd, var, 4);
        stack_number += 4;
        Varaible *type1 = add_to_var(pd, scope, stack_number);
        cout << "var: " << pd->varaible->buffer << " " << check_if_pureExpression(pd->expression) << endl;
        // if (check_if_pureExpression(pd->expression) == 0)
        // {
        // cout << "is constant" << endl;
        // Varaible *type1 = get_varaible(pd, scope);

        // if (type1 == nullptr)
        // {
        //     return;
        // }
        // string allocr = allocateReg();
        // if (type1->varType->id == type::FLOAT)
        // {
        //     float constantF = (constant_prop_float(pd->expression));
        //     int work1 = (int)(constantF * OFFSET);
        //     string a = "li " + allocr + "," + to_string(work1) + "\n";
        //     wf(outfile, a);
        // }
        // else if (type1->varType->id == type::INT)
        // {
        //     string a = "li " + allocr + "," + to_string(constant_prop_integer(pd->expression)) + "\n";
        //     wf(outfile, a);
        // }
        // else if (type1->varType->id == type::BOOL)
        // {
        //     string a = "li " + allocr + "," + to_string(constant_prop_boolean(pd->expression)) + " \n";
        //     wf(outfile, a);
        // }
        // else if (type1->varType->id == type::CHAR)
        // {
        //     string a = "li " + allocr + "," + to_string(constant_prop_char(pd->expression)) + " \n";
        //     wf(outfile, a);
        // }
        // cout << pd->varaible->buffer << endl;
        // // cout << to_string(var[pd->varailbe->buffer]->stackNum) << endl;
        // cout << "hi \n";
        // string add = "sw " + allocr + "," + to_string(type1->stackNum) + "($sp) \n";
        // wf(outfile, add);
        // }
        // else
        // {
        cout << "isnt constant" << endl;

        // Varaible *type1 = var[pd->varailbe->buffer];

        if (type1 == nullptr)
        {
            return;
        }
        cout << "else \n";
        string add = "";
        if (type1->varType->id == type::FLOAT)
        {
            string reg = gen_float_op(pd->expression, scope, global_string);
            add += "sw " + reg + "," + to_string(type1->stackNum) + "($sp) \n";
        }
        else if (type1->varType->id == type::INT)
        {
            string reg = "";
            int b = gen_integer_op(pd->expression, scope, global_string, reg);
            if (reg != "")
            {
                add = "sw " + reg + "," + to_string(type1->stackNum) + "($sp) \n";
            }
            else
            {
                reg = allocateReg();
                add += "li" + reg + ", " + to_string(b) + " \n";
                add += "sw " + reg + "," + to_string(type1->stackNum) + "($sp) \n";
            }
            // add = "sw " + gen_integer_op(pd->expression, scope, global_string) + "," + to_string(type1->stackNum) + "($sp) \n";
        }
        else if (type1->varType->id == type::BOOL)
        {
            add = "sw " + handle_boolean(pd->expression, scope, global_string) + "," + to_string(type1->stackNum) + "($sp) \n";
        }
        else if (type1->varType->id == type::CHAR)
        {
            add = "sw " + gen_char_op(pd->expression, scope, global_string) + "," + to_string(type1->stackNum) + "($sp) \n";
        }

        wf(outfile, global_string);
        global_string = "";
        wf(outfile, add);
        // }
    }
    else if (instanceof <VaraibleReference *>(statement))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(statement);
        cout << "var: " << pd->varaible->buffer << "   " << check_if_pureExpression(pd->expression) << endl;
        // if (check_if_pureExpression(pd->expression) == 0)
        // {
        // cout << "is constant" << endl;

        // cout << "boolean" << endl;

        // Varaible *type1 = get_varaible(pd, scope);
        // if (type1 == nullptr)
        // {
        //     cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
        //     exit(1);
        //     return;
        // }
        // if (type1->constant == 1)
        // {
        //     cout << "is constant";
        //     exit(1);
        //     return;
        // }
        // string allocr = allocateReg();
        // if (type1->varType->id == type::FLOAT)
        // {
        //     float constantF = (constant_prop_float(pd->expression));
        //     int work1 = (int)(constantF * OFFSET);
        //     string a = "li " + allocr + "," + to_string(work1) + "\n";
        //     wf(outfile, a);
        // }
        // else if (type1->varType->id == type::INT)
        // {
        //     string a = "li " + allocr + "," + to_string(constant_prop_integer(pd->expression)) + "\n";
        //     wf(outfile, a);
        // }
        // else if (type1->varType->id == type::BOOL)
        // {
        //     string a = "li " + allocr + "," + to_string(constant_prop_boolean(pd->expression)) + " \n";
        //     wf(outfile, a);
        // }
        // else if ((type1->varType->id == type::CHAR))
        // {
        //     string a = "li " + allocr + "," + to_string(constant_prop_char(pd->expression)) + " \n";
        //     wf(outfile, a);
        // }
        // string add = "sw " + allocr + "," + to_string(type1->stackNum) + "($sp) \n";
        // wf(outfile, add);
        // }
        // else
        // {
        cout << "no constant" << endl;

        Varaible *type1 = get_varaible(pd, scope);

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
        string add = "";
        if (type1->varType->id == type::FLOAT)
        {
            string reg = gen_float_op(pd->expression, scope, global_string);
            add += "sw " + reg + "," + to_string(type1->stackNum) + "($sp) \n";
        }
        else if (type1->varType->id == type::INT)
        {
            string reg = "";
            int b = gen_integer_op(pd->expression, scope, global_string, reg);
            if (reg != "")
            {
                add = "sw " + reg + "," + to_string(type1->stackNum) + "($sp) \n";
            }
            else
            {
                reg = allocateReg();
                add += "li" + reg + ", " + to_string(b) + "\n";
                add += "sw " + reg + "," + to_string(type1->stackNum) + "($sp) \n";
            }
        }
        else if (type1->varType->id == type::BOOL)
        {
            add = "sw " + handle_boolean(pd->expression, scope, global_string) + "," + to_string(type1->stackNum) + "($sp) \n";
        }
        else if (type1->varType->id == type::CHAR)
        {
            add = "sw " + gen_char_op(pd->expression, scope, global_string) + "," + to_string(type1->stackNum) + "($sp) \n";
        }

        cout << "string: " + global_string << endl;
        wf(outfile, global_string);
        global_string = "";
        wf(outfile, add);
        // }
        freeReg();
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
                func->setup_params(para, gen_string, scope);
                wf(outfile, gen_string);
            }
        }
        else
        {
            vector<Node *> para = pd->params;
            vector<VaraibleDeclaration *> param = f[pd->funcCall->buffer]->params;
            for (int i = 0; i < param.size(); i++)
            {
                if (param[i]->typeOfVar->id == type::INT)
                {
                    global_string = "";
                    string reg = "";
                    gen_integer_op(para[i], scope, global_string, reg);
                    wf(outfile, global_string);
                    string a = "move " + allocate_argumentRegister() + "," + reg + "#f \n";
                    wf(outfile, a);
                    global_string = "";
                    a = "";
                }
            }
            // for (int i = 0; i < param.size(); i++)
            // {
            //     if (param[i]->typeOfVar->id == type::FLOAT)
            //     {
            //         if (check_if_pureExpression(para[i]) == 0)
            //         {
            //             string a = "li " + allocate_argumentRegister() + "," + to_string(constant_prop_float(para[i])) + "\n";
            //             wf(outfile, a);
            //             a = "";
            //         }
            //         else
            //         {
            //             global_string = "";
            //             string reg = gen_float_op(para[i], scope, global_string);
            //             wf(outfile, global_string);
            //             string a = "move " + allocate_argumentRegister() + "," + reg + "#f \n";
            //             wf(outfile, a);
            //             global_string = "";
            //             a = "";
            //         }
            //     }
            //     else if (param[i]->typeOfVar->id == type::INT)
            //     {
            //         if (check_if_pureExpression(para[i]) == 0)
            //         {
            //             string a = "li " + allocate_argumentRegister() + "," + to_string(constant_prop_integer(para[i])) + "\n";
            //             wf(outfile, a);
            //             a = "";
            //         }
            //         else
            //         {
            //             global_string = "";
            //             string reg = gen_integer_op(para[i], scope, global_string);
            //             wf(outfile, global_string);
            //             string a = "move " + allocate_argumentRegister() + "," + reg + "#f \n";
            //             wf(outfile, a);
            //             global_string = "";
            //             a = "";
            //         }
            //     }
            // }
            // reset_arg_register();
            // global_string += "sw $ra,4($sp) \n";
            // global_string += "jal " + pd->funcCall->buffer + "\n";
            // global_string += "lw $ra,4($sp) \n";
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
        handle_boolean(pd->condition, scope, global_string);

        wf(outfile, global_string);

        global_string = "";
        int ifBranch = getnOfBranch();
        increase_numofbranch();
        allocate_Scope(scope);
        for (int i = 0; i < pd->statements.size(); i++)
        {

            statementsGen(pd->statements[i], scope, f, outfile); // write a new function for this T~T
        }
        increase_numofbranch();
        int elseBranch = getnOfBranch();
        increase_numofbranch();
        deallocate_Scope(scope);
        if (pd->Else != nullptr)
        {
            allocate_Scope(scope);
            global_string = "j L" + to_string(elseBranch) + " \n";
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
                statementsGen(statementsElse[i], scope, f, outfile);
            }
            // go through statements
            global_string = "L" + to_string(elseBranch) + ": \n";
            wf(outfile, global_string);
            global_string = "";
            deallocate_Scope(scope);
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

        handle_boolean(pd->condition, scope, global_string, 1);
        string condition = global_string;
        increase_numofbranch();
        global_string = "";
        allocate_Scope(scope);
        for (int i = 0; i < pd->statements.size(); i++)
        {

            statementsGen(pd->statements[i], scope, f, outfile); // write a new function for this T~T
        }
        deallocate_Scope(scope);
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
        allocate_Scope(scope);
        cout << "size: " << scope.size() << endl;

        statementsGen(pd->incrimentorVar, scope, f, outfile); // write a new function for this T~T
        allocate_Scope(scope);
        increase_numofbranch();
        int b = getnOfBranch();
        global_string += "b L" + to_string(b) + "\n";
        increase_numofbranch();
        global_string += "L" + to_string(getnOfBranch()) + ": \n # loop"; // condition
        wf(outfile, global_string);
        global_string = "";

        handle_boolean(pd->condition, scope, global_string, 1);
        string condition = global_string;
        increase_numofbranch();
        global_string = "";
        for (int i = 0; i < pd->statements.size(); i++)
        {

            statementsGen(pd->statements[i], scope, f, outfile); // write a new function for this T~T
        }

        global_string += "L" + to_string(b) + ": \n # condition";
        wf(outfile, global_string);
        wf(outfile, condition);

        global_string = "";
        deallocate_Scope(scope);
        deallocate_Scope(scope);
    }
}

void rewrite_vars(Node *op)
{
    string filename = "";
    string dirname = "src/MipsTarget/MipsTargetASM/";
    int status = fs::create_directories(dirname);

    if (filename == "")
    {
        filename = "out.s";
    }

    ofstream outfile(dirname + filename);
    string word = ".data \n .text \n";
    wf(outfile, word);

    string function_name = "main: \n";
    wf(outfile, function_name);
    string global_string = "";
    string reg = "";
    gen_Expression(op, global_string, reg);

    wf(outfile, global_string);
    string exitStuff = "li $v0, 10 \n syscall # exited program pop into QtSpim and it should work";
    wf(outfile, exitStuff);
    outfile.close();
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
        max_size = 4;
        stack_number = 4;
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

        for (int i = 0; i < state.size(); i++)
        {
            statementsGen(state[i], scope, f, outfile);
        }
        string exitStack = "addi $sp, $sp," + to_string(max_size) + " # Move the stack pointer up by " + to_string(max_size) + " bytes\n  jr $ra \n";
        wf(outfile, exitStack);
        if (pd->nameOfFunction->buffer == "main")
        {
            string exitStuff = "li $v0, 10 \n syscall # exited program pop into QtSpim and it should work";
            wf(outfile, exitStuff);
        }
#pragma endregion // iterate function
    }
    outfile.close();
}
