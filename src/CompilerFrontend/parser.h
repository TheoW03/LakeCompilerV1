#include <iostream>
#include <string>
#include <vector>
#include <optional>
#include "../../src/CompilerFrontend/Lexxer.h"
using namespace std;

#define OFFSET 65536

#ifndef NODE_H
#define NODE_H

struct Node
{
    Node *left;
    // int s;
    Node *right;
    virtual ~Node();
};
#endif

#ifndef VAR_REF_H
#define VAR_REF_H
struct VaraibleReference : public Node
{
    Node *expression;
    Tokens varaible;
};
#endif

#ifndef VAR_DEC_H
#define VAR_DEC_H
struct VaraibleDeclaration : public Node
{
    Node *expression;
    Tokens varaible;
    Tokens typeOfVar;
    int size;
    int constant;
};
#endif

#ifndef BOOL_EXPR_NODE_H
#define BOOL_EXPR_NODE_H
struct BoolExpressionNode : public Node
{
    Node *right;
    Node *left;
    optional<Tokens> op;
};
#endif

#ifndef BOOL_LITERAL_NODE_H
#define BOOL_LITERAL_NODE_H
struct BooleanLiteralNode : public Node
{
    Tokens *value;
};
#endif

#ifndef LOOP_NODE_H
#define LOOP_NODE_H
struct LoopNode : public Node
{
    BoolExpressionNode *condition;
    vector<Node *> statements;
};
#endif

#ifndef ELSE_NODE_H
#define ELSE_NODE_H
struct ElseNode : public Node
{

    vector<Node *> statements;
};
#endif

#ifndef CHAR_NODE_H
#define CHAR_NODE_H
struct CharNode : public Node
{
    string character;
};
#endif

#ifndef IF_STATEMENT_NODE_H
#define IF_STATEMENT_NODE_H
struct IfSatementNode : public Node
{

    BoolExpressionNode *condition;
    vector<Node *> statements;
    ElseNode *Else;
};
#endif

#ifndef FUNC_CALL_NODE_H
#define FUNC_CALL_NODE_H
struct funcCallNode : public Node
{
    Tokens funcCall;
    vector<Node *> params;
};
#endif

#ifndef FLOAT_NODE_H
#define FLOAT_NODE_H
struct FloatNode : public Node
{
    string num;
};
#endif

#ifndef INT_NODE_H
#define INT_NODE_H
struct IntegerNode : public Node
{
    string num;
};
#endif

#ifndef STRING_NODE_H
#define STRING_NODE_H
struct StringNode : public Node
{
    string stringBuffer;
};
#endif

#ifndef OP_NODE_H
#define OP_NODE_H
struct OperatorNode : public Node
{
    struct Tokens token;
};
#endif
#ifndef STATE_NODE_H
#define STATE_NODE_H
struct StatementNode : public Node
{
    struct Node *expression;
    Tokens nameOfVar;
};
#endif
#ifndef RETURN_STATEMENT_H
#define RETURN_STATEMENT_H
struct ReturnStatment : public Node
{
    Node *expression;
};
#endif

#ifndef FUNCTION_NODE_H
#define FUNCTION_NODE_H
struct FunctionNode : public Node
{
    Tokens nameOfFunction;
    vector<VaraibleDeclaration *> params;
    vector<Node *> statements;
    optional<Tokens> returnType;
};
#endif
#ifndef FOR_LOOP_NODE_H
#define FOR_LOOP_NODE_H
struct ForLoopNode : public Node
{
    Node *incrimentorVar;
    BoolExpressionNode *condition;
    vector<Node *> statements;
};
#endif

#ifndef ARRAY_DEC_H
#define ARRAY_DEC_H

struct ArrayDeclaration : public Node
{
    Node *size;
    Tokens varaible;
    Tokens typeOfVar;
};
#endif

#ifndef ARRAY_REF_H
#define ARRAY_REF_H
struct ArrayRef : public Node
{
    Tokens name;
    Node *RefedLocation;
    Node *value;
};
#endif
vector<FunctionNode *> parse(vector<Tokens> &tokens);

Node *term(vector<Tokens> &tokens);
Node *factor(vector<Tokens> &tokens);

Node *expression(vector<Tokens> &tokens);

Tokens *matchAndRemove(vector<Tokens> &tokens, type type, string caller);
bool isNull(Node *n);
Node *handleFunctions(vector<Tokens> &tokens);
void printParams(vector<Tokens *> a);
Node *testParse(vector<Tokens> &tokens);

vector<FunctionNode *> functionParse(vector<Tokens> &tokens);
Node *parseVar(vector<Tokens> &tokens, Tokens *name);
Node *handleCalls(vector<Tokens> &tokens, Tokens *checkIfFunct);
Node *handleSatements(vector<Tokens> &tokens);
Node *testExpressionParse(vector<Tokens> &tokens);