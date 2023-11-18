#include <iostream>
#include <string>
#include <vector>
#include "../../src/CompilerFrontend/Lexxer.h"
#include <typeinfo>
#include <sstream>
#include <memory>

#include <optional>

#define OFFSET 65536

using namespace std;

#pragma region Node

/**
 * @brief C++ OOP is icky so
 *
 */

struct Node
{
    virtual ~Node();
    // Node *left;
    // Node *right;

    unique_ptr<Node> right;
    unique_ptr<Node> left;
};

struct VaraibleDeclaration : public Node
{
    unique_ptr<Node> expression;
    Tokens varaible;
    Tokens typeOfVar;
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
    Tokens value;
};
struct ReturnStatment : public Node
{
    unique_ptr<Node> expression;
};
struct BoolExpressionNode : public Node
{
    // Node *right;
    // Node *left;
    optional<Tokens> op;
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
    unique_ptr<Node> expression;
    Tokens varaible;
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
    Tokens token;
};

struct StatementNode : public Node
{
    struct Node *expression;
    struct Tokens nameOfVar;
};
struct ArrayDeclaration : public Node
{
    Node *size;
    Tokens varaible;
    Tokens typeOfVar;
};
struct ArrayRef : public Node
{
    Tokens name;
    unique_ptr<Node> RefedLocation;
    unique_ptr<Node> value;
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
    Tokens funcCall;
    vector<unique_ptr<Node>> params;
};
struct MacroNode : public Node
{
    Tokens macro;
    Node *statement;
};
/**
 * @brief
 * function main(a,b){}
 *
 */
struct FunctionNode : public Node
{
    Tokens nameOfFunction;
    // string hashed_functioName; // the name that is asm
    vector<VaraibleDeclaration *> params;
    vector<Node *> statements;
    optional<Tokens> returnType;
};
#pragma endregion
unique_ptr<Node> expression(vector<Tokens> &tokens);
#pragma region ignore

#pragma endregion

optional<Tokens> current;
Node *handleSatements(vector<Tokens> &tokens);
Node *handleCalls(vector<Tokens> &tokens, Tokens checkIfFunct);

/**
 * @brief I could use a stack, but a stack coesnt have peek lol
 * it returns the 1st token, if the neum is equal return else.
 *
 * @param tokens
 * @param typeT
 * @param caller
 * @return Tokens*
 */

optional<Tokens> matchAndRemove(vector<Tokens> &tokens, type typeT)
{
    if (tokens.empty())
    {
        return nullopt;
    }
    if (tokens[0].id == typeT)
    {

        Tokens t = tokens[0];
        current = t;
        tokens.erase(tokens.begin());
        return t;
    }
    return nullopt;
}
#pragma region Expression term and factor (for equations)
/**
 * @brief does Number, var, anything thats not +-/%
 * if number or var returns a nod for them else does recursion
 *
 * @param tokens
 * @return Node*
 */

unique_ptr<Node> factor(vector<Tokens> &tokens)
{

    if (matchAndRemove(tokens, type::NUMBER).has_value())
    {
        string myString = current.value().buffer;
        if (myString.find(".") == string::npos)
        {
            unique_ptr<IntegerNode> intNode = make_unique<IntegerNode>();
            // IntegerNode *intNode = new IntegerNode;
            intNode->num = current.value().buffer;
            return intNode;
        }
        int fPoint = (int)(stof(myString) * OFFSET);
        unique_ptr<FloatNode> floatNode = make_unique<FloatNode>();
        floatNode->num = to_string(fPoint);
        return floatNode;
    }
    else if (matchAndRemove(tokens, type::OP_PARENTHISIS).has_value())
    {
        unique_ptr<Node> exp = expression(tokens);
        matchAndRemove(tokens, type::CL_PARENTHISIS);
        return exp;
    }
    else if (matchAndRemove(tokens, type::STRING_LITERAL).has_value())
    {
        unique_ptr<StringNode> s = make_unique<StringNode>();
        s->stringBuffer = current.value().buffer;
        return s;
    }
    else if (matchAndRemove(tokens, type::WORD).has_value())
    {
        if (matchAndRemove(tokens, type::OP_BRACKET).has_value())
        {

            unique_ptr<ArrayRef> arrayRef = make_unique<ArrayRef>();
            arrayRef->name = current.value();
            arrayRef->RefedLocation = expression(tokens);
            matchAndRemove(tokens, type::CL_BRACKET);
            return arrayRef;
        }
        // Node *a = handleCalls(tokens, current.value());

        // if (a == nullptr)
        // {
        // VaraibleReference *var = new VaraibleReference;
        unique_ptr<VaraibleReference> var = make_unique<VaraibleReference>();
        var->varaible = current.value();
        return var;
        // }
        // return a;
    }
    else if (matchAndRemove(tokens, type::TRUE).has_value() || matchAndRemove(tokens, type::FALSE).has_value())
    {

        unique_ptr<BooleanLiteralNode> boolean = make_unique<BooleanLiteralNode>();
        boolean->value = current.value();
        return boolean;
    }
    else if (matchAndRemove(tokens, type::CHAR_LITERAL).has_value())
    {

        // CharNode *integer = new CharNode;
        unique_ptr<CharNode> integer = make_unique<CharNode>();
        string s = current.value().buffer;
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
unique_ptr<Node> term(vector<Tokens> &tokens)
{

    unique_ptr<Node> opNode = factor(tokens);
    optional<Tokens> op = (matchAndRemove(tokens, type::MULTIPLY).has_value())   ? current
                          : (matchAndRemove(tokens, type::DIVISION).has_value()) ? current
                          : (matchAndRemove(tokens, type::MOD).has_value())      ? current
                          : (matchAndRemove(tokens, type::SLL).has_value())      ? current
                          : (matchAndRemove(tokens, type::SRR).has_value())      ? current
                          : (matchAndRemove(tokens, type::B_AND).has_value())    ? current
                          : (matchAndRemove(tokens, type::B_OR).has_value())     ? current
                                                                                 : nullopt;

    while (op.has_value())
    {
        auto n = make_unique<OperatorNode>();
        n->left = move(opNode);
        n->right = factor(tokens);
        opNode = move(n);
        op = (matchAndRemove(tokens, type::MULTIPLY).has_value())   ? current
             : (matchAndRemove(tokens, type::DIVISION).has_value()) ? current
             : (matchAndRemove(tokens, type::MOD).has_value())      ? current
             : (matchAndRemove(tokens, type::SLL).has_value())      ? current
             : (matchAndRemove(tokens, type::SRR).has_value())      ? current
             : (matchAndRemove(tokens, type::B_AND).has_value())    ? current
             : (matchAndRemove(tokens, type::B_OR).has_value())     ? current
                                                                    : nullopt;
    }
    return opNode;
}

/**
 * @brief +,-
 *
 * @param tokens
 * @return Node*
 */
unique_ptr<Node> expression(vector<Tokens> &tokens)
{
    unique_ptr<Node> opNode = term(tokens);
    optional<Tokens> op = (matchAndRemove(tokens, type::ADDITION).has_value())   ? current
                          : (matchAndRemove(tokens, type::SUBTRACT).has_value()) ? current
                                                                                 : nullopt;
    while (op.has_value())
    {

        auto n = make_unique<OperatorNode>();
        n->left = move(opNode);
        n->right = term(tokens);
        opNode = move(n);
        op = (matchAndRemove(tokens, type::ADDITION).has_value())   ? current
             : (matchAndRemove(tokens, type::SUBTRACT).has_value()) ? current
                                                                    : nullopt;
    }

    return opNode;
}
// unique_ptr<Node> safe_expression(vector<Tokens> &tokens);
// unique_ptr<Node> safe_factor(vector<Tokens> &tokens)
// {
//     if (matchAndRemove(tokens, type::NUMBER).has_value())
//     {
//         auto intNode = make_unique<IntegerNode>();
//         intNode->num = current.value().buffer;
//         return intNode;
//     }
//     else if (matchAndRemove(tokens, type::OP_PARENTHISIS).has_value())
//     {
//         unique_ptr<Node> exp = safe_expression(tokens);
//         matchAndRemove(tokens, type::CL_PARENTHISIS);
//         return exp;
//     }
//     return nullptr;
// }
// unique_ptr<Node> safe_term(vector<Tokens> &tokens)
// {
//     unique_ptr<Node> opNode = safe_factor(tokens);
//     optional<Tokens> op = (matchAndRemove(tokens, type::MULTIPLY).has_value())   ? current
//                           : (matchAndRemove(tokens, type::DIVISION).has_value()) ? current
//                           : (matchAndRemove(tokens, type::MOD).has_value())      ? current
//                           : (matchAndRemove(tokens, type::SLL).has_value())      ? current
//                           : (matchAndRemove(tokens, type::SRR).has_value())      ? current
//                           : (matchAndRemove(tokens, type::B_AND).has_value())    ? current
//                           : (matchAndRemove(tokens, type::B_OR).has_value())     ? current
//                                                                                  : nullopt;

//     while (op.has_value())
//     {
//         auto n = make_unique<OperatorNode>();
//         n->safe_left = move(opNode);
//         n->safe_right = safe_factor(tokens);
//         n->token = op.value();
//         opNode = move(n);
//         op = (matchAndRemove(tokens, type::MULTIPLY).has_value())   ? current
//              : (matchAndRemove(tokens, type::DIVISION).has_value()) ? current
//              : (matchAndRemove(tokens, type::MOD).has_value())      ? current
//              : (matchAndRemove(tokens, type::SLL).has_value())      ? current
//              : (matchAndRemove(tokens, type::SRR).has_value())      ? current
//              : (matchAndRemove(tokens, type::B_AND).has_value())    ? current
//              : (matchAndRemove(tokens, type::B_OR).has_value())     ? current
//                                                                     : nullopt;
//     }

//     return opNode;
// }
// unique_ptr<Node> safe_expression(vector<Tokens> &tokens)
// {

//     unique_ptr<Node> opNode = safe_term(tokens);
//     optional<Tokens> op = (matchAndRemove(tokens, type::ADDITION).has_value())   ? current
//                           : (matchAndRemove(tokens, type::SUBTRACT).has_value()) ? current
//                                                                                  : nullopt;

//     while (op.has_value())
//     {
//         auto n = make_unique<OperatorNode>();
//         n->safe_left = move(opNode);
//         n->safe_right = safe_term(tokens);
//         n->token = op.value();
//         opNode = move(n);
//         op = (matchAndRemove(tokens, type::ADDITION).has_value())   ? current
//              : (matchAndRemove(tokens, type::SUBTRACT).has_value()) ? current
//                                                                     : nullopt;
//     }

//     return opNode;
// }
// unique_ptr<Node> safe_parse(vector<Tokens> &tokens)
// {
//     return safe_expression(tokens);
// }
#pragma endregion
#pragma region statements
/**
 * @brief helper function for 'l'
 *
 * @param list
 */
void RemoveEOLS(vector<Tokens> &list)
{
    while (matchAndRemove(list, type::END_OF_LINE).has_value())
        ;
}
optional<Tokens> getTypes(vector<Tokens> &tokens)
{

    return (matchAndRemove(tokens, type::FLOAT).has_value())    ? current
           : (matchAndRemove(tokens, type::INT).has_value())    ? current
           : (matchAndRemove(tokens, type::BOOL).has_value())   ? current
           : (matchAndRemove(tokens, type::STRING).has_value()) ? current
           : (matchAndRemove(tokens, type::CHAR).has_value())   ? current
                                                                : nullopt;
}
// will parse functions
FunctionNode *handleFunctions(vector<Tokens> &tokens)
{
    FunctionNode *f = new FunctionNode;
    optional<Tokens> name = matchAndRemove(tokens, type::WORD);
    f->nameOfFunction = name.value();
    string function_nameHashed = name.value().buffer;
    // f->hashed_functioName = function_nameHashed + "_lacus";
    matchAndRemove(tokens, type::OP_PARENTHISIS);

    vector<VaraibleDeclaration *> vars;

    while (!matchAndRemove(tokens, type::CL_PARENTHISIS).has_value())
    {
        optional<Tokens> word = matchAndRemove(tokens, type::WORD);
        matchAndRemove(tokens, type::SEMI_COLON);
        optional<Tokens> typeVar = getTypes(tokens);
        matchAndRemove(tokens, type::COMMA);
        VaraibleDeclaration *v = new VaraibleDeclaration;
        v->typeOfVar = typeVar.value();
        v->varaible = word.value();
        vars.push_back(v);
    }
    f->params = vars;
    return f;
}

// Node *handleMacros(vector<Tokens> &list)
// {
//     optional<Tokens> name = matchAndRemove(list, type::WORD);
//     Node *statements;
//     statements = expression(list);
//     if (statements == nullptr)
//     {
//         statements = handleSatements(list);
//     }
//     MacroNode *a = new MacroNode;
//     a->macro = name.value();
//     a->statement = statements;
//     RemoveEOLS(list);
//     return a;
// }
void printParams(vector<Tokens *> a)
{
    cout << "params" << endl;
    for (size_t i = 0; i < a.size(); i++)
    {
        cout << a[i]->dictionary[a[i]->id] + "(" + a[i]->buffer + ") \n";
    }
}

Node *parserVarRef(vector<Tokens> &tokens, Tokens name)
{
    VaraibleReference *var = new VaraibleReference;
    var->varaible = name;
    matchAndRemove(tokens, type::EQUALS);
    var->expression = expression(tokens);
    return var;
}
// /**
//  * @brief just me testing, ignore
//  *
//  * @param tokens
//  * @return Node*
//  */
// Node *testParse(vector<Tokens> &tokens)
// {
//     FunctionNode *f = static_cast<FunctionNode *>(handleFunctions(tokens));
//     // printParams(f->params);
//     return f;
// }

Node *parseVar(vector<Tokens> &tokens, Tokens type, int constant = 0)
{

    Tokens name = matchAndRemove(tokens, type::WORD).value();
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
    unique_ptr<Node> right = expression(tokens);

    optional<Tokens> op = (matchAndRemove(tokens, type::BOOL_EQ).has_value())  ? current
                          : (matchAndRemove(tokens, type::LTE).has_value())    ? current
                          : (matchAndRemove(tokens, type::GTE).has_value())    ? current
                          : (matchAndRemove(tokens, type::GT).has_value())     ? current
                          : (matchAndRemove(tokens, type::LT).has_value())     ? current
                          : (matchAndRemove(tokens, type::NOT_EQ).has_value()) ? current
                                                                               : nullopt;
    unique_ptr<Node> left = expression(tokens);
    BoolExpressionNode *a = new BoolExpressionNode;
    a->right = move(right);
    a->left = move(left);
    a->op = op;

    return a;
}

Node *handleCalls(vector<Tokens> &tokens, Tokens checkIfFunct)
{
    funcCallNode *f1 = new funcCallNode;
    f1->funcCall = checkIfFunct;
    if (!matchAndRemove(tokens, type::OP_PARENTHISIS).has_value())
    {
        delete f1;
        return nullptr;
    }
    vector<unique_ptr<Node>> vars;
    while (!matchAndRemove(tokens, type::CL_PARENTHISIS).has_value())
    {
        vars.push_back(move(expression(tokens)));
        matchAndRemove(tokens, type::COMMA);
    }
    // f1->params = move(vars);
    f1->params = move(vars);
    return f1;
}
Node *handleLoops(vector<Tokens> &tokens)
{
    LoopNode *loop = new LoopNode;
    matchAndRemove(tokens, type::OP_PARENTHISIS);
    loop->condition = handleBooleanExpression(tokens);
    matchAndRemove(tokens, type::CL_PARENTHISIS);
    matchAndRemove(tokens, type::DO);

    vector<Node *> states;

    if (!matchAndRemove(tokens, type::BEGIN).has_value())
    {
        RemoveEOLS(tokens);
        states.push_back(handleSatements(tokens));
    }
    else
    {
        while (!matchAndRemove(tokens, type::END).has_value())
        {
            RemoveEOLS(tokens);
            states.push_back(handleSatements(tokens));
            RemoveEOLS(tokens);
        }
    }
    RemoveEOLS(tokens);

    loop->statements = states;

    return loop;
}

Node *handleIfStatements(vector<Tokens> &tokens)
{
    matchAndRemove(tokens, type::OP_PARENTHISIS);
    IfSatementNode *ifStatement = new IfSatementNode;
    BoolExpressionNode *a = handleBooleanExpression(tokens);
    ifStatement->condition = a;
    if (!matchAndRemove(tokens, type::CL_PARENTHISIS).has_value() && !matchAndRemove(tokens, type::THEN).has_value())
    {
        delete ifStatement;
        cout << "error" << endl;
        exit(EXIT_FAILURE);

        return nullptr;
    }

    vector<Node *> states;
    RemoveEOLS(tokens);
    if (matchAndRemove(tokens, type::BEGIN).has_value())
    {
        while (!matchAndRemove(tokens, type::END).has_value())
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

    if (matchAndRemove(tokens, type::ELSE).has_value())
    {
        ElseNode *elseNode = new ElseNode;

        vector<Node *> states;
        RemoveEOLS(tokens);

        if (!matchAndRemove(tokens, type::BEGIN).has_value())
        {
            RemoveEOLS(tokens);
            states.push_back(handleSatements(tokens));
            RemoveEOLS(tokens);
        }
        else
        {
            while (!matchAndRemove(tokens, type::END).has_value())
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
    Node *b = handleSatements(tokens); // step
    RemoveEOLS(tokens);
    matchAndRemove(tokens, type::CL_PARENTHISIS);

    if (matchAndRemove(tokens, type::BEGIN).has_value())
    {
        while (!matchAndRemove(tokens, type::END).has_value())
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

    // forLoop->statements = ;
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
    unique_ptr<VaraibleReference> var2 = make_unique<VaraibleReference>();

    // Node *word = expression(tokens);
    Tokens word = matchAndRemove(tokens, type::WORD).value();
    var->varaible = word;
    var2->varaible = word;
    // OperatorNode *op = new OperatorNode;
    unique_ptr<OperatorNode> op = make_unique<OperatorNode>();

    matchAndRemove(tokens, type::COMMA);
    op->left = factor(tokens);
    op->right = move(var2);
    Tokens toke;
    toke.id = type::ADDITION;
    op->token = toke;
    var->expression = move(op);
    return var;
}
Node *parse_arr_Ref(vector<Tokens> &tokens, Tokens name)
{
    ArrayRef *arrayRef = new ArrayRef;
    arrayRef->name = name;
    arrayRef->RefedLocation = expression(tokens);
    matchAndRemove(tokens, type::CL_BRACKET);
    matchAndRemove(tokens, type::EQUALS);
    arrayRef->value = expression(tokens);
    return arrayRef;
}

Node *parse_var_statements(vector<Tokens> &tokens, Tokens a)
{
    if (a.id == type::WORD)
    {
        Node *functionCall = handleCalls(tokens, a);
        if (functionCall != nullptr)
        {
            return functionCall;
        }
        if (matchAndRemove(tokens, type::OP_BRACKET).has_value())
        {
            return parse_arr_Ref(tokens, a);
        }
        return parserVarRef(tokens, a);
    }
    else if (a.id == type::CONSTANT)
    {
        a = getTypes(tokens).value();
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
    // matchAndRemove(tokens, type::OP_BRACKET);
    // Tokens type = getTypes(tokens).value();
    // matchAndRemove(tokens, type::SEMI_COLON);
    // Node *size = expression(tokens);
    // matchAndRemove(tokens, type::CL_BRACKET);
    // array->size = size;
    // array->typeOfVar = type;
    // array->varaible = matchAndRemove(tokens, type::WORD).value();
    // RemoveEOLS(tokens);
    return array;
}
Node *handleSatements(vector<Tokens> &tokens)
{
#pragma region functionstate
    //     if (matchAndRemove(tokens, type::MACRO).has_value())
    //     {
    //         return handleMacros(tokens);
    //     }
    optional<Tokens> checkIfFunct = (matchAndRemove(tokens, type::PRINT).has_value())  ? current
                                    : (matchAndRemove(tokens, type::EXIT).has_value()) ? current
                                                                                       : nullopt;
    if (checkIfFunct.has_value())
        return handleCalls(tokens, checkIfFunct.value());

#pragma endregion
#pragma region varstates
    optional<Tokens> a = (matchAndRemove(tokens, type::WORD).has_value())       ? current
                         : (matchAndRemove(tokens, type::CONSTANT).has_value()) ? current
                         : (matchAndRemove(tokens, type::FLOAT).has_value())    ? current
                         : (matchAndRemove(tokens, type::INT).has_value())      ? current
                         : (matchAndRemove(tokens, type::BOOL).has_value())     ? current
                         : (matchAndRemove(tokens, type::STRING).has_value())   ? current
                         : (matchAndRemove(tokens, type::CHAR).has_value())     ? current
                         : (matchAndRemove(tokens, type::IF).has_value())       ? current
                         : (matchAndRemove(tokens, type::LOOP).has_value())     ? current
                         : (matchAndRemove(tokens, type::FOR_LOOP).has_value()) ? current
                         : (matchAndRemove(tokens, type::STEP).has_value())     ? current
                         : (matchAndRemove(tokens, type::RETURN).has_value())   ? current
                         : (matchAndRemove(tokens, type::ARRAY).has_value())    ? current
                                                                                : nullopt;

    if (!a.has_value())
    {

        cout << "undefined statement" << endl;
        cout << "" << endl;
        cout << "1st one is the undefined statement" << endl;
        cout << "================" << endl;
        printList(tokens);
        exit(0);
        return nullptr;
    }
    switch (a.value().id)
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
        return parse_var_statements(tokens, a.value());
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
    while (matchAndRemove(tokens, type::FUNCTION).has_value())
    {

        vector<Node *> states;
        FunctionNode *pd = handleFunctions(tokens);
        if (matchAndRemove(tokens, type::RETURNS).has_value())
        {

            pd->returnType = getTypes(tokens);
        }
        else if (matchAndRemove(tokens, type::SEMI_COLON).has_value())
        {
            pd->returnType = getTypes(tokens);
            if (matchAndRemove(tokens, type::SUBTRACT).has_value() && matchAndRemove(tokens, type::GT).has_value())
            {
                states.push_back(handleReturn(tokens));
            }
        }
        else
        {
            pd->returnType = nullopt;
        }
        if (matchAndRemove(tokens, type::BEGIN).has_value())
        {

            while (!matchAndRemove(tokens, type::END).has_value())
            {
                RemoveEOLS(tokens);

                states.push_back(handleSatements(tokens));
                RemoveEOLS(tokens);
            }
        }
        pd->statements = states;
        functionNodes.push_back(pd);
        // delete pd;
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
unique_ptr<Node> testExpressionParse(vector<Tokens> &tokens)
{
    return expression(tokens);
}
/**
 */
// unique_ptr<Node> test_Expression(vector<Tokens> &tokens)
// {

// }

vector<FunctionNode *> parse(vector<Tokens> &tokens)
{

    return functionParse(tokens);
}
#pragma endregion