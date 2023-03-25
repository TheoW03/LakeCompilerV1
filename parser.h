#include <iostream>
#include <string>
#include <vector>
#include "tools.h"
using namespace std;


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
    struct Tokens token;
};

Node* parse(vector <Tokens> tokens);

Node* term(vector<Tokens> tokens);
Node* factor(vector<Tokens> tokens);

Node* expression(vector<Tokens> tokens);

Tokens* matchAndRemove(vector<Tokens> tokens, type type);