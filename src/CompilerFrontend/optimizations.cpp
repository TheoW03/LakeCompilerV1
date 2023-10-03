#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include "../../src/CompilerFrontend/Lexxer.h"
#include "../../src/CompilerFrontend/parser.h"
#include "../../src/MipsTarget/UtilFunctions.h"
// #include "../compilerFrontend/parser.h"

using namespace std;

/**
 * @brief does the optimization where it has
 *
 * a = 1 + 1;
 * it checks if its "pure" else it sums it for u
 *
 * @param op
 * @return int
 */
int check_if_pureExpression(Node *op)
{
    if (op == nullptr)
    {
        return 0;
    }
    check_if_pureExpression(op->left);
    if (instanceof <VaraibleReference *>(op))
    {
        cout << "var \n";
        return 1;
    }
    check_if_pureExpression(op->right);
    return 1;
}
int constant_prop_boolean(Node *op)
{
    if (op == nullptr)
    {
        return 0;
    }
    if (instanceof <BooleanLiteralNode *>(op))
    {
        BooleanLiteralNode *pd = dynamic_cast<BooleanLiteralNode *>(op);

        if (pd->value->id == type::TRUE)
        {
            cout << "true" << endl;
            return 1;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}
float constant_prop_float(Node *op)
{

    if (op == nullptr)
    {
        return 1;
    }

    // FloatNode *pd2 = dynamic_cast<FloatNode *>(op);

    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);

        return (int)stoi(pd->num);
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        return (float)stof(pd->num) / OFFSET;
    }
    if (instanceof <OperatorNode *>(op))
    {
        OperatorNode *pd = dynamic_cast<OperatorNode *>(op); // downcast
        type t = pd->token->id;
        if (t == type::ADDITION)
        {
            return constant_prop_float(op->left) + constant_prop_float(op->right);
        }
        if (t == type::SUBTRACT)
        {
            return constant_prop_float(op->left) - constant_prop_float(op->right);
        }
        if (t == type::MULTIPLY)
        {
            return constant_prop_float(op->left) * constant_prop_float(op->right);
        }
        if (t == type::DIVISION)
        {
            return constant_prop_float(op->left) / constant_prop_float(op->right);
        }
        if (t == type::MOD)
        {
            return ((int)constant_prop_float(op->left) % (int)constant_prop_float(op->right));
        }
    }
    return 0;
}
int constant_prop_integer(Node *op)
{
    cout << "constant prop" << endl;
    if (op == nullptr)
    {
        return 1;
    }

    if (instanceof <IntegerNode *>(op))
    {
        IntegerNode *pd = dynamic_cast<IntegerNode *>(op);

        return (int)stoi(pd->num);
    }
    if (instanceof <FloatNode *>(op))
    {
        FloatNode *pd = dynamic_cast<FloatNode *>(op);
        return (int)stoi(pd->num) / OFFSET;
    }

    if (instanceof <OperatorNode *>(op))
    {
        OperatorNode *pd = dynamic_cast<OperatorNode *>(op); // downcast
        type t = pd->token->id;
        if (t == type::ADDITION)
        {
            return constant_prop_integer(op->right) + constant_prop_integer(op->left);
        }
        if (t == type::SUBTRACT)
        {
            return constant_prop_integer(op->right) - constant_prop_integer(op->left);
        }
        if (t == type::MULTIPLY)
        {
            return constant_prop_integer(op->right) * constant_prop_integer(op->left);
        }
        if (t == type::DIVISION)
        {
            return constant_prop_integer(op->right) / constant_prop_integer(op->left);
        }
        if (t == type::MOD)
        {
            return ((int)constant_prop_integer(op->right) % (int)constant_prop_integer(op->left));
        }
    }
    return 0;
}