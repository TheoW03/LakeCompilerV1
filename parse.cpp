#include <iostream>
#include <string>
#include <vector>
#include "tools.h"

using namespace std;

#pragma region Node

struct Node
{
    struct Node *left;
    struct Node *right;
    int value;
    // virtual void toString() {}
};
struct NumNode : public Node
{
    string num;
    // virtual void toStrinf() {return n;um}
};
struct operatorNode : public Node
{
    struct Tokens *token;
    virtual void a() {}
};
#pragma endregion

// Node parse(vector<Tokens> tokens){
//     return expression(tokens);
// }
Tokens *current;
Tokens *matchAndRemove(vector<Tokens> &tokens, type typeT)
{
    if (tokens.size() == 1)
    {
        return nullptr;
    }
    if (tokens[0].id == typeT)
    {
        Tokens *t = new Tokens(tokens[0]);
        current = t;
        tokens.erase(tokens.begin());
        return t;
    }
    Tokens t2;
    return nullptr;
}

Node *factor(vector<Tokens> &tokens)
{
    Tokens *a = matchAndRemove(tokens, type::NUMBER);
    if (a != nullptr)
    {
        NumNode *numN;
        numN->num = a->buffer;
        return numN;
    }
    return nullptr;
    // do stuff
}
Node *term(vector<Tokens> &tokens)
{

    // n.value = 0;
    Node *n;
    Node *opNode = factor(tokens);
    Tokens *op = (matchAndRemove(tokens, type::MULTIPLY) != nullptr) ? current : (matchAndRemove(tokens, type::DIVISION) != nullptr) ? current
                                                                                                                                     : nullptr; // n.value = 0;
    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::MULTIPLY) != nullptr) ? current : (matchAndRemove(tokens, type::DIVISION) != nullptr) ? current
                                                                                                                                         : nullptr; // n.value = 0;
            }
            if (op == nullptr)
            {
                return opNode;
            }
            Node *right = factor(tokens);
            operatorNode *opNode = new operatorNode;
            opNode->left = opNode;
            opNode->right = right;
            opNode->token = op;
            node = opNode;
        }
    }
    return opNode;
    // do stuff
}

Node *expression(vector<Tokens> &tokens)
{
    Node *n;
    Node *opNode = term(tokens);

    Tokens *op = (matchAndRemove(tokens, type::ADDITION) != nullptr) ? current : (matchAndRemove(tokens, type::SUBTRACT) != nullptr) ? current
                                                                                                                                     : nullptr; // n.value = 0;
    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::ADDITION) != nullptr) ? current : (matchAndRemove(tokens, type::SUBTRACT) != nullptr) ? current
                                                                                                                                         : nullptr; // n.value = 0;
            }
            if (op == nullptr)
            {
                return opNode;
            }
            Node *right = term(tokens);
            operatorNode *opNode = new operatorNode;
            opNode->left = opNode;
            opNode->right = right;
            opNode->token = op;
            node = opNode;
        }
    }
    return opNode;
    // do stuff
}
Node *parse(vector<Tokens> &tokens)
{
    // printList(tokens);
    Node *a = new Node(*expression(tokens));
    return a;
}