#include <iostream>
#include <string>
#include <vector>
#include "tools.h"
using namespace std;

#ifndef NODE_H
#define NODE_H

struct Node
{
    struct Node *left;
    struct Node *right;
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
    struct Tokens *token;
};

#endif // NODE_H

Node *parse(vector<Tokens>& tokens);

Node *term(vector<Tokens>& tokens);
Node *factor(vector<Tokens>& tokens);

Node *expression(vector<Tokens>& tokens);

Tokens *matchAndRemove(vector<Tokens>& tokens, type type);