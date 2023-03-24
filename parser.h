#include <iostream>
#include <string>
#include <vector>
#include "tools.h"

struct Node
{
    struct Node *left;
    struct Node *right;
};

struct NullStruct : public Node{

};
struct NumNode : public Node
{
    string num;
};
typedef struct operatorNode : public Node
{
    struct Tokens token;
};
