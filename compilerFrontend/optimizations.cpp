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

int check_if_pureExpression(Node *op)
{
    if (op == nullptr)
    {
        return 0;
    }
    if (dynamic_cast<NumNode *>(op) != nullptr)
    {
        NumNode* pd = dynamic_cast<NumNode *>(op);
        cout << pd->num << endl;
    }
    else
    {
        cout << "null" << endl;
    }

    check_if_pureExpression(op->left);
    check_if_pureExpression(op->right);
    return 1;
}
int solve(Node *op)
{
    if (op == nullptr)
    {
        return 1;
    }
    NumNode *pd = dynamic_cast<NumNode *>(op);
    if (pd != nullptr)
    {
        return stoi(pd->num);
    }
    if (dynamic_cast<operatorNode *>(op) != nullptr)
    {
        operatorNode *pd = dynamic_cast<operatorNode *>(op); // downcast
        type t = pd->token->id;
        if (t == type::ADDITION)
        {
            return solve(op->right) + solve(op->left);
        }
         if (t == type::SUBTRACT)
        {
            return solve(op->right) - solve(op->left);
        }
        if (t == type::MULTIPLY)
        {
            return solve(op->right) * solve(op->left);
        }
        if (t == type::DIVISION)
        {
            return solve(op->right) / solve(op->left);
        }
        if (t == type::MOD)
        {
            return solve(op->right) % solve(op->left);
        }
    }
    
}