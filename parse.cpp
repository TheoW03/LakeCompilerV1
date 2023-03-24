#include <iostream>
#include <string>
#include <vector>
#include "tools.h"
using namespace std;


struct NullStruct : public Node{

};
struct Node
{
    struct Node *left;
    struct Node *right;
    int value;
    

};

struct NumNode : public Node
{
    string num;
};
typedef struct operatorNode : public Node
{
    struct Tokens token;
};
Node expression()
{
    operatorNode n;
    return n;
    // do stuff
}
Node term()
{
    operatorNode n;
    return n;
    // do stuff
}
Node factor()
{
    operatorNode n;
    return n;
    // do stuff
}
Tokens matchAndRemove(vector<Tokens> tokens, type type)
{
    if (tokens[0].id == type)
    {
        Tokens t = tokens[0];
        tokens.erase(tokens.begin());
        return t;
    }
    Tokens t2;
    return t2;
}
