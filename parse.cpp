#include <iostream>
#include <string>
#include <vector>
#include "tools.h"

using namespace std;

#pragma region Node
enum status
{
    N_NULL,
    NUM,
    OPERATOR,
    NODE
};
struct Node
{
    virtual ~Node();

    struct Node *left;
    struct Node *right;
    int value;
    status s = status::NODE;
};

struct NumNode : public Node
{
    string num;
};
struct operatorNode : public Node
{
    struct Tokens *token;
};
#pragma endregion

bool isNull(Node *n)
{
    return n->s == status::N_NULL;
}
void setNull(Node *n)
{
    n->s = status::N_NULL;
}
// Node parse(vector<Tokens> tokens){
//     return expression(tokens);
// }
Tokens *current = new Tokens;
Tokens *matchAndRemove(vector<Tokens> &tokens, type typeT)
{
    if (tokens.empty())
    {
        return nullptr;
    }

    cout << "passed: " + tokens[0].dictionary[typeT];
    cout << "\n";
    cout << "id: " + tokens[0].dictionary[tokens[0].id];
    cout << "\n";
    cout << "buf: " + tokens[0].buffer + " \n";
    if (tokens[0].id == typeT)
    {

        Tokens *t = new Tokens(tokens[0]);
        current = t;
        tokens.erase(tokens.begin());
        return t;
    }
    cout << "unmatched \n";
    return nullptr;
}

Node *factor(vector<Tokens> &tokens)
{
    Tokens *a = new Tokens;
    a = matchAndRemove(tokens, type::NUMBER);
    if (a != nullptr)
    {
        NumNode *numN = new NumNode;
        string b = a->buffer;
        cout << "b: " + b << endl;
        numN->num =b;
        cout << "numN: "+numN->num << endl;
        delete a;
        return numN;
    }
    else
    {
        return nullptr;
    }
    // do stuff
}
Node *term(vector<Tokens> &tokens)
{

    // n.value = 0;
    Node *n;
    Node *opNode;
    opNode = factor(tokens);
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
            operatorNode *n = new operatorNode;
            n->left = opNode;
            n->right = right;
            n->token = op;
            opNode = n;
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
                                                                                                                                     : NULL; // n.value = 0;
    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::ADDITION) != nullptr) ? current : (matchAndRemove(tokens, type::SUBTRACT) != nullptr) ? current
                                                                                                                                         : NULL; // n.value = 0;
            }
            if (op == nullptr)
            {
                if (opNode == nullptr)
                {
                    cout << "opNode nulll \n";
                }
                return opNode;
            }
            Node *right = term(tokens);
            operatorNode *n = new operatorNode;
            n->left = opNode;
            n->right = right;
            n->token = op;
            opNode = n;
            node = opNode;
        }
    }
    if (opNode == nullptr)
    {
        cout << "opNode nulll \n";
    }
    return opNode;
    // do stuff
}
Node *parse(vector<Tokens> &tokens)
{
    // printList(tokens);
    Node *a = expression(tokens);
    if (a == nullptr)
    {
        cout << "null \n";
        return nullptr;
    }

    return a;
}