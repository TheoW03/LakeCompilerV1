#include <iostream>
#include <string>

#include <sys/stat.h>
#include <fstream>
#include <typeinfo>
#include <filesystem>
#include "../compilerFrontend/Lexxer.h"
#include "../compilerFrontend/parser.h"
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
    if (dynamic_cast<varaibleNode *>(op) != nullptr)
    {

        cout << "var \n";
        return 1;
    }
    int a = check_if_pureExpression(op->left);
    if (a == 0)
    {
        check_if_pureExpression(op->right);
    }
}

float constant_prop_float(Node *op)
{
    if (op == nullptr)
    {
        return 1;
    }
    IntegerNode *pd1 = dynamic_cast<IntegerNode *>(op);
    FloatNode *pd2 = dynamic_cast<FloatNode *>(op);

    if (pd1 != nullptr)
    {
        return (int)stoi(pd1->num);
    }
    if (pd2 != nullptr)
    {
        cout << "the float: ";
        cout << stof(pd2->num) / OFFSET << endl;
        return (float)stof(pd2->num) / OFFSET;
    }
    if (dynamic_cast<operatorNode *>(op) != nullptr)
    {
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t = pd->token->id;
        if (t == type::ADDITION)
        {
            return constant_prop_float(op->right) + constant_prop_float(op->left);
        }
        if (t == type::SUBTRACT)
        {
            return constant_prop_float(op->right) - constant_prop_float(op->left);
        }
        if (t == type::MULTIPLY)
        {
            return constant_prop_float(op->right) * constant_prop_float(op->left);
        }
        if (t == type::DIVISION)
        {
            return constant_prop_float(op->right) / constant_prop_float(op->left);
        }
        if (t == type::MOD)
        {
            return ((int)constant_prop_float(op->right) % (int)constant_prop_float(op->left));
        }
    }
}
int constant_prop_integer(Node *op)
{
    if (op == nullptr)
    {
        return 1;
    }
    IntegerNode *pd = dynamic_cast<IntegerNode *>(op);

    if (pd != nullptr)
    {
        return (int)stoi(pd->num);
    }

    if (dynamic_cast<operatorNode *>(op) != nullptr)
    {
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
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