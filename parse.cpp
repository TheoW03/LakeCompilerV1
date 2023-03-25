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
};
struct NullStruct : public Node
{
};
struct NumNode : public Node
{
    string num;
};
struct operatorNode : public Node
{
    struct Tokens token;
};
#pragma endregion

// Node parse(vector<Tokens> tokens){
//     return expression(tokens);
// }

Tokens *matchAndRemove(vector<Tokens> tokens, type type)
{
    if (tokens[0].id == type)
    {
        Tokens *t = new Tokens(tokens[0]);
        tokens.erase(tokens.begin());
        return t;
    }
    Tokens t2;
    return nullptr;
}

Node *factor(vector<Tokens> tokens)
{
    NullStruct op;
    Tokens *a = matchAndRemove(tokens, type::NUMBER);
    if (a == nullptr)
    {
        NumNode *numN;
        numN->num = a->buffer;
        return numN;
    }
    return nullptr;
    // do stuff
}
Node *term(vector<Tokens> tokens)
{
    operatorNode *n;
    // n.value = 0;
    return n;
    // do stuff
}

Node *expression(vector<Tokens> tokens)
{
    operatorNode* n;
    // n.value = 0;
    return n;
    // do stuff
}
Node *parse(vector<Tokens> tokens)
{
    // printList(tokens);

    return expression(tokens);
}