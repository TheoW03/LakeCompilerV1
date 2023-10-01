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

string handle_boolean(Node *op, vector<Scope_dimension *> &scope, string &global_string, int isLoop = 0)
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
        Varaible *var = get_varaible(pd, scope);
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
        if (pd->op->id == type::BOOL_EQ)
        {
            string resultReg = allocateReg();
            if (isLoop == 1)
            {
                global_string += "beq " + handle_boolean(pd->right, scope, global_string, isLoop) + " ," + handle_boolean(pd->left, scope, global_string, isLoop) + " , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "bne " + handle_boolean(pd->right, scope, global_string) + " ," + handle_boolean(pd->left, scope, global_string) + " , L" + to_string(nOfBranch) + "\n";
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
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope, global_string, isLoop) + " ," + handle_boolean(pd->left, scope, global_string, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope, global_string) + " ," + handle_boolean(pd->left, scope, global_string) + "\n";
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
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope, global_string, isLoop) + " ," + handle_boolean(pd->left, scope, global_string, isLoop) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope, global_string) + " ," + handle_boolean(pd->left, scope, global_string) + "\n";
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
                global_string += "slt " + resultReg + "," + handle_boolean(pd->left, scope, global_string, isLoop) + " ," + handle_boolean(pd->right, scope, global_string, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->left, scope, global_string) + " ," + handle_boolean(pd->right, scope, global_string) + "\n";
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
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope, global_string, isLoop) + " ," + handle_boolean(pd->left, scope, global_string, isLoop) + "\n";
                global_string += "beq " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }
            else
            {
                global_string += "slt " + resultReg + "," + handle_boolean(pd->right, scope, global_string) + " ," + handle_boolean(pd->left, scope, global_string) + "\n";
                global_string += "bne " + resultReg + " ,$zero , L" + to_string(nOfBranch) + "\n";
                global_string += "nop \n";
            }

            return "";
        }
    }
    return "";
}

string gen_float_op(Node *op, vector<Scope_dimension *> &scope, string &global_string)
{
    if (op == nullptr)
    {
        cout << "null \n";
        return "";
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
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        string reg = allocateReg();
        global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }

    // varaibleNode *pd1 = dynamic_cast<varaibleNode *>(op);
    if (instanceof <VaraibleReference *>(op))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);
        // type a = map[pd1->varailbe->buffer]->varType->id;
        Varaible *var = get_varaible(pd, scope);

        if (var == nullptr)
        {
            cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
            exit(0);
            return "";
        }
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
    if (instanceof <OperatorNode *>(op))
    {
        cout << "is in op node \n";
        OperatorNode *pd = dynamic_cast<OperatorNode *>(op); // downcast
        type t = pd->token->id;

        string resultReg = allocateReg();
        if (t == type::ADDITION)
        {
            // return
            string left = gen_float_op(op->left, scope, global_string);

            string right = gen_float_op(op->right, scope, global_string);
            global_string += "add " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            // return
            string left = gen_float_op(op->left, scope, global_string);
            cout << "sub";

            string right = gen_float_op(op->right, scope, global_string);

            global_string += "sub " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MULTIPLY)
        {
            // return
            string left = gen_float_op(op->left, scope, global_string);
            string right = gen_float_op(op->right, scope, global_string);
            global_string += "mult " + left + ", " + right + " \n";
            global_string += "mflo " + resultReg + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::DIVISION)
        {
            // return
            string left = gen_float_op(op->left, scope, global_string);
            string right = gen_float_op(op->right, scope, global_string);
            global_string += "div " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MOD)
        {

            string left = gen_float_op(op->left, scope, global_string);
            string right = gen_float_op(op->right, scope, global_string);
            global_string += "div " + resultReg + "," + left + ", " + right + " \n";
            global_string += "mfhi " + resultReg + "\n";
            freeReg();
            freeReg();
            return resultReg;
            //           div $t4,$v0, $t3
            // div $t4, $t3, 10
            // mfhi $t3
        }
    }
    return "";
}

string gen_integer_op(Node *op, vector<Scope_dimension *> &scope, string &global_string)
{

    if (op == nullptr)
    {
        cout << "null \n";
        return "";
    }

    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
        cout << "works in num \n";

        string reg = allocateReg();
        global_string += "li " + reg + "," + pd->num + "\n";
        return reg;
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        string reg = allocateReg();
        float fixedpoint = (float)stoi(pd->num) / OFFSET;
        int fp2 = (int)fixedpoint;
        string num = to_string(fp2);
        global_string += "li " + reg + "," + num + "\n";
        return reg;
    }

    if (instanceof <VaraibleReference *>(op))
    {
        VaraibleReference *pd = dynamic_cast<VaraibleReference *>(op);

        cout << "works in var \n";
        string reg = allocateReg();
        Varaible *var = get_varaible(pd, scope);
        
        if (var == nullptr)
        {
            cerr << pd->varaible->buffer + " doesnt exist as a var" << endl;
            exit(0);
            return "";
        }
        else
        {
            // cout << "out: " << map[pd->varailbe->buffer] << endl;

            if (var->varType->id == type::FLOAT)
            {
                string reg = allocateReg();
                string resultReg = allocateReg();

                global_string += "lw " + reg + "," + to_string(var->stackNum) + "($sp) \n";

                global_string += "div " + reg + "," + reg + ", " + to_string(OFFSET) + " \n"; // scaling. I forgot i worked on this lmao :')
                cout << "" << endl;
                cout << global_string << endl;
                freeReg();
                return reg;
            }
            else
            {

                string reg = allocateReg();
                global_string += "lw " + reg + "," + to_string(var->stackNum) + "($sp) \n";
                return reg;
            }
        }
    }
    if (instanceof <OperatorNode *>(op))
    {
        cout << "is in op node \n";
        OperatorNode *pd = dynamic_cast<OperatorNode *>(op); // downcast
        type t;
        if (pd->token != nullptr)
        {
            t = pd->token->id;
            cout << "token op: " + pd->token->buffer << endl;
        }
        string resultReg = allocateReg();
        if (t == type::ADDITION)
        {
            // return
            string left = gen_integer_op(op->left, scope, global_string);
            string right = gen_integer_op(op->right, scope, global_string);
            global_string += "add " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::SUBTRACT)
        {
            // return
            string left = gen_integer_op(op->left, scope, global_string);
            string right = gen_integer_op(op->right, scope, global_string);
            global_string += "sub " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MULTIPLY)
        {
            // return
            string left = gen_integer_op(op->left, scope, global_string);
            string right = gen_integer_op(op->right, scope, global_string);
            global_string += "mult " + left + ", " + right + " \n";
            global_string += "mflo " + resultReg + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::DIVISION)
        {
            // return
            string left = gen_integer_op(op->left, scope, global_string);
            string right = gen_integer_op(op->right, scope, global_string);
            global_string += "div " + resultReg + "," + left + ", " + right + " \n";
            freeReg();
            freeReg();
            return resultReg;
        }
        if (t == type::MOD)
        {
            string left = gen_integer_op(op->left, scope, global_string);
            string right = gen_integer_op(op->right, scope, global_string);
            global_string += "div " + resultReg + "," + left + ", " + right + " \n";
            global_string += "mfhi " + resultReg + "\n";
            freeReg();
            freeReg();
            return resultReg;
            //           div $t4,$v0, $t3
            // div $t4, $t3, 10
            // mfhi $t3
        }
    }
    return "";
}