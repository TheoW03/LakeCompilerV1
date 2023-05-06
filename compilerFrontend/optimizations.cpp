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
        return 1;
    }
    if (dynamic_cast<varaibleNode *>(op) != nullptr)
    {

        return 0;
    }

    check_if_pureExpression(op->left);
    check_if_pureExpression(op->right);
}
float constant_prop(Node *op)
{
    if (op == nullptr)
    {
        return 1;
    }
    IntegerNode *pd = dynamic_cast<IntegerNode *>(op);
    FloatNode *pd2 = dynamic_cast<FloatNode *>(op);

    if (pd != nullptr)
    {
        return (int)stoi(pd->num);
    }
    else if (pd2 != nullptr)
    {
        return stof(pd2->num);
    }

    if (dynamic_cast<operatorNode *>(op) != nullptr)
    {
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t = pd->token->id;
        if (t == type::ADDITION)
        {
            return constant_prop(op->right) + constant_prop(op->left);
        }
        if (t == type::SUBTRACT)
        {
            return constant_prop(op->right) - constant_prop(op->left);
        }
        if (t == type::MULTIPLY)
        {
            return constant_prop(op->right) * constant_prop(op->left);
        }
        if (t == type::DIVISION)
        {
            return constant_prop(op->right) / constant_prop(op->left);
        }
        if (t == type::MOD)
        {
            return (float)((int)constant_prop(op->right) % (int)constant_prop(op->left));
        }
    }
    return 0;
}