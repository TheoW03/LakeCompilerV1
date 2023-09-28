#include <iostream>
#include <string>
#include <vector>
#include "../compilerFrontend/Lexxer.h"
#include <typeinfo>
#include <sstream>

#define OFFSET 65536

using namespace std;

#pragma region Node

// useless
enum status
{
    N_NULL,
    NUM,
    OPERATOR,
    NODE
};
/**
 * @brief C++ OOP is icky so
 *
 */
struct Node
{
    virtual ~Node();
    Node() : left(nullptr), right(nullptr) {}
    struct Node *left;
    struct Node *right;
    int value;
    status s = status::NODE;
};
// struct ConstantVarNode : public Node
// {
//     Node *expression;
//     Tokens *varailbe;
//     Tokens *typeOfVar;
//     int size;
// };
template <typename Base, typename T> 
bool instanceof2 (T * ptr)
{
    return (dynamic_cast<Base>(ptr) != nullptr);
}

struct VaraibleDeclaration : public Node
{
    Node *expression;
    Tokens *varailbe;
    Tokens *typeOfVar;
    int size;
    int constant;
};

struct FloatNode : public Node
{
    string num;
};
struct IntegerNode : public Node
{
    string num;
};
struct stringNode : public Node
{
    string stringBuffer;
};
struct BooleanLiteralNode : public Node
{
    Tokens *value;
};
struct ReturnStatment : public Node
{
    Node *expression;
};
struct BoolExpressionNode : public Node
{
    Node *right;
    Node *left;
    Tokens *op;
};
struct ElseNode : public Node
{

    vector<Node *> statements;
};
struct IfSatementNode : public Node
{

    BoolExpressionNode *condition;
    vector<Node *> statements;
    ElseNode *Else;
};
struct VaraibleReference : public Node
{
    Node *expression;
    Tokens *varailbe;
};

/**
 * @brief this gives an expression, left, rigt
 *
 */
struct operatorNode : public Node
{
    struct Tokens *token;
};

struct StatementNode : public Node
{
    struct Node *expression;
    struct Tokens *nameOfVar;
};
struct LoopNode : public Node
{
    BoolExpressionNode *condition;
    vector<Node *> statements;
};
/**
 * @brief calls
 * print();
 */
struct funcCallNode : public Node
{
    Tokens *funcCall;
    vector<Node *> params;
};
struct MacroNode : public Node
{
    Tokens *macro;
    Node *statement;
};
/**
 * @brief
 * function main(a,b){}
 *
 */
struct FunctionNode : public Node
{
    struct Tokens *nameOfFunction;
    vector<VaraibleDeclaration *> params;
    vector<Node *> statements;
    Tokens *returnType;
};
#pragma endregion
Node *expression(vector<Tokens> &tokens);
#pragma region ignore
/**
 * annoying segfault vibes :p unused so ignore
 *
 * @param n
 * @return true
 * @return false
 */
bool isNull(Node *n)
{
    return n->s == status::N_NULL;
}
void setNull(Node *n)
{
    n->s = status::N_NULL;
}
#pragma endregion
// Node parse(vector<Tokens> tokens){
//     return expression(tokens);
// }
Tokens *current = new Tokens;
Node *handleSatements(vector<Tokens> &tokens);

/**
 * @brief I could use a stack, but a stack coesnt have peek lol
 * it returns the 1st token, if the neum is equal return else.
 *
 * @param tokens
 * @param typeT
 * @param caller
 * @return Tokens*
 */
Tokens *matchAndRemove(vector<Tokens> &tokens, type typeT, string caller)
{
    if (tokens.empty())
    {
        return nullptr;
    }
    // cout << "caller: " + caller << endl;
    // cout << "passed: " + tokens[0].dictionary[typeT];
    // cout << "\n";
    // cout << "id: " + tokens[0].dictionary[tokens[0].id];
    // cout << "\n";
    // cout << "buf: " + tokens[0].buffer + " \n";
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
#pragma region Expression term and factor (for equations)
/**
 * @brief does Number, var, anything thats not +-/%
 * if number or var returns a nod for them else does recursion
 *
 * @param tokens
 * @return Node*
 */
Node *factor(vector<Tokens> &tokens)
{
    Tokens *a = new Tokens;
    a = (matchAndRemove(tokens, type::NUMBER, "factor") != nullptr)           ? current
        : (matchAndRemove(tokens, type::OP_PARENTHISIS, "factor") != nullptr) ? current
        : (matchAndRemove(tokens, type::WORD, "factor") != nullptr)           ? current
                                                                              : nullptr;

    if (a == nullptr)
    {
        a = matchAndRemove(tokens, type::STRING_LITERAL, "factor");
    }
    if (a == nullptr)
    {
        a = matchAndRemove(tokens, type::TRUE, "factor")    ? current
            : matchAndRemove(tokens, type::FALSE, "factor") ? current
                                                            : nullptr;
        BooleanLiteralNode *boolean = new BooleanLiteralNode;
        boolean->value = a;
        return boolean;
    }
    type id;
    if (a != nullptr)
    {
        id = a->id;
    }
    else
    {
        return nullptr;
    }
    if (id == type::STRING_LITERAL)
    {
        stringNode *s = new stringNode;
        s->stringBuffer = a->buffer;
        return s;
    }
    if (id == type::NUMBER)
    {
        string myString = a->buffer;
        if (myString.find(".") == string::npos)
        {
            IntegerNode *intNode = new IntegerNode;
            intNode->num = a->buffer;
            cout << "is int" << endl;
            delete a;
            return intNode;
        }
        int fPoint = (int)(stof(myString) * OFFSET);
        cout << fPoint << endl;
        cout << "hi \n";
        myString = to_string(fPoint); // whoses idea was it to give me a C++ compiler >:3
        cout << myString << endl;
        int myInt;
        FloatNode *floatNode = new FloatNode;
        floatNode->num = to_string(fPoint);
        delete a;
        return floatNode;
        // NumNode *numN = new NumNode;
        // string b = a->buffer;
        // // cout << "b: " + b << endl;
        // numN->num = b;
        // cout << "numN: " + numN->num << endl;
        // delete a;
        // return numN;
    }
    else if (id == type::OP_PARENTHISIS)
    {
        Node *exp = expression(tokens);
        matchAndRemove(tokens, type::CL_PARENTHISIS, "factor");
        cout << "op \n";

        return exp;
    }
    // else if (id == type::WORD)
    // {
    //     varaibleNode *var = new varaibleNode;
    //     var->varailbe = a;
    //     cout << "has been based var \n";
    //     // NumNode *numN = new NumNode;
    //     // string b = a->buffer;
    //     // // cout << "b: " + b << endl;
    //     // numN->num = b;
    //     // // cout << "numN: "+numN->num << endl;
    //     // return numN;
    //     return var;
    // }
    else if (id == type::WORD)
    {
        VaraibleReference *var = new VaraibleReference;
        var->varailbe = a;
        return var;
    }
    else
    {
        return nullptr;
    }

    // do stuff
}

/**
 * @brief
 * does /*%
 * @param tokens
 * @return Node*
 */
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

/**
 * @brief +,-
 *
 * @param tokens
 * @return Node*
 */
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
#pragma endregion
#pragma region statements
/**
 * @brief helper function for 'l'
 *
 * @param list
 */
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
// will parse functions
Node *handleFunctions(vector<Tokens> &tokens)
{
    FunctionNode *f = new FunctionNode;
    matchAndRemove(tokens, type::FUNCTION, "handlefunctions");
    Tokens *name = matchAndRemove(tokens, type::WORD, "handlefunctions");
    matchAndRemove(tokens, type::OP_PARENTHISIS, "handlefunctions");

    vector<VaraibleDeclaration *> vars;

    while (matchAndRemove(tokens, type::CL_PARENTHISIS, "handlefunctions") == nullptr)
    {
        Tokens *typeVar = (matchAndRemove(tokens, type::VAR, "parsefunctions") != nullptr)      ? current
                          : (matchAndRemove(tokens, type::FLOAT, "parsefunctions") != nullptr)  ? current
                          : (matchAndRemove(tokens, type::INT, "parsefunctions") != nullptr)    ? current
                          : (matchAndRemove(tokens, type::STRING, "parseFunctions") != nullptr) ? current
                          : (matchAndRemove(tokens, type::BOOL, "parseFunctions") != nullptr)   ? current

                                                                                              : nullptr;
        Tokens *word = matchAndRemove(tokens, type::WORD, "handlefunctions");

        matchAndRemove(tokens, type::COMMA, "handlefunctions");
        VaraibleDeclaration *v = new VaraibleDeclaration;
        v->typeOfVar = typeVar;
        v->varailbe = word;
        vars.push_back(v);
    }
    // while (matchAndRemove(tokens, type::CL_PARENTHISIS, "handlefunctions") == nullptr)
    // {
    //     Tokens *var = matchAndRemove(tokens, type::WORD, "handlefunctions");
    //     matchAndRemove(tokens, type::COMMA, "handlefunctions");
    //     vars.push_back(var);
    // }
    f->nameOfFunction = name;
    f->params = vars;
    return f;
}

Node *handleMacros(vector<Tokens> &list)
{
    Tokens *name = matchAndRemove(list, type::WORD, "macros");
    Node *statements;
    statements = expression(list);
    if (statements == nullptr)
    {
        statements = handleSatements(list);
    }
    MacroNode *a = new MacroNode;
    a->macro = name;
    a->statement = statements;
    RemoveEOLS(list);
    return a;
}
void printParams(vector<Tokens *> a)
{
    cout << "params" << endl;
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i]->dictionary[a[i]->id] + "(" + a[i]->buffer + ") \n";
    }
}

Node *parserVarRef(vector<Tokens> &tokens, Tokens *name)
{
    VaraibleReference *var = new VaraibleReference;
    var->varailbe = name;
    var->expression = expression(tokens);
    return var;
}
/**
 * @brief just me testing, ignore
 *
 * @param tokens
 * @return Node*
 */
Node *testParse(vector<Tokens> &tokens)
{
    FunctionNode *f = static_cast<FunctionNode *>(handleFunctions(tokens));
    // printParams(f->params);
    return f;
}

Node *parseVar(vector<Tokens> &tokens, Tokens *name, Tokens *type, int constant = 0)
{

    name = matchAndRemove(tokens, type::WORD, "parseVar");
    matchAndRemove(tokens, type::EQUALS, "parseVar");
    VaraibleDeclaration *n = new VaraibleDeclaration;
    n->expression = expression(tokens);
    n->varailbe = name;
    n->size = 4;
    n->typeOfVar = type;
    n->constant = constant;
    RemoveEOLS(tokens);
    return n;
}
/**
 * @brief function
 *
 * @param tokens
 * @param checkIfFunct
 * @return Node*
 */
BoolExpressionNode *handleBooleanExpression(vector<Tokens> &tokens)
{
    Node *right = factor(tokens);

    Tokens *op = (matchAndRemove(tokens, type::BOOL_EQ, "parsefunctions") != nullptr) ? current
                 : (matchAndRemove(tokens, type::LTE, "parsefunctions") != nullptr)   ? current
                 : (matchAndRemove(tokens, type::GTE, "parsefunctions") != nullptr)   ? current
                 : (matchAndRemove(tokens, type::GT, "parsefunctions") != nullptr)    ? current
                 : (matchAndRemove(tokens, type::LT, "parseFunctions") != nullptr)    ? current
                                                                                      : nullptr;
    Node *left = factor(tokens);
    BoolExpressionNode *a = new BoolExpressionNode;
    a->right = right;
    a->left = left;
    a->op = op;
    return a;
}

Node *handleCalls(vector<Tokens> &tokens, Tokens *checkIfFunct)
{
    funcCallNode *f1 = new funcCallNode;
    f1->funcCall = checkIfFunct;
    matchAndRemove(tokens, type::OP_PARENTHISIS, "handlecalls");
    vector<Node *> vars;
    while (matchAndRemove(tokens, type::CL_PARENTHISIS, "handlecalls") == nullptr)
    {
        // Node *var = expression(tokens);
        Tokens *var = (matchAndRemove(tokens, type::WORD, "parsefunctions") != nullptr)             ? current
                      : (matchAndRemove(tokens, type::NUMBER, "parsefunctions") != nullptr)         ? current
                      : (matchAndRemove(tokens, type::STRING_LITERAL, "parseFunctions") != nullptr) ? current
                                                                                                    : nullptr;
        matchAndRemove(tokens, type::COMMA, "handlefunctions");
        VaraibleReference *v = new VaraibleReference;
        if (var->id == type::NUMBER)
        {
            string myString = var->buffer;
            if (myString.find(".") == string::npos)
            {
                IntegerNode *intNode = new IntegerNode;
                intNode->num = var->buffer;
                cout << "is int" << endl;
                vars.push_back(intNode);
            }
            else
            {
                int fPoint = (int)(stof(myString) * OFFSET);
                cout << fPoint << endl;
                cout << "hi \n";
                myString = to_string(fPoint); // whoses idea was it to give me a C++ compiler >:3
                cout << myString << endl;
                int myInt;
                FloatNode *floatNode = new FloatNode;
                floatNode->num = to_string(fPoint);
                vars.push_back(floatNode);
            }
        }
        else if (var->id == type::WORD)
        {
            VaraibleReference *v1 = new VaraibleReference;
            v1->varailbe = var;
            vars.push_back(v1);
        }
        else if (var->id == type::STRING_LITERAL)
        {
            stringNode *s = new stringNode;
            s->stringBuffer = var->buffer;
            vars.push_back(s);
        }
    }
    f1->params = vars;
    return f1;
}
Node *handleLoops(vector<Tokens> &tokens)
{
    LoopNode *loop = new LoopNode;
    BoolExpressionNode *a = new BoolExpressionNode;
    matchAndRemove(tokens, type::OP_PARENTHISIS, "a");
    a = handleBooleanExpression(tokens);
    matchAndRemove(tokens, type::CL_PARENTHISIS, "a");
    matchAndRemove(tokens, type::BEGIN, "a");
    vector<Node *> states;
    RemoveEOLS(tokens);

    while (matchAndRemove(tokens, type::END, "j") == nullptr)
    {
        RemoveEOLS(tokens);
        states.push_back(handleSatements(tokens));
        RemoveEOLS(tokens);
    }
    loop->condition = a;
    loop->statements = states;

    return loop;
}

Node *handleIfStatements(vector<Tokens> &tokens)
{
    BoolExpressionNode *a = new BoolExpressionNode;
    matchAndRemove(tokens, type::OP_PARENTHISIS, "a");
    a = handleBooleanExpression(tokens);

    IfSatementNode *ifStatement = new IfSatementNode;
    ifStatement->condition = a;
    matchAndRemove(tokens, type::CL_PARENTHISIS, "a");
    matchAndRemove(tokens, type::BEGIN, "a");
    vector<Node *> states;
    RemoveEOLS(tokens);

    while (matchAndRemove(tokens, type::END, "j") == nullptr)
    {
        RemoveEOLS(tokens);
        states.push_back(handleSatements(tokens));
        RemoveEOLS(tokens);
    }
    ifStatement->statements = states;
    return ifStatement;
}

Node *handleReturn(vector<Tokens> &tokens)
{
    ReturnStatment *returns = new ReturnStatment;
    returns->expression = expression(tokens);
    return returns;
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
    if (matchAndRemove(tokens, type::MACRO, "statements") != nullptr)
    {
        return handleMacros(tokens);
    }
    Tokens *checkIfFunct = (matchAndRemove(tokens, type::PRINT, "functionParse") != nullptr)  ? current
                           : (matchAndRemove(tokens, type::EXIT, "functionParse") != nullptr) ? current
                                                                                              : nullptr;
    if (checkIfFunct != nullptr)
    {
        cout << "hi as \n";
        return handleCalls(tokens, checkIfFunct);
    }
#pragma endregion
#pragma region varstates
    Tokens *a = (matchAndRemove(tokens, type::WORD, "parsefunctions") != nullptr)       ? current
                : (matchAndRemove(tokens, type::CONSTANT, "parsefunctions") != nullptr) ? current
                : (matchAndRemove(tokens, type::VAR, "parsefunctions") != nullptr)      ? current
                : (matchAndRemove(tokens, type::FLOAT, "parsefunctions") != nullptr)    ? current
                : (matchAndRemove(tokens, type::INT, "parsefunctions") != nullptr)      ? current
                : (matchAndRemove(tokens, type::BOOL, "parsefunctions") != nullptr)     ? current
                : (matchAndRemove(tokens, type::STRING, "parseFunctions") != nullptr)   ? current
                : (matchAndRemove(tokens, type::IF, "k") != nullptr)                    ? current
                : (matchAndRemove(tokens, type::LOOP, "k") != nullptr)                  ? current
                : (matchAndRemove(tokens, type::RETURN, "k") != nullptr)                ? current
                                                                                        : nullptr;

    if (a != nullptr)
    {
        if (a->id == type::IF)
        {
            return handleIfStatements(tokens);
        }
        if (a->id == type::LOOP)
        {
            return handleLoops(tokens);
        }
        if (a->id == type::RETURN)
        {
            return handleReturn(tokens);
        }

        Node *var;
        if (a->id == type::WORD)
        {
            if (matchAndRemove(tokens, type::EQUALS, "state") == nullptr)
            {
                return handleCalls(tokens, a);
            }
            // var = parseVar(tokens, a, nullptr);
            var = parserVarRef(tokens, a);
        }
        else if (a->id == type::VAR || a->id == type::INT || a->id == type::FLOAT || a->id == type::STRING || a->id == type::BOOL)
        {
            var = parseVar(tokens, nullptr, a);
        }
        else if (a->id == type::CONSTANT)
        {
            a = (matchAndRemove(tokens, type::VAR, "parsefunctions") != nullptr)     ? current
                : (matchAndRemove(tokens, type::FLOAT, "parsefunctions") != nullptr) ? current
                : (matchAndRemove(tokens, type::INT, "parsefunctions") != nullptr)   ? current
                : (matchAndRemove(tokens, type::BOOL, "parsefunctions") != nullptr)  ? current
                                                                                     : nullptr;
            var = parseVar(tokens, nullptr, a, 1);
        }
        return var;
    }
    else
    {
        cout << "undefined statement" << endl;
        cout << "" << endl;
        cout << "1st one is the undefined statement" << endl;
        cout << "================" << endl;
        printList(tokens);

        exit(0);
    }

#pragma endregion
    return nullptr;
}
/**
 * @brief parses functions
 *
 *
 * @param tokens
 * @return Node*
 */
vector<FunctionNode *> functionParse(vector<Tokens> &tokens)
{
    vector<FunctionNode *> functionNodes;
    while (matchAndRemove(tokens, type::FUNCTION, "functioon parse") != nullptr)
    {

        FunctionNode *f;

        vector<Node *> states;
        // if (matchAndRemove(tokens, type::FUNCTION, "functioon parse") != nullptr)
        // {
        Node *func = handleFunctions(tokens);
        FunctionNode *pd = dynamic_cast<FunctionNode *>(func);

        if (matchAndRemove(tokens, type::RETURNS, "parsefunctions") != nullptr)
        {
            Tokens *f1 = (matchAndRemove(tokens, type::FLOAT, "parsefunctions") != nullptr)    ? current
                         : (matchAndRemove(tokens, type::INT, "parsefunctions") != nullptr)    ? current
                         : (matchAndRemove(tokens, type::BOOL, "parsefunctions") != nullptr)   ? current
                         : (matchAndRemove(tokens, type::STRING, "parseFunctions") != nullptr) ? current
                                                                                               : nullptr;

            pd->returnType = f1;
        }
        else if (matchAndRemove(tokens, type::SEMI_COLON, "parsefunctions") != nullptr)
        {
            Tokens *f1 = (matchAndRemove(tokens, type::FLOAT, "parsefunctions") != nullptr)    ? current
                         : (matchAndRemove(tokens, type::INT, "parsefunctions") != nullptr)    ? current
                         : (matchAndRemove(tokens, type::BOOL, "parsefunctions") != nullptr)   ? current
                         : (matchAndRemove(tokens, type::STRING, "parseFunctions") != nullptr) ? current
                                                                                               : nullptr;
            pd->returnType = f1;
            if (matchAndRemove(tokens, type::SUBTRACT, "parsefunctions") != nullptr && matchAndRemove(tokens, type::GT, "parsefunctions") != nullptr)
            {
                states.push_back(handleReturn(tokens));
            }
        }

        if (matchAndRemove(tokens, type::BEGIN, "parsefunctions") != nullptr)
        {
            while (matchAndRemove(tokens, type::END, "parsefunctions") == nullptr)
            {
                RemoveEOLS(tokens);
                states.push_back(handleSatements(tokens));
                RemoveEOLS(tokens);
            }
        }
        pd->statements = states;

        functionNodes.push_back(pd);
        RemoveEOLS(tokens);
        // return pd;
    }

    return functionNodes;
}
#pragma endregion

#pragma region entrypoint
/**
 * @brief
 *
 * @param tokens
 * @return Node*
 * this one handles the expression, can be used for testing "x86" target
 */
Node *testExpressionParse(vector<Tokens> &tokens)
{
    return expression(tokens);
}
/**
 * the entry point for parsing
 * i will eventually upgrade it to a list
 */

vector<FunctionNode *> parse(vector<Tokens> &tokens)
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
#pragma endregion