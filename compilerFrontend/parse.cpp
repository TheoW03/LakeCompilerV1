#include <iostream>
#include <string>
#include <vector>
#include "../compilerFrontend/Lexxer.h"
#include <typeinfo>
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
    Node() : left(nullptr), right(nullptr) {}
    struct Node *left;
    struct Node *right;
    int value;
    status s = status::NODE;
};
struct varaibleNode : public Node
{
    Node *expression;
    Tokens *varailbe;
    int size;
};
struct NumNode : public Node
{
    string num;
};
struct operatorNode : public Node
{
    struct Tokens *token;
};
struct StatementNode : public Node
{
    struct Node *expression;
    struct Tokens *nameOfVar;
};
struct funcCallNode : public Node
{
    Tokens *funcCall;
    vector<Tokens *> params;
};
struct FunctionNode : public Node
{
    struct Tokens *nameOfFunction;
    vector<Tokens *> params;
    vector<Node *> statements;
};
#pragma endregion
Node *expression(vector<Tokens> &tokens);
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
Tokens *matchAndRemove(vector<Tokens> &tokens, type typeT, string caller)
{
    if (tokens.empty())
    {
        return nullptr;
    }
    cout << "caller: " + caller << endl;
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
    // cout << "unmatched \n";
    return nullptr;
}

Node *factor(vector<Tokens> &tokens)
{
    Tokens *a = new Tokens;
    a = (matchAndRemove(tokens, type::NUMBER, "factor") != nullptr) ? current : (matchAndRemove(tokens, type::OP_PARENTHISIS, "factor") != nullptr) ? current
                                                                            : (matchAndRemove(tokens, type::WORD, "factor") != nullptr)             ? current
                                                                                                                                                    : nullptr;
    type id;
    if (a != nullptr)
    {
        id = a->id;
    }
    else
    {
        return nullptr;
    }
    if (id == type::NUMBER)
    {
        NumNode *numN = new NumNode;
        string b = a->buffer;
        // cout << "b: " + b << endl;
        numN->num = b;
        cout << "numN: " + numN->num << endl;
        delete a;
        return numN;
    }
    else if (id == type::OP_PARENTHISIS)
    {
        Node *exp = expression(tokens);
        matchAndRemove(tokens, type::CL_PARENTHISIS, "factor");
        cout << "op \n";

        return exp;
    }
    else if (id == type::WORD)
    {
        varaibleNode *var = new varaibleNode;
        var->varailbe = a;
        cout << "has been based var \n";
        // NumNode *numN = new NumNode;
        // string b = a->buffer;
        // // cout << "b: " + b << endl;
        // numN->num = b;
        // // cout << "numN: "+numN->num << endl;
        // return numN;
        return var;
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
    Node *n = new Node;
    Node *opNode = new Node;
    opNode = factor(tokens);
    Tokens *op = (matchAndRemove(tokens, type::MULTIPLY, "term") != nullptr) ? current : (matchAndRemove(tokens, type::DIVISION, "term") != nullptr) ? current
                                                                                     : (matchAndRemove(tokens, type::MOD, "term") != nullptr)        ? current
                                                                                                                                                     : nullptr; // n.value = 0;
    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::MULTIPLY, "term") != nullptr) ? current : (matchAndRemove(tokens, type::DIVISION, "term") != nullptr) ? current
                                                                                         : (matchAndRemove(tokens, type::MOD, "term") != nullptr)        ? current
                                                                                                                                                         : nullptr; // n.value = 0;
            }
            if (op == nullptr)
            {

                return opNode;
            }
            Node *right = new Node;
            right = factor(tokens);
            operatorNode *n = new operatorNode;
            n->left = new Node;
            n->left = opNode;
            n->right = new Node;
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
    Node *n = new Node;
    // Node* opNode = new Node;
    Node *opNode = term(tokens);
    Tokens *op = (matchAndRemove(tokens, type::ADDITION, "expressiion") != nullptr) ? current : (matchAndRemove(tokens, type::SUBTRACT, "expressiion") != nullptr) ? current
                                                                                                                                                                   : nullptr; // n.value = 0;

    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::ADDITION, "expressiion") != nullptr) ? current : (matchAndRemove(tokens, type::SUBTRACT, "expressiion") != nullptr) ? current
                                                                                                                                                                       : nullptr; // n.value = 0;
            }
            if (op == nullptr)
            {
                if (opNode == nullptr)
                {
                    // cout << "opNode nulll \n";
                }
                return opNode;
            }
            Node *right = new Node;
            right = term(tokens);
            operatorNode *n = new operatorNode;
            n->left = new Node;
            n->left = opNode;
            n->right = new Node;
            n->right = right;
            n->token = op;
            cout << op->buffer << endl;

            opNode = n;
            node = opNode;
            // delete op;
        }
    }
    if (opNode == nullptr)
    {
        cout << "opNode nulll \n";
    }
    return opNode;
    // do stuff
}

// will parse functions
Node *handleFunctions(vector<Tokens> &tokens)
{
    FunctionNode *f = new FunctionNode;
    matchAndRemove(tokens, type::FUNCTION, "handlefunctions");
    Tokens *name = matchAndRemove(tokens, type::WORD, "handlefunctions");
    matchAndRemove(tokens, type::OP_PARENTHISIS, "handlefunctions");

    vector<Tokens *> vars;
    while (matchAndRemove(tokens, type::CL_PARENTHISIS, "handlefunctions") == nullptr)
    {
        Tokens *var = matchAndRemove(tokens, type::WORD, "handlefunctions");
        matchAndRemove(tokens, type::COMMA, "handlefunctions");
        vars.push_back(var);
    }
    f->nameOfFunction = name;
    f->params = vars;
    return f;
}
void printParams(vector<Tokens *> a)
{
    cout << "params" << endl;
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i]->dictionary[a[i]->id] + "(" + a[i]->buffer + ") \n";
    }
}
Node *testParse(vector<Tokens> &tokens)
{
    FunctionNode *f = static_cast<FunctionNode *>(handleFunctions(tokens));
    printParams(f->params);
    return f;
}
void RemoveEOLS(vector<Tokens> &list)
{
    while (true)
    {
        Tokens *e = matchAndRemove(list, type::END_OF_LINE, "remove EOLS");

        if (e == nullptr)
        {
            return;
        }
    }
}
Node *parseVar(vector<Tokens> &tokens, Tokens *name)
{
    if (name == nullptr)
    {
        name = matchAndRemove(tokens, type::WORD, "parseVar");
    }
    matchAndRemove(tokens, type::EQUALS, "parseVar");
    varaibleNode *n = new varaibleNode;
    n->expression = expression(tokens);
    n->varailbe = name;
    n->size = 4;
    RemoveEOLS(tokens);

    return n;
}
Node *handleCalls(vector<Tokens> &tokens, Tokens *checkIfFunct)
{
    funcCallNode *f1 = new funcCallNode;
    f1->funcCall = checkIfFunct;
    matchAndRemove(tokens, type::OP_PARENTHISIS, "handlecalls");
    vector<Tokens *> vars;
    while (matchAndRemove(tokens, type::CL_PARENTHISIS, "handlecalls") == nullptr)
    {
        Tokens *var = matchAndRemove(tokens, type::WORD, "handlecalls");
        matchAndRemove(tokens, type::COMMA, "handlecalls");
        vars.push_back(var);
    }
    f1->params = vars;
    return f1;
}
/**
 * @brief function stuff
 *
 * @param tokens
 * @return Node*
 */
Node *handleSatements(vector<Tokens> &tokens)
{
#pragma region functionstate
    Tokens *checkIfFunct = matchAndRemove(tokens, type::PRINT, "functionParse");
    if (checkIfFunct != nullptr)
    {
        cout << "hi as \n";
        return handleCalls(tokens, checkIfFunct);
    }
#pragma endregion
#pragma region varstates
    Tokens *a = (matchAndRemove(tokens, type::WORD, "parsefunctions") != nullptr)  ? current
                : (matchAndRemove(tokens, type::VAR, "parsefunctions") != nullptr) ? current
                                                                                   : nullptr;
    if (a != nullptr)
    {
        Node *var;
        if (a->id == type::WORD)
        {
            if (matchAndRemove(tokens, type::EQUALS, "state") == nullptr)
            {
                return handleCalls(tokens, a);
            }
            var = parseVar(tokens, a);
        }
        else if (a->id == type::VAR)
        {
            var = parseVar(tokens, nullptr);
        }
        return var;
    }
#pragma endregion
    return nullptr;
}
Node *functionParse(vector<Tokens> &tokens)
{
    printList(tokens);
    FunctionNode *f;
    vector<Node *> states;
    if (matchAndRemove(tokens, type::FUNCTION, "functioon parse") != nullptr)
    {
        Node *func = handleFunctions(tokens);
        matchAndRemove(tokens, type::BEGIN, "parsefunctions");
        while (matchAndRemove(tokens, type::END, "parsefunctions") == nullptr)
        {
            RemoveEOLS(tokens);

            states.push_back(handleSatements(tokens));
            RemoveEOLS(tokens);
        }
        FunctionNode *pd = dynamic_cast<FunctionNode *>(func);
        if (pd != nullptr)
        {
            pd->statements = states;
        }
        return pd;
    }
    return nullptr;
}

/**
 * meant for testing Recursion stuff
 */
Node *parse(vector<Tokens> &tokens)
{
    return functionParse(tokens);

    // Tokens *var = matchAndRemove(tokens, type::WORD);
    // matchAndRemove(tokens, type::EQUALS);
    // Node *a = expression(tokens);
    // // varaibleNode *c = new varaibleNode;
    // // c->expression = a;
    // // c->varailbe = var;

    // if (a == nullptr)
    // {
    //     // cout << "null \n";
    //     return nullptr;
    // }

    // return a;
}