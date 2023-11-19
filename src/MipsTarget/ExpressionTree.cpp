#include <iostream>
#include <string>
#include <map>
#include <optional>
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"

#include "../../src/MipsTarget/VaraibleScope.h"
#include "../../src/MipsTarget/UtilFunctions.h"

using namespace std;

int nOfBranch = 0;
void increase_numofbranch()
{
    nOfBranch++;
}
int getnOfBranch()
{
    return nOfBranch;
}
void handle_function_calls(vector<VaraibleDeclaration *> function_params, vector<unique_ptr<Node>> params, Scope_Monitor &scope_monitor, string &global_string);

string gen_string(unique_ptr<Node> op, vector<string> &tabs, vector<Scope_dimension *> &scope, string &global_string)
{
    if (op == nullptr)
    {
        // cout << "null \n";
        return "";
    }

    // if (instanceof <StringNode *>(op))
    // {
    //     StringNode *pd = dynamic_cast<StringNode *>(op);
    //     // instanceof1<stringNode *>(op);
    //     // allocate size

    //     /*
    //       li $v0,9      # allocate syscall code.
    //     li $a0,64    # 64 bytes allocated in heap.
    //     syscall
    //     sw $v0, p   # pointer p to  heap space

    //     li $t0,'a'      # first byte 'a' char
    //     sb $t0,0($v0)
    //     li $t0,'b'      # second byte 'b' char
    //     sb $t0,1($v0)
    //     li $t0, 'c'
    //     sb $t0, 2($v0)
    //     li $t0, 'd'
    //     sb $t0, 3($v0)

    //     */
    //     // and yehs
    // }
    return "";
}

string handle_boolean(unique_ptr<Node> op, Scope_Monitor &scope_monitor, string &global_string, int isLoop = 0)
{
    if (op == nullptr)
    {

        return "";
        // c++ moment. casually coding and bam "cout is ambigous " T~T
    }
    if (instanceof <funcCallNode *>(op.get()))
    {
        funcCallNode *pd = dynamic_cast<funcCallNode *>(op.get());
        if (scope_monitor.f.find(pd->funcCall.buffer) == scope_monitor.f.end())
        {
            cerr << pd->funcCall.buffer + " is not a function" << endl;
            exit(EXIT_FAILURE);
            return 0;
        }

        FunctionNode *function = scope_monitor.f[pd->funcCall.buffer];
        vector<VaraibleDeclaration *> called_params = function->params;
        // vector<unique_ptr<Node>> a = pd->params;

        handle_function_calls(called_params, move(pd->params), scope_monitor, global_string);
        scope_monitor.rg.send_save(global_string);
        global_string += "sw $ra,4($sp) \n";

        global_string += "jal " + function->nameOfFunction.buffer + "\n";

        global_string += "lw $ra,4($sp) \n";
        scope_monitor.rg.return_save(global_string);
        string register_result = scope_monitor.rg.allocate_register(1);
        Tokens returnType = function->returnType.value();
        if (returnType.id == type::INT || returnType.id == type::CHAR)
        {
            string reg = scope_monitor.rg.allocate_register(0);
            global_string += "li " + reg + "," + to_string(OFFSET) + "\n";
            global_string += "mult " + reg + "," + "$v0" + " \n";
            global_string += "mflo " + register_result + " \n"; // scaling
        }
        else
        {
            global_string += "move " + register_result + ", $v0 \n";
        }
        return register_result;
    }
    if (instanceof <IntegerNode *>(op.get()))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op.get());
        // cout << "works in num \n";
        string reg = scope_monitor.rg.allocate_register(1);
        // string reg = allocateReg();
        int num = stoi(pd->num) * OFFSET;
        global_string += "li " + reg + "," + to_string(num) + "\n";
        return reg;
    }
    if (instanceof <CharNode *>(op.get()))
    {
        CharNode *pd = dynamic_cast<CharNode *>(op.get());
        // cout << "works in num \n";
        string reg = scope_monitor.rg.allocate_register(1);
        int num = (int)stoi(pd->character) * OFFSET;
        global_string += "li " + reg + "," + to_string(num) + "\n";
        return reg;
    }
    if (instanceof <FloatNode *>(op.get()))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op.get());
        string reg = scope_monitor.rg.allocate_register(1);
        global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    if (instanceof <BooleanLiteralNode *>(op.get()))
    {
        BooleanLiteralNode *pd = dynamic_cast<BooleanLiteralNode *>(op.get());
        string reg = scope_monitor.rg.allocate_register(1);
        if (pd->value->id == type::TRUE)
        {
            global_string += "li " + reg + ", 1 \n";
        }
        else
        {
            global_string += "li " + reg + ", 0 \n";
        }
        // global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    // varaibleNode *pd1 = dynamic_cast<varaibleNode *>(op);
    if (instanceof <VaraibleReference *>(op.get()))
    {

        // VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op.get());
        // type a = map[pd1->varailbe->buffer]->varType->id;
        Varaible *var = get_varaible(pd, (scope_monitor.scope));
        if (var == nullptr)
        {
            cerr << pd->varaible.buffer + " doesnt exist as a var" << endl;
            exit(0);
            return "";
        }
        // type a = map[pd1->varailbe->buffer]->varType->id;
        if (var->varType.id == type::INT)
        {

            string reg = scope_monitor.rg.allocate_register(0);
            string reg2 = scope_monitor.rg.allocate_register(0);
            string resultReg = scope_monitor.rg.allocate_register(1);
            global_string += "lw " + reg + "," + to_string(var->stackNum) + "($fp) \n";

            global_string += "li " + reg2 + "," + to_string(OFFSET) + "\n";

            global_string += "mult " + reg + "," + reg2 + " \n";
            global_string += "mflo " + resultReg + " \n"; // scalings

            return resultReg;
        }
        else
        {
            string reg = scope_monitor.rg.allocate_register(1);
            global_string += "lw " + reg + "," + to_string(var->stackNum) + "($fp) \n";
            return reg;
        }
        string reg = scope_monitor.rg.allocate_register(1);
        global_string += "lw " + reg + "," + to_string(var->stackNum) + "($fp) \n";
        return reg;
    }
    if (instanceof <BoolExpressionNode *>(op.get()))
    {
        BoolExpressionNode *pd = dynamic_cast<BoolExpressionNode *>(op.get());
        if (!pd->op.has_value())
        {
            if (isLoop == 1)
            {
                global_string += "j L" + to_string(nOfBranch) + "\n";
            }
            return "";
        }

        string register1 = "";
        string register2 = "";
        Tokens op = pd->op.value();
        if (op.id != type::BOOL_EQ)
        {
            if (instanceof <BooleanLiteralNode *>(pd->left.get()) || instanceof <BooleanLiteralNode *>(pd->right.get()))
            {
                return "";
            }
        }
        if (isLoop == 1)
        {
            register1 = handle_boolean(move(pd->right), scope_monitor, global_string, isLoop);
            register2 = handle_boolean(move(pd->right), scope_monitor, global_string, isLoop);
        }
        else
        {
            register1 = handle_boolean(move(pd->right), scope_monitor, global_string);
            register2 = handle_boolean(move(pd->right), scope_monitor, global_string);
        }

        string resultReg = scope_monitor.rg.allocate_register(0);
        if (isLoop == 1)
        {
            switch (op.id)
            {
            case type::BOOL_EQ:
                global_string += "beq " + register1 + " ," + register2 + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::GT:
                global_string += "slt " + resultReg + "," + register1 + " ," + register2 + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::LT:
                global_string += "slt " + resultReg + "," + register1 + " ," + register2 + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::LTE:
                global_string += "slt " + resultReg + "," + register2 + " ," + register1 + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                break;
            case type::GTE:
                global_string += "slt " + resultReg + "," + register1 + " ," + register2 + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::NOT_EQ:
                global_string += "bne " + register1 + " ," + register2 + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            default:
                break;
            }
        }
        else
        {
            switch (op.id)
            {
            case type::BOOL_EQ:
                global_string += "bne " + register1 + " ," + register2 + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::GT:
                global_string += "slt " + resultReg + "," + register1 + " ," + register2 + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::LT:
                global_string += "slt " + resultReg + "," + register1 + " ," + register2 + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::LTE:
                global_string += "slt " + resultReg + "," + register2 + " ," + register1 + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::GTE:
                global_string += "slt " + resultReg + "," + register1 + " ," + register2 + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            case type::NOT_EQ:
                global_string += "beq " + register1 + " ," + register2 + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
                break;
            default:
                break;
            }
        }
        return "";

        // if (pd->op->id == type::BOOL_EQ)
        // {
        //     // string resultReg = scope_monitor->rg->allocate_register(1);

        //     if (isLoop == 1)
        //     {
        //         global_string += "beq " + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + " , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }
        //     else
        //     {
        //         global_string += "bne " + handle_boolean(pd->right, scope_monitor, global_string) + " ," + handle_boolean(pd->left, scope_monitor, global_string) + " , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }

        //     return "";
        // }
        // if (pd->op->id == type::GT)
        // {
        //     if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
        //     {
        //         return "";
        //     }
        //     // string resultReg = allocateReg();
        //     if (isLoop == 1)
        //     {
        //         global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + "\n";
        //         global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }
        //     else
        //     {
        //         global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string) + " ," + handle_boolean(pd->left, scope_monitor, global_string) + "\n";
        //         global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }

        //     return "";
        // }
        // if (pd->op->id == type::LT)
        // {
        //     if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
        //     {
        //         return "";
        //     }
        //     string resultReg = allocateReg();
        //     if (isLoop == 1)
        //     {
        //         global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + "\n";
        //         global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }
        //     else
        //     {
        //         global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string) + " ," + handle_boolean(pd->left, scope_monitor, global_string) + "\n";
        //         global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }

        //     return "";
        // }
        // if (pd->op->id == type::LTE)
        // {
        //     if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
        //     {
        //         return "";
        //     }

        //     // string resultReg = allocateReg();
        //     if (isLoop == 1)
        //     {
        //         global_string += "slt " + resultReg + "," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + "\n";
        //         global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }
        //     else
        //     {
        //         global_string += "slt " + resultReg + "," + handle_boolean(pd->left, scope_monitor, global_string) + " ," + handle_boolean(pd->right, scope_monitor, global_string) + "\n";
        //         global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }

        //     return "";
        // }
        // if (pd->op->id == type::GTE)
        // {

        //     // string resultReg = allocateReg();
        //     if (isLoop == 1)
        //     {
        //         global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + "\n";
        //         global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }
        //     else
        //     {
        //         global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string) + " ," + handle_boolean(pd->left, scope_monitor, global_string) + "\n";
        //         global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
        //         global_string += "nop \n";
        //     }

        //     return "";
        // }
    }
    return "";
}

float gen_float_op(unique_ptr<Node> op, Scope_Monitor &scope_monitor, string &global_string, string &register_result)
{

    if (op == nullptr)
    {
        return 0.0f;
    }
    // if (instanceof <funcCallNode *>(op.get()))
    // {
    //     funcCallNode *pd = dynamic_cast<funcCallNode *>(op.get());
    //     register_result = "$v0";

    //     if (scope_monitor.f.find(pd->funcCall.buffer) == scope_monitor.f.end())
    //     {
    //         cerr << pd->funcCall.buffer + " is not a function" << endl;
    //         exit(EXIT_FAILURE);
    //         return 0;
    //     }
    // }
    if (instanceof <funcCallNode *>(op.get()))
    {
        funcCallNode *pd = dynamic_cast<funcCallNode *>(op.get());
        register_result = "$v0";
        if (scope_monitor.f.find(pd->funcCall.buffer) == scope_monitor.f.end())
        {
            cerr << pd->funcCall.buffer + " is not a function" << endl;
            exit(EXIT_FAILURE);
            return 0;
        }
        FunctionNode *function = scope_monitor.f[pd->funcCall.buffer];

        vector<VaraibleDeclaration *> called_params = function->params;
        vector<unique_ptr<Node>> a;
        for (int i = 0; i < pd->params.size(); i++)
        {
            a.push_back(move(pd->params[i]));
        }
        handle_function_calls(called_params, move(a), scope_monitor, global_string);
        scope_monitor.rg.send_save(global_string);
        global_string += "sw $ra,4($sp) \n";

        global_string += "jal " + function->nameOfFunction.buffer + "\n";

        global_string += "lw $ra,4($sp) \n";
        global_string += "move $fp, $sp \n";
        scope_monitor.rg.return_save(global_string);
        register_result = scope_monitor.rg.allocate_register(1);
        Tokens returnType = function->returnType.value();

        if (returnType.id == type::INT || returnType.id == type::CHAR)
        {
            string reg = scope_monitor.rg.allocate_register(0);
            global_string += "li " + reg + "," + to_string(OFFSET) + "\n";
            global_string += "mult " + reg + "," + "$v0" + " \n";
            global_string += "mflo " + register_result + " \n"; // scaling
        }
        else
        {
            global_string += "move " + register_result + ", $v0 \n";
        }
        return 0;
    }
    if (instanceof <BooleanLiteralNode *>(op.get()))
    {
        cout << "boolean literal not accepted in integer, use 1 or 0" << endl;
        exit(EXIT_FAILURE);
    }
    if (instanceof <IntegerNode *>(op.get()))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op.get());
        register_result = "";
        int num = stoi(pd->num) * OFFSET;
        return num;
    }
    if (instanceof <FloatNode *>(op.get()))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op.get());
        register_result = "";
        return stoi(pd->num);
    }
    if (instanceof <CharNode *>(op.get()))
    {
        CharNode *pd = dynamic_cast<CharNode *>(op.get());
        int ch = stoi(pd->character);
        int num = (int)ch * OFFSET;
        register_result = "";
        return num;
    }
    if (instanceof <VaraibleReference *>(op.get()))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op.get());
        Varaible *var = get_varaible(pd, scope_monitor.scope);
        if (var == nullptr)
        {
            cerr << pd->varaible.buffer + " doesnt exist as a var" << endl;
            exit(0);
            return -1;
        }
        if (var->varType.id == type::INT)
        {
            register_result = scope_monitor.rg.allocate_register(1);
            string reg = scope_monitor.rg.allocate_register(0);
            string reg2 = scope_monitor.rg.allocate_register(0);
            global_string += "lw " + reg + "," + to_string(var->stackNum) + "($fp) \n";

            global_string += "li " + reg2 + "," + to_string(OFFSET) + "\n";

            global_string += "mult " + reg + "," + reg2 + " \n";
            global_string += "mflo " + register_result + " \n"; // scaling
        }
        else
        {
            register_result = scope_monitor.rg.allocate_register(1);
            global_string += "lw " + register_result + "," + to_string(var->stackNum) + "($fp) \n";
        }

        return 0;
    }
    if (instanceof <OperatorNode *>(op.get()))
    {
        OperatorNode *pd = dynamic_cast<OperatorNode *>(op.get());
        map<type, string> operations;
        operations[type::ADDITION] = "add ";
        operations[type::SUBTRACT] = "sub ";
        operations[type::DIVISION] = "div ";
        operations[type::MOD] = "div ";
        operations[type::MULTIPLY] = "mult ";

        string registers = "";
        string registers2 = "";
        int a, b;

        a = gen_float_op(move(op->left), scope_monitor, global_string, registers);
        b = gen_float_op(move(op->right), scope_monitor, global_string, registers2);

        if (registers2 == "" && registers == "")
        {
            if (pd->token.id == type::ADDITION)
            {
                int n = a + b;
                return n;
            }
            if (pd->token.id == type::SUBTRACT)
            {
                int n = a - b;
                return n;
            }
            if (pd->token.id == type::DIVISION)
            {
                if (b == 0)
                {
                    return a;
                }
                int n = a / b;
                return n;
            }
            if (pd->token.id == type::MOD)
            {
                int n = a % b;
                return n;
            }
            if (pd->token.id == type::MULTIPLY)
            {
                int n = a * b;
                return n;
            }
        }
        else
        {
            if (registers == "")
            {
                registers = scope_monitor.rg.allocate_register(1);
                global_string += "li " + registers + ", " + to_string(a) + "\n";
                register_result = registers;
            }
            else if (registers2 == "")
            {
                registers2 = scope_monitor.rg.allocate_register(1);
                global_string += "li " + registers2 + ", " + to_string(b) + "\n";
                register_result = registers2;
            }
            if (operations.find(pd->token.id) != operations.end())
            {
                register_result = scope_monitor.rg.allocate_register(1);
                if (pd->token.id == type::MULTIPLY)
                {
                    global_string += operations[pd->token.id] + " " + registers + ", " + registers2 + "\n";
                    global_string += "mflo " + register_result + "\n";
                }
                else
                {
                    global_string += operations[pd->token.id] + " " + register_result + ", " + registers + ", " + registers2 + "\n";
                    if (pd->token.id == type::MOD)
                    {
                        global_string += "mfhi " + register_result + "\n";
                    }
                }
                scope_monitor.rg.downgrade_register(registers);
                scope_monitor.rg.downgrade_register(registers2);
                return 0;
            }
        }
    }
    return 0;
}

string gen_char_op(unique_ptr<Node> op, Scope_Monitor &scope_monitor, string &global_string)
{
    if (op == nullptr)
    {
        // cout << "null \n";
        return "";
    }
    if (instanceof <funcCallNode *>(op.get()))
    {
        funcCallNode *pd = dynamic_cast<funcCallNode *>(op.get());

        if (scope_monitor.f.find(pd->funcCall.buffer) == scope_monitor.f.end())
        {
            cerr << pd->funcCall.buffer + " is not a function" << endl;
            exit(EXIT_FAILURE);
            return 0;
        }
        FunctionNode *function = scope_monitor.f[pd->funcCall.buffer];

        vector<VaraibleDeclaration *> param = function->params;
        std::vector<Node *> called_params_raw;
        for (const auto &param : pd->params)
        {
            called_params_raw.push_back(param.get());
        }

        // handle_function_calls(param, called_params_raw, scope_monitor, global_string);
        // handle_function_calls(param, pd->params, scope_monitor, global_string);
        global_string += "sw $ra,4($sp) \n";

        global_string += "jal " + function->nameOfFunction.buffer + "\n";

        global_string += "lw $ra,4($sp) \n";
        global_string += "move $fp, $sp \n";
        Tokens returnType = function->returnType.value();
        if (returnType.id == type::FLOAT)
        {
            cout << "error: float isnt accepted here" << endl;
            exit(0);
        }
        string register_result = scope_monitor.rg.allocate_register(1);
        global_string += "move " + register_result + ", $v0 \n";
        // delete pd;
        return register_result;
    }
    if (instanceof <BooleanLiteralNode *>(op.get()))
    {
        cout << "boolean literal not accepted in integer, use 1 or 0" << endl;
        exit(EXIT_FAILURE);
    }
    if (instanceof <CharNode *>(op.get()))
    {
        CharNode *pd = dynamic_cast<CharNode *>(op.get());
        string reg = scope_monitor.rg.allocate_register(0);
        global_string += "li " + reg + "," + pd->character + "\n";
        // delete pd;
        return reg;
    }
    if (instanceof <IntegerNode *>(op.get()))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op.get());
        string reg = scope_monitor.rg.allocate_register(0);
        if (stoi(pd->num) > 255)
        {
            cerr << "char only accepts an 8 bit number" << endl;
            exit(0);
            return "";
        }
        global_string += "li " + reg + "," + pd->num + "\n";
        // delete pd;
        return reg;
    }
    if (instanceof <FloatNode *>(op.get()))
    {
        cerr << "char only accepts an 8 bit unsigned integer or letter not floats" << endl;
        exit(1);
        return "";
    }
    if (instanceof <VaraibleReference *>(op.get()))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op.get());

        string reg = scope_monitor.rg.allocate_register(0);
        Varaible *var = get_varaible(pd, (scope_monitor.scope));

        if (var == nullptr)
        {
            cerr << pd->varaible.buffer + " doesnt exist as a var" << endl;
            exit(0);
            return "";
        }
        if (var->varType.id == type::FLOAT)
        {
            cerr << pd->varaible.buffer + " must be an integer" << endl;
            exit(0);
            return "";
        }
        global_string += "lw " + reg + "," + to_string(var->stackNum) + "($fp) \n";
        delete pd;
        return reg;
    }
    return "";
}

int gen_integer_op(unique_ptr<Node> op, Scope_Monitor &scope_monitor, string &global_string, string &register_result)
{

    if (op == nullptr)
    {
        // cout << "null \n";
        return 0;
    }
    if (instanceof <BooleanLiteralNode *>(op.get()))
    {
        cout << "boolean literal not accepted in integer, use 1 or 0" << endl;
        exit(EXIT_FAILURE);
    }
    if (instanceof <funcCallNode *>(op.get()))
    {
        funcCallNode *pd = dynamic_cast<funcCallNode *>(op.get());
        // register_result = "$v0";

        if (scope_monitor.f.find(pd->funcCall.buffer) == scope_monitor.f.end())
        {
            cerr << pd->funcCall.buffer + " is not a function" << endl;
            exit(EXIT_FAILURE);
            return 0;
        }
        FunctionNode *f1 = scope_monitor.f[pd->funcCall.buffer];
        if (!f1->returnType.has_value())
        {
            cout << pd->funcCall.buffer + "no return type" << endl;
            exit(EXIT_FAILURE);
            return 0;
        }
        vector<VaraibleDeclaration *> param = f1->params;
        Tokens returnTypes = f1->returnType.value();
        handle_function_calls(param, move(pd->params), scope_monitor, global_string);
        scope_monitor.rg.send_save(global_string);
        global_string += "sw $ra,4($sp) \n";

        global_string += "jal " + f1->nameOfFunction.buffer + "\n";

        global_string += "lw $ra,4($sp) \n";
        global_string += "move $fp, $sp \n";

        scope_monitor.rg.return_save(global_string);
        // bring_saveBack(global_string, a);
        register_result = scope_monitor.rg.allocate_register(1);
        global_string += "move " + register_result + ", $v0 \n";
        if (returnTypes.id == type::FLOAT)
        {
            global_string += "div " + register_result + "," + register_result + ", " + to_string(OFFSET) + " #is not float \n"; // scaling. I forgot i worked on this lmao :')
        }
        return 0;
    }
    if (instanceof <IntegerNode *>(op.get()))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op.get());
        register_result = "";
        int a = stoi(pd->num);
        // delete pd;
        return a;
    }
    if (instanceof <CharNode *>(op.get()))
    {
        CharNode *pd = dynamic_cast<CharNode *>(op.get());
        register_result = "";
        int a = (int)stoi(pd->character);
        // delete pd;
        return a;
    }
    if (instanceof <FloatNode *>(op.get()))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op.get());
        register_result = "";
        int a = (int)stoi(pd->num) / OFFSET;
        // delete pd;
        return a;
    }
    if (instanceof <VaraibleReference *>(op.get()))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op.get());
        Varaible *var = get_varaible(pd, (scope_monitor.scope));
        if (var == nullptr)
        {
            cerr << pd->varaible.buffer + " doesnt exist as a var" << endl;
            // delete pd;
            exit(0);
            return -1;
        }

        if (var->varType.id == type::FLOAT)
        {

            // string reg = allocateReg();
            string reg = scope_monitor.rg.allocate_register(0);
            register_result = scope_monitor.rg.allocate_register(1);
            global_string += "lw " + reg + "," + to_string(var->stackNum) + "($fp) \n";
            global_string += "div " + register_result + "," + reg + ", " + to_string(OFFSET) + " #is not float \n"; // scaling. I forgot i worked on this lmao :')
            freeReg();
        }
        else
        {
            register_result = scope_monitor.rg.allocate_register(1);
            global_string += "lw " + register_result + "," + to_string(var->stackNum) + "($fp) #float \n";
        }
        // delete pd;

        return 0;
    }
    if (instanceof <OperatorNode *>(op.get()))
    {
        OperatorNode *pd = dynamic_cast<OperatorNode *>(op.get());
        map<type, string> operations;
        operations[type::ADDITION] = "add ";
        operations[type::SUBTRACT] = "sub ";
        operations[type::DIVISION] = "div ";
        operations[type::MOD] = "div ";
        operations[type::MULTIPLY] = "mult ";

        operations[type::SLL] = "sll ";
        operations[type::SRR] = "srl ";
        operations[type::B_AND] = "and ";
        operations[type::B_OR] = "or ";
        string registers = "";
        string registers2 = "";
        int a, b;
        // if (instanceof <funcCallNode *>(op->right) && (instanceof <VaraibleReference *>(op->left)))
        // {
        //     b = gen_integer_op(op->right, scope_monitor, global_string, registers2);
        //     a = gen_integer_op(op->left, scope_monitor, global_string, registers);
        //     cout << "left" << endl;
        // }
        // else
        // {
        a = gen_integer_op(move(op->left), scope_monitor, global_string, registers);
        b = gen_integer_op(move(op->right), scope_monitor, global_string, registers2);
        // }

        if (registers2 == "" && registers == "")
        {

            // register_result = allocateReg();
            string num = "";
            if (pd->token.id == type::ADDITION)
            {
                int n = a + b;
                return n;
            }
            if (pd->token.id == type::SUBTRACT)
            {
                int n = a - b;
                return n;
            }
            if (pd->token.id == type::DIVISION)
            {
                if (b == 0)
                {
                    return a;
                }
                else
                {
                    int n = a / b;
                    return n;
                }
            }
            if (pd->token.id == type::MOD)
            {
                int n = a % b;
                return n;
            }
            if (pd->token.id == type::MULTIPLY)
            {
                int n = a * b;
                return n;
            }
            if (pd->token.id == type::SLL)
            {
                int n = a >> b;
                return n;
            }
            if (pd->token.id == type::SRR)
            {
                int n = a << b;
                return n;
            }
            if (pd->token.id == type::B_AND)
            {
                int n = a & b;
                return n;
            }
            if (pd->token.id == type::B_OR)
            {
                int n = a | b;
                return n;
            }
        }
        else
        {
            if (registers == "")
            {
                // registers = allocateReg();
                registers = scope_monitor.rg.allocate_register(1);
                global_string += "li " + registers + ", " + to_string(a) + "#5\n ";
                register_result = registers;
            }
            if (registers2 == "")
            {
                registers2 = scope_monitor.rg.allocate_register(1);
                global_string += "li " + registers2 + ", " + to_string(b) + "#4\n ";
                register_result = registers2;
            }
            if (operations.find(pd->token.id) != operations.end())
            {
                // register_result = allocateReg();
                register_result = scope_monitor.rg.allocate_register(1);
                if (pd->token.id == type::MULTIPLY)
                {
                    global_string += operations[pd->token.id] + " " + registers + ", " + registers2 + "\n";
                    global_string += "mflo " + register_result + "\n";
                }
                else
                {
                    global_string += operations[pd->token.id] + " " + register_result + ", " + registers + ", " + registers2 + "\n";
                    if (pd->token.id == type::MOD)
                    {
                        global_string += "mfhi " + register_result + "\n";
                    }
                }
                scope_monitor.rg.downgrade_register(registers);
                scope_monitor.rg.downgrade_register(registers2);

                return 0;
            }
        }
        // freeReg();
        // freeReg();
        return 0;
    }
    return 0;
}
void handle_function_calls(vector<VaraibleDeclaration *> function_params, vector<unique_ptr<Node>> params, Scope_Monitor &scope_monitor, string &global_string)
{

    for (int i = 0; i < function_params.size(); i++)
    {
        if (function_params[i]->typeOfVar.id == type::INT)
        {
            string reg = "";
            int c = gen_integer_op(move(params[i]), scope_monitor, global_string, reg);
            if (reg == "")
            {
                reg = allocateReg();
                global_string += "li " + reg + ", " + to_string(c) + "\n";
            }
            global_string += "move " + allocate_argumentRegister() + "," + reg + " \n";
            scope_monitor.rg.downgrade_register(reg);
        }
        else if (function_params[i]->typeOfVar.id == type::FLOAT)
        {
            string reg = "";
            int c = gen_float_op(move(params[i]), scope_monitor, global_string, reg);
            if (reg == "")
            {
                reg = allocateReg();
                global_string += "li " + reg + ", " + to_string(c) + " \n";
            }
            global_string += "move " + allocate_argumentRegister() + "," + reg + "#f \n";
            scope_monitor.rg.downgrade_register(reg);
        }
        else if (function_params[i]->typeOfVar.id == type::CHAR)
        {
            string reg = gen_char_op(move(params[i]), scope_monitor, global_string);
            global_string += "move " + allocate_argumentRegister() + "," + reg + "#f \n";
            scope_monitor.rg.downgrade_register(reg);
        }
    }
    reset_arg_register();
}

void update_var_values(Tokens type, unique_ptr<Node> expression, string &global_string, string &reg, Scope_Monitor &scope_monitor)
{
    if (type.id == type::FLOAT)
    {
        int b = gen_float_op(move(expression), scope_monitor, global_string, reg);
        if (reg == "")
        {
            reg = allocateReg();
            global_string += "li" + reg + ", " + to_string(b) + " \n";
        }
    }
    else if (type.id == type::INT)
    {
        int b = gen_integer_op(move(expression), scope_monitor, global_string, reg);
        // cout << "here" << endl;
        if (reg == "")
        {
            // reg = allocateReg();
            reg = scope_monitor.rg.allocate_register(0);
            global_string += "li" + reg + ", " + to_string(b) + " \n";
        }
    }
    else if (type.id == type::BOOL)
    {
        reg = handle_boolean(move(expression), scope_monitor, global_string);
    }
    else if (type.id == type::CHAR)
    {
        reg = gen_char_op(move(expression), scope_monitor, global_string);
    }
    scope_monitor.rg.reset_registers();
}