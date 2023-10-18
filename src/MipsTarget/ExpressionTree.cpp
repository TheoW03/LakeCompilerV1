#include <iostream>
#include <string>
#include <map>

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
void handle_function_calls(vector<VaraibleDeclaration *> function_params, vector<Node *> params, Scope_Monitor *&scope_monitor, string &global_string);

string gen_string(Node *op, vector<string> &tabs, vector<Scope_dimension *> &scope, string &global_string)
{
    if (op == nullptr)
    {
        cout << "null \n";
        return "";
    }

    if (instanceof <StringNode *>(op))
    {
        StringNode *pd = dynamic_cast<StringNode *>(op);
        // instanceof1<stringNode *>(op);
        // allocate size

        /*
          li $v0,9      # allocate syscall code.
        li $a0,64    # 64 bytes allocated in heap.
        syscall
        sw $v0, p   # pointer p to  heap space

        li $t0,'a'      # first byte 'a' char
        sb $t0,0($v0)
        li $t0,'b'      # second byte 'b' char
        sb $t0,1($v0)
        li $t0, 'c'
        sb $t0, 2($v0)
        li $t0, 'd'
        sb $t0, 3($v0)

        */
        // and yehs
    }
    return "";
}

string handle_boolean(Node *op, Scope_Monitor *&scope_monitor, string &global_string, int isLoop = 0)
{
    if (op == nullptr)
    {

        return "";
        // c++ moment. casually coding and bam "cout is ambigous " T~T
    }
    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        cout << "works in num \n";
        string reg = allocateReg();
        int num = stoi(pd->num) * OFFSET;
        global_string += "li " + reg + "," + to_string(num) + "\n";
        return reg;
    }
    if (instanceof <CharNode *>(op))
    {
        CharNode *pd = dynamic_cast<CharNode *>(op);
        cout << "works in num \n";
        string reg = allocateReg();
        int num = (int)stoi(pd->character) * OFFSET;
        global_string += "li " + reg + "," + to_string(num) + "\n";
        return reg;
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        string reg = allocateReg();
        global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    if (instanceof <BooleanLiteralNode *>(op))
    {
        BooleanLiteralNode *pd = dynamic_cast<BooleanLiteralNode *>(op);
        string reg = allocateReg();
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
    if (instanceof <VaraibleReference *>(op))
    {

        // VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);
        // type a = map[pd1->varailbe->buffer]->varType->id;
        Varaible *var = get_varaible(pd, scope_monitor->scope);
        if (var == nullptr)
        {
            cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
            exit(0);
            return "";
        }
        // type a = map[pd1->varailbe->buffer]->varType->id;
        if (var->varType->id == type::INT)
        {

            string reg = allocateReg();
            string reg2 = allocateReg();
            string resultReg = allocateReg();

            global_string += "lw " + reg + "," + to_string(var->stackNum) + "($sp) \n";

            global_string += "li " + reg2 + "," + to_string(OFFSET) + "\n";

            global_string += "mult " + reg + "," + reg2 + " \n";
            global_string += "mflo " + resultReg + " \n"; // scaling
            freeReg();
            freeReg();

            return resultReg;
        }
        else
        {
            string reg = allocateReg();
            global_string += "lw " + reg + "," + to_string(var->stackNum) + "($sp) \n";
            return reg;
        }
        string reg = allocateReg();
        global_string += "lw " + reg + "," + to_string(var->stackNum) + "($sp) \n";
        return reg;
    }
    if (instanceof <BoolExpressionNode *>(op))
    {
        BoolExpressionNode *pd = dynamic_cast<BoolExpressionNode *>(op);
        if (pd->op == nullptr)
        {
            if (isLoop == 1)
            {
                global_string += "j L" + to_string(nOfBranch) + "\n";
            }
            return "";
        }
        if (pd->op->id == type::BOOL_EQ)
        {
            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "beq " + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "bne " + handle_boolean(pd->right, scope_monitor, global_string) + " ," + handle_boolean(pd->left, scope_monitor, global_string) + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
        if (pd->op->id == type::GT)
        {
            if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
            {
                return "";
            }
            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string) + " ," + handle_boolean(pd->left, scope_monitor, global_string) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
        if (pd->op->id == type::LT)
        {
            if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
            {
                return "";
            }
            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string) + " ," + handle_boolean(pd->left, scope_monitor, global_string) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
        if (pd->op->id == type::LTE)
        {
            if (instanceof <BooleanLiteralNode *>(pd->left) || instanceof <BooleanLiteralNode *>(pd->right))
            {
                return "";
            }

            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->left, scope_monitor, global_string) + " ," + handle_boolean(pd->right, scope_monitor, global_string) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
        if (pd->op->id == type::GTE)
        {

            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string, isLoop) + " ," + handle_boolean(pd->left, scope_monitor, global_string, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope_monitor, global_string) + " ," + handle_boolean(pd->left, scope_monitor, global_string) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
    }
}

float gen_float_op(Node *op, Scope_Monitor *&scope_monitor, string &global_string, string &register_result)
{

    if (op == nullptr)
    {
        return 0.0f;
    }
    if (instanceof <BooleanLiteralNode *>(op))
    {
        cout << "boolean literal not accepted in integer, use 1 or 0" << endl;
        exit(EXIT_FAILURE);
    }
    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        register_result = "";
        int num = stoi(pd->num) * OFFSET;
        return num;
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        register_result = "";
        return stoi(pd->num);
    }
    if (instanceof <CharNode *>(op))
    {
        CharNode *pd = dynamic_cast<CharNode *>(op);
        string reg = allocateReg();
        int ch = stoi(pd->character);
        int num = (int)ch * OFFSET;
        register_result = "";
        return num;
    }
    if (instanceof <VaraibleReference *>(op))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);
        Varaible *var = get_varaible(pd, scope_monitor->scope);
        if (var == nullptr)
        {
            cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
            exit(0);
            return -1;
        }
        if (var->varType->id == type::INT)
        {
            register_result = allocateReg();
            string reg = allocateReg();
            string reg2 = allocateReg();
            global_string += "lw " + reg + "," + to_string(var->stackNum) + "($sp) \n";

            global_string += "li " + reg2 + "," + to_string(OFFSET) + "\n";

            global_string += "mult " + reg + "," + reg2 + " \n";
            global_string += "mflo " + register_result + " \n"; // scaling
            freeReg();
            freeReg();
        }
        else
        {
            register_result = allocateReg();
            global_string += "lw " + register_result + "," + to_string(var->stackNum) + "($sp) \n";
        }

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
        int a = gen_float_op(op->left, scope_monitor, global_string, registers);
        string registers2 = "";
        int b = gen_float_op(op->right, scope_monitor, global_string, registers2);
        if (registers2 == "" && registers == "")
        {
            if (pd->token->id == type::ADDITION)
            {
                int n = a + b;
                return n;
            }
            if (pd->token->id == type::SUBTRACT)
            {
                int n = a - b;
                return n;
            }
            if (pd->token->id == type::DIVISION)
            {
                if (b == 0)
                {
                    return a;
                }
                int n = a / b;
                return n;
            }
            if (pd->token->id == type::MOD)
            {
                int n = a % b;
                return n;
            }
            if (pd->token->id == type::MULTIPLY)
            {
                int n = a * b;
                return n;
            }
        }
        else
        {
            if (registers == "")
            {
                registers = allocateReg();
                global_string += "li " + registers + ", " + to_string(a) + "\n";
                register_result = registers;
            }
            else if (registers2 == "")
            {
                registers2 = allocateReg();
                global_string += "li " + registers2 + ", " + to_string(b) + "\n";
                register_result = registers2;
            }
            if (operations.find(pd->token->id) != operations.end())
            {
                register_result = allocateReg();
                if (pd->token->id == type::MULTIPLY)
                {
                    global_string += operations[pd->token->id] + " " + registers + ", " + registers2 + "\n";
                    global_string += "mflo " + register_result + "\n";
                }
                else
                {
                    global_string += operations[pd->token->id] + " " + register_result + ", " + registers + ", " + registers2 + "\n";
                    if (pd->token->id == type::MOD)
                    {
                        global_string += "mfhi " + register_result + "\n";
                    }
                }
                return 0;
            }
        }
    }
}

string gen_char_op(Node *op, Scope_Monitor *&scope_monitor, string &global_string)
{
    if (op == nullptr)
    {
        cout << "null \n";
        return "";
    }
    if (instanceof <BooleanLiteralNode *>(op))
    {
        cout << "boolean literal not accepted in integer, use 1 or 0" << endl;
        exit(EXIT_FAILURE);
    }
    if (instanceof <CharNode *>(op))
    {
        CharNode *pd = dynamic_cast<CharNode *>(op);
        string reg = allocateReg();
        global_string += "li " + reg + "," + pd->character + "\n";
        return reg;
    }
    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        string reg = allocateReg();
        if (stoi(pd->num) > 255)
        {
            cerr << "char only accepts an 8 bit number" << endl;
            exit(0);
            return "";
        }
        global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    if (instanceof <FloatNode *>(op))
    {
        cerr << "char only accepts an 8 bit unsigned integer or letter not floats" << endl;
        exit(1);
        return "";
    }
    if (instanceof <VaraibleReference *>(op))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);

        cout << "works in var \n";
        string reg = scope_monitor->rg->allocate_register(0);
        Varaible *var = get_varaible(pd, scope_monitor->scope);

        if (var == nullptr)
        {
            cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
            exit(0);
            return "";
        }
        if (var->varType->id == type::FLOAT)
        {
            cerr << pd->varaible->buffer + " must be an integer" << endl;
            exit(0);
            return "";
        }
        global_string += "lw " + reg + "," + to_string(var->stackNum) + "($sp) \n";

        return reg;
    }
    return "";
}

int gen_integer_op(Node *op, Scope_Monitor *&scope_monitor, string &global_string, string &register_result)
{

    if (op == nullptr)
    {
        cout << "null \n";
        return 0;
    }
    if (instanceof <BooleanLiteralNode *>(op))
    {
        cout << "boolean literal not accepted in integer, use 1 or 0" << endl;
        exit(EXIT_FAILURE);
    }
    if (instanceof <funcCallNode *>(op))
    {
        funcCallNode *pd = dynamic_cast<funcCallNode *>(op);
        // register_result = "$v0";

        if (scope_monitor->f.find(pd->funcCall->buffer) == scope_monitor->f.end())
        {
            cerr << pd->funcCall->buffer + " is not a function" << endl;
            exit(EXIT_FAILURE);
            return 0;
        }

        vector<VaraibleDeclaration *> param = scope_monitor->f[pd->funcCall->buffer]->params;
        cout << pd->funcCall->buffer << endl;

        handle_function_calls(param, pd->params, scope_monitor, global_string);

        // int a = save_registers(global_string);
        scope_monitor->rg->send_save(global_string);
        global_string += "sw $ra,4($sp) \n";

        global_string += "jal " + pd->funcCall->buffer + "\n";

        global_string += "lw $ra,4($sp) \n";
        scope_monitor->rg->return_save(global_string);
        // bring_saveBack(global_string, a);
        register_result = scope_monitor->rg->allocate_register(1);
        global_string += "move " + register_result + ", $v0 \n";
        return 0;
    }
    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        register_result = "";
        return stoi(pd->num);
    }
    if (instanceof <CharNode *>(op))
    {
        CharNode *pd = dynamic_cast<CharNode *>(op);
        register_result = "";
        return (int)stoi(pd->character);
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        register_result = "";
        return (int)stoi(pd->num) / OFFSET;
    }
    if (instanceof <VaraibleReference *>(op))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);
        Varaible *var = get_varaible(pd, scope_monitor->scope);
        if (var == nullptr)
        {
            cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
            exit(0);
            return -1;
        }

        if (var->varType->id == type::FLOAT)
        {

            // string reg = allocateReg();
            string reg = scope_monitor->rg->allocate_register(0);
            register_result = scope_monitor->rg->allocate_register(1);
            global_string += "lw " + reg + "," + to_string(var->stackNum) + "($sp) \n";
            global_string += "div " + register_result + "," + reg + ", " + to_string(OFFSET) + " #is not float \n"; // scaling. I forgot i worked on this lmao :')
            freeReg();
        }
        else
        {
            register_result = scope_monitor->rg->allocate_register(1);
            global_string += "lw " + register_result + "," + to_string(var->stackNum) + "($sp) #float \n";
        }

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
        string registers2 = "";
        int a, b;
        if (instanceof <funcCallNode *>(op->right) && (instanceof <VaraibleReference *>(op->left)))
        {
            b = gen_integer_op(op->right, scope_monitor, global_string, registers2);
            a = gen_integer_op(op->left, scope_monitor, global_string, registers);
            cout << "left" << endl;
        }
        else
        {
            a = gen_integer_op(op->left, scope_monitor, global_string, registers);
            b = gen_integer_op(op->right, scope_monitor, global_string, registers2);
        }

        if (registers2 == "" && registers == "")
        {

            // register_result = allocateReg();
            string num = "";
            if (pd->token->id == type::ADDITION)
            {
                int n = a + b;
                return n;
            }
            if (pd->token->id == type::SUBTRACT)
            {
                int n = a - b;
                return n;
            }
            if (pd->token->id == type::DIVISION)
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
            if (pd->token->id == type::MOD)
            {
                int n = a % b;
                return n;
            }
            if (pd->token->id == type::MULTIPLY)
            {
                int n = a * b;
                return n;
            }
        }
        else
        {
            if (registers == "")
            {
                // registers = allocateReg();
                registers = scope_monitor->rg->allocate_register(1);
                global_string += "li " + registers + ", " + to_string(a) + "#5\n ";
                register_result = registers;
            }
            if (registers2 == "")
            {
                registers2 = scope_monitor->rg->allocate_register(1);
                global_string += "li " + registers2 + ", " + to_string(b) + "#4\n ";
                register_result = registers2;
            }
            if (operations.find(pd->token->id) != operations.end())
            {
                // register_result = allocateReg();
                register_result = scope_monitor->rg->allocate_register(1);
                if (pd->token->id == type::MULTIPLY)
                {
                    global_string += operations[pd->token->id] + " " + registers + ", " + registers2 + "\n";
                    global_string += "mflo " + register_result + "\n";
                }
                else
                {
                    global_string += operations[pd->token->id] + " " + register_result + ", " + registers + ", " + registers2 + "\n";
                    if (pd->token->id == type::MOD)
                    {
                        global_string += "mfhi " + register_result + "\n";
                    }
                }
                scope_monitor->rg->downgrade_register(registers);
                scope_monitor->rg->downgrade_register(registers2);

                return 0;
            }
        }
        // freeReg();
        // freeReg();
        return 0;
    }
    return 0;
}
void handle_function_calls(vector<VaraibleDeclaration *> function_params, vector<Node *> params, Scope_Monitor *&scope_monitor, string &global_string)
{

    for (int i = 0; i < function_params.size(); i++)
    {
        if (function_params[i]->typeOfVar->id == type::INT)
        {
            string reg = "";
            int c = gen_integer_op(params[i], scope_monitor, global_string, reg);
            if (reg == "")
            {
                reg = allocateReg();
                global_string += "li " + reg + ", " + to_string(c) + "\n";
            }
            global_string += "move " + allocate_argumentRegister() + "," + reg + " \n";
            scope_monitor->rg->downgrade_register(reg);
        }
        else if (function_params[i]->typeOfVar->id == type::FLOAT)
        {
            string reg = "";
            int c = gen_float_op(params[i], scope_monitor, global_string, reg);
            if (reg == "")
            {
                reg = allocateReg();
                global_string += "li " + reg + ", " + to_string(c) + " \n";
            }
            global_string += "move " + allocate_argumentRegister() + "," + reg + "#f \n";
            scope_monitor->rg->downgrade_register(reg);
        }
        else if (function_params[i]->typeOfVar->id == type::CHAR)
        {
            string reg = gen_char_op(params[i], scope_monitor, global_string);
            global_string += "move " + allocate_argumentRegister() + "," + reg + "#f \n";
            scope_monitor->rg->downgrade_register(reg);
        }
    }
    reset_arg_register();
}

void update_var_values(Tokens *type, Node *expression, string &global_string, string &reg, Scope_Monitor *&scope_monitor)
{
    if (type->id == type::FLOAT)
    {
        int b = gen_float_op(expression, scope_monitor, global_string, reg);
        if (reg == "")
        {
            reg = allocateReg();
            global_string += "li" + reg + ", " + to_string(b) + " \n";
        }
    }
    else if (type->id == type::INT)
    {
        int b = gen_integer_op(expression, scope_monitor, global_string, reg);
        cout << "here" << endl;
        if (reg == "")
        {
            // reg = allocateReg();
            reg = scope_monitor->rg->allocate_register(0);
            global_string += "li" + reg + ", " + to_string(b) + " \n";
        }
    }
    else if (type->id == type::BOOL)
    {
        reg = handle_boolean(expression, scope_monitor, global_string);
    }
    else if (type->id == type::CHAR)
    {
        reg = gen_char_op(expression, scope_monitor, global_string);
    }
    scope_monitor->rg->reset_registers();
}