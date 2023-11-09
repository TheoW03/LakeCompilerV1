#include <iostream>
#include <string>
#include <vector>
#include "../../src/CompilerFrontend/Lexxer.h"
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

struct VaraibleDeclaration : public Node
{
    Node *expression;
    Tokens *varaible;
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
struct StringNode : public Node
{
    string stringBuffer;
};
struct CharNode : public Node
{
    string character;
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
    Tokens *varaible;
};
struct ForLoopNode : public Node
{
    Node *incrimentorVar;
    BoolExpressionNode *condition;
    vector<Node *> statements;
};

/**
 * @brief this gives an expression, left, rigt
 *
 */
struct OperatorNode : public Node
{
    struct Tokens *token;
};

struct StatementNode : public Node
{
    struct Node *expression;
    struct Tokens *nameOfVar;
};
struct ArrayDeclaration : public Node
{
    Node *size;
    Tokens *varaible;
    Tokens *typeOfVar;
};
struct ArrayRef : public Node
{
    Tokens *name;
    Node *RefedLocation;
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
Node *handleCalls(vector<Tokens> &tokens, Tokens *checkIfFunct);

/**
 * @brief I could use a stack, but a stack coesnt have peek lol
 * it returns the 1st token, if the neum is equal return else.
 *
 * @param tokens
 * @param typeT
 * @param caller
 * @return Tokens*
 */
Tokens *matchAndRemove(vector<Tokens> &tokens, type typeT)
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
        // cout << "matched" << endl;
        // cout << "=" << endl;
        // cout << "=" << endl;
        return t;
    }
    // cout << "unmatched \n";
    // cout << "=" << endl;
    // cout << "=" << endl;

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

// todo: #3 REWRITE
Node *factor(vector<Tokens> &tokens)
{

    if (matchAndRemove(tokens, type::NUMBER) != nullptr)
    {
        string myString = current->buffer;
        if (myString.find(".") == string::npos)
        {
            IntegerNode *intNode = new IntegerNode;
            intNode->num = current->buffer;
            return intNode;
        }
        int fPoint = (int)(stof(myString) * OFFSET);
        FloatNode *floatNode = new FloatNode;
        floatNode->num = to_string(fPoint);
        return floatNode;
    }
    else if (matchAndRemove(tokens, type::OP_PARENTHISIS) != nullptr)
    {
        Node *exp = expression(tokens);
        matchAndRemove(tokens, type::CL_PARENTHISIS);
        return exp;
    }
    else if (matchAndRemove(tokens, type::STRING_LITERAL) != nullptr)
    {
        StringNode *s = new StringNode;
        s->stringBuffer = current->buffer;
        return s;
    }
    else if (matchAndRemove(tokens, type::WORD) != nullptr)
    {
        Node *a = handleCalls(tokens, current);
        if (a == nullptr)
        {
            VaraibleReference *var = new VaraibleReference;
            var->varaible = current;
            return var;
        }
        return a;
    }
    else if (matchAndRemove(tokens, type::TRUE) != nullptr || matchAndRemove(tokens, type::FALSE) != nullptr)
    {
        BooleanLiteralNode *boolean = new BooleanLiteralNode;
        boolean->value = current;
        return boolean;
    }
    else if (matchAndRemove(tokens, type::CHAR_LITERAL) != nullptr)
    {
        CharNode *integer = new CharNode;

        string s = current->buffer;
        char myChar = s[0]; // This gets the first character (index 0)
        integer->character = to_string((int)myChar);
        return integer;
    }
    else
    {
        return nullptr;
    }
}

/**
 * @brief
 * does div, mul, mod
 * @param tokens
 * @return Node*
 */
Node *term(vector<Tokens> &tokens)
{

    // n.value = 0;
    Node *n = new Node;
    Node *opNode = new Node;
    opNode = factor(tokens);
    Tokens *op = (matchAndRemove(tokens, type::MULTIPLY) != nullptr)   ? current
                 : (matchAndRemove(tokens, type::DIVISION) != nullptr) ? current
                 : (matchAndRemove(tokens, type::MOD) != nullptr)      ? current
                 : (matchAndRemove(tokens, type::SLL) != nullptr)      ? current
                 : (matchAndRemove(tokens, type::SRR) != nullptr)      ? current
                 : (matchAndRemove(tokens, type::B_AND) != nullptr)    ? current
                 : (matchAndRemove(tokens, type::B_OR) != nullptr)     ? current
                                                                       : nullptr; // n.value = 0;
    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::MULTIPLY) != nullptr)   ? current
                     : (matchAndRemove(tokens, type::DIVISION) != nullptr) ? current
                     : (matchAndRemove(tokens, type::MOD) != nullptr)      ? current
                     : (matchAndRemove(tokens, type::SLL) != nullptr)      ? current
                     : (matchAndRemove(tokens, type::SRR) != nullptr)      ? current
                     : (matchAndRemove(tokens, type::B_AND) != nullptr)    ? current
                     : (matchAndRemove(tokens, type::B_OR) != nullptr)     ? current
                                                                           : nullptr; // n.value =
            }
            if (op == nullptr)
            {

                return opNode;
            }
            Node *right = new Node;
            right = factor(tokens);
            OperatorNode *n = new OperatorNode;
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
    Tokens *op = (matchAndRemove(tokens, type::ADDITION) != nullptr)   ? current
                 : (matchAndRemove(tokens, type::SUBTRACT) != nullptr) ? current
                                                                       : nullptr; // n.value = 0;
    if (op != nullptr)
    {
        Node *node = nullptr;
        while (true)
        {
            if (node != nullptr)
            {
                op = (matchAndRemove(tokens, type::ADDITION) != nullptr)   ? current
                     : (matchAndRemove(tokens, type::SUBTRACT) != nullptr) ? current
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
            OperatorNode *n = new OperatorNode;
            n->left = new Node;
            n->left = opNode;
            n->right = new Node;
            n->right = right;
            n->token = op;
            // cout << op->buffer << endl;

            opNode = n;
            node = opNode;
            // delete op;
        }
    }
    // if (opNode == nullptr)
    // {
    //     cout << "opNode nulll \n";
    // }
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
        Tokens *e = matchAndRemove(list, type::END_OF_LINE);

        if (e == nullptr)
        {
            return;
        }
    }
}
Tokens *getTypes(vector<Tokens> &tokens)
{
    Tokens *types = (matchAndRemove(tokens, type::FLOAT) != nullptr)    ? current
                    : (matchAndRemove(tokens, type::INT) != nullptr)    ? current
                    : (matchAndRemove(tokens, type::BOOL) != nullptr)   ? current
                    : (matchAndRemove(tokens, type::STRING) != nullptr) ? current
                    : (matchAndRemove(tokens, type::CHAR) != nullptr)   ? current
                                                                        : nullptr;
    return types;
}
// will parse functions
Node *handleFunctions(vector<Tokens> &tokens)
{
    FunctionNode *f = new FunctionNode;
    matchAndRemove(tokens, type::FUNCTION);
    Tokens *name = matchAndRemove(tokens, type::WORD);
    matchAndRemove(tokens, type::OP_PARENTHISIS);

    vector<VaraibleDeclaration *> vars;

    while (matchAndRemove(tokens, type::CL_PARENTHISIS) == nullptr)
    {
        Tokens *word = matchAndRemove(tokens, type::WORD);
        matchAndRemove(tokens, type::SEMI_COLON);
        Tokens *typeVar = getTypes(tokens);
        matchAndRemove(tokens, type::COMMA);
        VaraibleDeclaration *v = new VaraibleDeclaration;
        v->typeOfVar = typeVar;
        v->varaible = word;
        vars.push_back(v);
    }
    f->nameOfFunction = name;
    f->params = vars;
    return f;
}

Node *handleMacros(vector<Tokens> &list)
{
    Tokens *name = matchAndRemove(list, type::WORD);
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
    for (size_t i = 0; i < a.size(); i++)
    {
        cout << a[i]->dictionary[a[i]->id] + "(" + a[i]->buffer + ") \n";
    }
}

Node *parserVarRef(vector<Tokens> &tokens, Tokens *name)
{
    VaraibleReference *var = new VaraibleReference;
    Tokens *DecOp = (matchAndRemove(tokens, type::ADDITION) != nullptr)   ? current
                    : (matchAndRemove(tokens, type::SUBTRACT) != nullptr) ? current
                                                                          : nullptr;

    var->varaible = name;
    if (DecOp != nullptr)
    {
        OperatorNode *n = new OperatorNode;
        IntegerNode *num = new IntegerNode;
        num->num = "1";
        VaraibleReference *vars = new VaraibleReference;
        vars->varaible = name;
        n->left = num;
        n->right = vars;
        n->token = DecOp;
        var->expression = n;
    }
    else
    {
        matchAndRemove(tokens, type::EQUALS);
        var->expression = expression(tokens);
    }
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

Node *parseVar(vector<Tokens> &tokens, Tokens *type, int constant = 0)
{

    Tokens *name = matchAndRemove(tokens, type::WORD);
    VaraibleDeclaration *n = new VaraibleDeclaration;
    n->typeOfVar = type;
    n->varaible = name;
    n->size = 4;
    n->constant = constant;
    matchAndRemove(tokens, type::EQUALS);
    n->expression = expression(tokens);
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
    Node *right = expression(tokens);

    Tokens *op = (matchAndRemove(tokens, type::BOOL_EQ) != nullptr)  ? current
                 : (matchAndRemove(tokens, type::LTE) != nullptr)    ? current
                 : (matchAndRemove(tokens, type::GTE) != nullptr)    ? current
                 : (matchAndRemove(tokens, type::GT) != nullptr)     ? current
                 : (matchAndRemove(tokens, type::LT) != nullptr)     ? current
                 : (matchAndRemove(tokens, type::NOT_EQ) != nullptr) ? current
                                                                     : nullptr;
    Node *left = expression(tokens);
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
    if (matchAndRemove(tokens, type::OP_PARENTHISIS) == nullptr)
    {
        return nullptr;
    }
    vector<Node *> vars;
    while (matchAndRemove(tokens, type::CL_PARENTHISIS) == nullptr)
    {
        vars.push_back(expression(tokens));
        matchAndRemove(tokens, type::COMMA);
    }
    f1->params = vars;
    return f1;
}
Node *handleLoops(vector<Tokens> &tokens)
{
    LoopNode *loop = new LoopNode;
    BoolExpressionNode *a = new BoolExpressionNode;
    matchAndRemove(tokens, type::OP_PARENTHISIS);
    a = handleBooleanExpression(tokens);
    matchAndRemove(tokens, type::CL_PARENTHISIS);
    matchAndRemove(tokens, type::DO);

    vector<Node *> states;

    if (matchAndRemove(tokens, type::BEGIN) == nullptr)
    {
        RemoveEOLS(tokens);
        states.push_back(handleSatements(tokens));
    }
    else
    {
        while (matchAndRemove(tokens, type::END) == nullptr)
        {
            RemoveEOLS(tokens);
            states.push_back(handleSatements(tokens));
            RemoveEOLS(tokens);
        }
    }
    RemoveEOLS(tokens);

    loop->condition = a;
    loop->statements = states;

    return loop;
}

Node *handleIfStatements(vector<Tokens> &tokens)
{
    BoolExpressionNode *a = new BoolExpressionNode;
    matchAndRemove(tokens, type::OP_PARENTHISIS);
    a = handleBooleanExpression(tokens);

    IfSatementNode *ifStatement = new IfSatementNode;
    ifStatement->condition = a;
    if (matchAndRemove(tokens, type::CL_PARENTHISIS) == nullptr && matchAndRemove(tokens, type::THEN) == nullptr)
    {
        cout << "error" << endl;
        exit(EXIT_FAILURE);
        return nullptr;
    }

    vector<Node *> states;
    RemoveEOLS(tokens);
    if (matchAndRemove(tokens, type::BEGIN) != nullptr)
    {
        while (matchAndRemove(tokens, type::END) == nullptr)
        {

            RemoveEOLS(tokens);

            states.push_back(handleSatements(tokens));
            RemoveEOLS(tokens);
        }
    }
    else
    {

        states.push_back(handleSatements(tokens));
        RemoveEOLS(tokens);
    }

    ifStatement->statements = states;

    if (matchAndRemove(tokens, type::ELSE) != nullptr)
    {
        ElseNode *elseNode = new ElseNode;

        vector<Node *> states;
        RemoveEOLS(tokens);

        if (matchAndRemove(tokens, type::BEGIN) == nullptr)
        {
            RemoveEOLS(tokens);
            cout << "test" << endl;
            states.push_back(handleSatements(tokens));
            RemoveEOLS(tokens);
        }
        else
        {
            while (matchAndRemove(tokens, type::END) == nullptr)
            {
                RemoveEOLS(tokens);
                states.push_back(handleSatements(tokens));
                cout << "test" << endl;
                RemoveEOLS(tokens);
            }
        }
        elseNode->statements = states;
        ifStatement->Else = elseNode;
    }
    else
    {
        ifStatement->Else = nullptr;
    }

    return ifStatement;
}

Node *handleReturn(vector<Tokens> &tokens)
{
    ReturnStatment *returns = new ReturnStatment;
    returns->expression = expression(tokens);
    return returns;
}
Node *handleFor(vector<Tokens> &tokens)
{
    ForLoopNode *forLoop = new ForLoopNode;
    matchAndRemove(tokens, type::OP_PARENTHISIS);

    vector<Node *> statements;

    forLoop->incrimentorVar = handleSatements(tokens);
    RemoveEOLS(tokens);
    forLoop->condition = handleBooleanExpression(tokens);
    RemoveEOLS(tokens);

    // matchAndRemove(tokens, type::ELLIPSIS, "elisp");

    // BoolExpressionNode *ex = new BoolExpressionNode;
    // ex->right = vars;

    // ex->left = expression(tokens);

    // Tokens *types = new Tokens;
    // types->id = type::NOT_EQ;
    // ex->op = types;

    Node *b = handleSatements(tokens); // step
    RemoveEOLS(tokens);
    matchAndRemove(tokens, type::CL_PARENTHISIS);

    if (matchAndRemove(tokens, type::BEGIN) != nullptr)
    {
        while (matchAndRemove(tokens, type::END) == nullptr)
        {
            RemoveEOLS(tokens);
            statements.push_back(handleSatements(tokens));

            RemoveEOLS(tokens);
        }
    }
    else
    {
        matchAndRemove(tokens, type::SUBTRACT);
        matchAndRemove(tokens, type::GT);
        statements.push_back(handleSatements(tokens));
        RemoveEOLS(tokens);
    }
    RemoveEOLS(tokens);
    statements.push_back(b);
    forLoop->statements = statements;
    // matchAndRemove(tokens, type::OP_PARENTHISIS, "a");
    // RemoveEOLS(tokens);
    // forLoop->condition = handleBooleanExpression(tokens);
    // RemoveEOLS(tokens);
    // vector<Node *> statements;
    // Node *b = handleSatements(tokens);
    // RemoveEOLS(tokens);
    // matchAndRemove(tokens, type::CL_PARENTHISIS, "a");
    // matchAndRemove(tokens, type::BEGIN, "a");
    // while (matchAndRemove(tokens, type::END, "a") == nullptr)
    // {
    //     RemoveEOLS(tokens);
    //     statements.push_back(handleSatements(tokens));
    //     RemoveEOLS(tokens);
    // }
    // statements.push_back(b);

    // forLoop->statements = statements;
    return forLoop;
}
/**
 * @brief function stuff
 *
 * @param tokens
 * @return Node*
 */
// vector<Node *> block_statements(vector<Tokens> &tokens)
// {
// }
Node *handle_step(vector<Tokens> &tokens)
{
    VaraibleReference *var = new VaraibleReference;
    // Node *word = expression(tokens);
    Tokens *word = matchAndRemove(tokens, type::WORD);
    var->varaible = word;
    OperatorNode *op = new OperatorNode;

    matchAndRemove(tokens, type::COMMA);
    op->left = factor(tokens);
    op->right = var;
    Tokens *toke = new Tokens;
    toke->id = type::ADDITION;
    op->token = toke;
    var->expression = op;
    return var;
}
Node *parse_var_statements(vector<Tokens> &tokens, Tokens *a)
{
    if (a->id == type::WORD)
    {
        Node *functionCall = handleCalls(tokens, a);
        if (functionCall != nullptr)
        {
            return functionCall;
        }
        return parserVarRef(tokens, a);
    }
    else if (a->id == type::CONSTANT)
    {
        a = getTypes(tokens);
        return parseVar(tokens, a, 1);
    }
    else
    {
        return parseVar(tokens, a);
    }
}
Node *handle_array_declaration(vector<Tokens> &tokens)
{
    ArrayDeclaration *array = new ArrayDeclaration;
    matchAndRemove(tokens, type::OP_BRACKET);
    Tokens *type = getTypes(tokens);
    matchAndRemove(tokens, type::SEMI_COLON);
    Node *size = expression(tokens);
    matchAndRemove(tokens, type::CL_BRACKET);
    array->size = size;
    array->typeOfVar = type;
    RemoveEOLS(tokens);
    return array;
}
Node *handleSatements(vector<Tokens> &tokens)
{
#pragma region functionstate
    if (matchAndRemove(tokens, type::MACRO) != nullptr)
    {
        return handleMacros(tokens);
    }
    Tokens *checkIfFunct = (matchAndRemove(tokens, type::PRINT) != nullptr)  ? current
                           : (matchAndRemove(tokens, type::EXIT) != nullptr) ? current
                                                                             : nullptr;
    if (checkIfFunct != nullptr)
    {
        return handleCalls(tokens, checkIfFunct);
    }
#pragma endregion
#pragma region varstates
    Tokens *a = (matchAndRemove(tokens, type::WORD) != nullptr)       ? current
                : (matchAndRemove(tokens, type::CONSTANT) != nullptr) ? current
                : (matchAndRemove(tokens, type::FLOAT) != nullptr)    ? current
                : (matchAndRemove(tokens, type::INT) != nullptr)      ? current
                : (matchAndRemove(tokens, type::BOOL) != nullptr)     ? current
                : (matchAndRemove(tokens, type::STRING) != nullptr)   ? current
                : (matchAndRemove(tokens, type::CHAR) != nullptr)     ? current
                : (matchAndRemove(tokens, type::IF) != nullptr)       ? current
                : (matchAndRemove(tokens, type::LOOP) != nullptr)     ? current
                : (matchAndRemove(tokens, type::FOR_LOOP) != nullptr) ? current
                : (matchAndRemove(tokens, type::STEP) != nullptr)     ? current
                : (matchAndRemove(tokens, type::RETURN) != nullptr)   ? current
                : (matchAndRemove(tokens, type::ARRAY) != nullptr)    ? current
                                                                      : nullptr;

    if (a == nullptr)
    {

        cout << "undefined statement" << endl;
        cout << "" << endl;
        cout << "1st one is the undefined statement" << endl;
        cout << "================" << endl;
        printList(tokens);
        exit(0);
        return nullptr;
    }
    switch (a->id)
    {
    case type::IF:
        return handleIfStatements(tokens);
    case type::LOOP:
        return handleLoops(tokens);
    case type::FOR_LOOP:
        return handleFor(tokens);
    case type::RETURN:
        return handleReturn(tokens);
    case type::STEP:
        return handle_step(tokens);
    case type::ARRAY:
        return handle_array_declaration(tokens);
    default:
        return parse_var_statements(tokens, a);
    }

#pragma endregion
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
    while (matchAndRemove(tokens, type::FUNCTION) != nullptr)
    {

        FunctionNode *f;
        vector<Node *> states;
        Node *func = handleFunctions(tokens);
        FunctionNode *pd = dynamic_cast<FunctionNode *>(func);
        if (matchAndRemove(tokens, type::RETURNS) != nullptr)
        {

            Tokens *return_type = getTypes(tokens);
            pd->returnType = return_type;
        }
        else if (matchAndRemove(tokens, type::SEMI_COLON) != nullptr)
        {
            Tokens *return_type = getTypes(tokens);
            pd->returnType = return_type;
            if (matchAndRemove(tokens, type::SUBTRACT) != nullptr && matchAndRemove(tokens, type::GT) != nullptr)
            {
                states.push_back(handleReturn(tokens));
            }
        }
        else
        {
            pd->returnType = nullptr;
        }
        if (matchAndRemove(tokens, type::BEGIN) != nullptr)
        {

            while (matchAndRemove(tokens, type::END) == nullptr)
            {
                RemoveEOLS(tokens);

                states.push_back(handleSatements(tokens));
                RemoveEOLS(tokens);
            }
        }
        pd->statements = states;
        functionNodes.push_back(pd);
        RemoveEOLS(tokens);
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