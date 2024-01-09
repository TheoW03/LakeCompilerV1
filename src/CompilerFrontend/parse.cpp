#include <iostream>
#include <string>
#include <vector>
#include "../../src/CompilerFrontend/Lexxer.h"
#include <typeinfo>
#include <sstream>
#include <memory>

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"

#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"
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
    unique_ptr<Node> right;
    unique_ptr<Node> left;
    virtual ~Node();
    virtual llvm::Value *Codegen();
};

struct VaraibleDeclaration : public Node
{
    unique_ptr<Node> expression;
    Tokens varaible;
    Tokens typeOfVar;
    int size;
    int constant;
};

struct CastNode : public Node
{
    Tokens type;
    unique_ptr<Node> expression;
};
struct FloatNode : public Node
{
    string num;
};
struct IntegerNode : public Node
{
    string num;
    virtual llvm::Value *Codegen();
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
    optional<Tokens> op;
    int next_is_or;
};

struct ElseNode : public Node
{

    vector<shared_ptr<Node>> statements;
};
struct IfSatementNode : public Node
{

    unique_ptr<BoolExpressionNode> condition;
    vector<shared_ptr<Node>> statements;
    unique_ptr<ElseNode> Else;
};
struct VaraibleReference : public Node
{
    unique_ptr<Node> expression;
    Tokens varaible;
};
struct ForLoopNode : public Node
{
    unique_ptr<Node> incrimentorVar;
    unique_ptr<BoolExpressionNode> condition;
    vector<shared_ptr<Node>> statements;
};

/**
 * @brief this gives an expression, left, rigt
 *
 */
struct OperatorNode : public Node
{
    Tokens token;
    virtual llvm::Value *Codegen();
};

struct ArrayDeclaration : public Node
{
    unique_ptr<Node> size;
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
    unique_ptr<BoolExpressionNode> condition;
    vector<shared_ptr<Node>> statements;
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
    vector<shared_ptr<VaraibleDeclaration>> params;
    vector<shared_ptr<Node>> statements;
    string hashed_functionName; // the name that is asm
    optional<Tokens> returnType;
};
#pragma endregion
unique_ptr<Node> expression(vector<Tokens> &tokens);
#pragma region ignore

#pragma endregion

optional<Tokens> current;
unique_ptr<Node> handleSatements(vector<Tokens> &tokens);
optional<Tokens> getTypes(vector<Tokens> &tokens);
unique_ptr<Node> handleCalls(vector<Tokens> &tokens, Tokens checkIfFunct);

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
        if (getTypes(tokens).has_value())
        {
            unique_ptr<CastNode> cast = make_unique<CastNode>();
            cast->type = current.value();
            matchAndRemove(tokens, type::CL_PARENTHISIS);
            cast->expression = factor(tokens);
            return cast;
        }
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
        unique_ptr<Node> a = move(handleCalls(tokens, current.value()));

        if (a == nullptr)
        {
            unique_ptr<VaraibleReference> var = make_unique<VaraibleReference>();
            var->varaible = current.value();
            return var;
        }
        return a;
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
    // wip

    // else if(matchAndRemove(tokens, type::SIZE_OF))
    //{
    //      matchAndRemove(tokens, type::OP_PARENTHESIS);
    //
    //	if(getTypes(tokens).has_value()){
    //
    //     }
    // }
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
        n->token = op.value();

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
        n->token = op.value();

        opNode = move(n);
        op = (matchAndRemove(tokens, type::ADDITION).has_value())   ? current
             : (matchAndRemove(tokens, type::SUBTRACT).has_value()) ? current
                                                                    : nullopt;
    }

    return opNode;
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
unique_ptr<FunctionNode> handleFunctions(vector<Tokens> &tokens)
{
    unique_ptr<FunctionNode> f = make_unique<FunctionNode>();
    optional<Tokens> name = matchAndRemove(tokens, type::WORD);
    f->nameOfFunction = name.value();
    string function_nameHashed = name.value().buffer;
    f->hashed_functionName = function_nameHashed + "_lacus";
    matchAndRemove(tokens, type::OP_PARENTHISIS);

    vector<shared_ptr<VaraibleDeclaration>> vars;

    while (!matchAndRemove(tokens, type::CL_PARENTHISIS).has_value())
    {
        optional<Tokens> word = matchAndRemove(tokens, type::WORD);
        matchAndRemove(tokens, type::SEMI_COLON);
        optional<Tokens> typeVar = getTypes(tokens);
        matchAndRemove(tokens, type::COMMA);
        unique_ptr<VaraibleDeclaration> v = make_unique<VaraibleDeclaration>();
        v->typeOfVar = typeVar.value();
        v->varaible = word.value();
        vars.push_back(move(v));
    }
    f->params = (vars);
    return f;
}

unique_ptr<Node> parserVarRef(vector<Tokens> &tokens, Tokens name)
{
    unique_ptr<VaraibleReference> var = make_unique<VaraibleReference>();
    var->varaible = name;
    matchAndRemove(tokens, type::EQUALS);
    var->expression = expression(tokens);
    return var;
}

unique_ptr<VaraibleDeclaration> parseVar(vector<Tokens> &tokens,
                                         Tokens type, int constant = 0)
{

    Tokens name = matchAndRemove(tokens, type::WORD).value();
    unique_ptr<VaraibleDeclaration> n = make_unique<VaraibleDeclaration>();
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
unique_ptr<BoolExpressionNode> handleBooleanExpression(vector<Tokens> &tokens)
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
    unique_ptr<BoolExpressionNode> a = make_unique<BoolExpressionNode>();
    a->right = move(right);
    a->left = move(left);
    a->op = op.value();
    return a;
}
unique_ptr<Node> BoolExpr(vector<Tokens> &tokens);
unique_ptr<Node> BoolFactor(vector<Tokens> &tokens)
{
    unique_ptr<Node> c = factor(tokens);
    if (c != nullptr)
    {
        return c;
    }
    if (matchAndRemove(tokens, type::OP_PARENTHISIS).has_value())
    {
        unique_ptr<Node> b = BoolExpr(tokens);
        matchAndRemove(tokens, type::CL_PARENTHISIS);
        return b;
    }
    return nullptr;
}
unique_ptr<Node> BoolExpr(vector<Tokens> &tokens)
{
    unique_ptr<Node> left = BoolFactor(tokens);
    optional<Tokens> op = (matchAndRemove(tokens, type::BOOL_EQ).has_value())  ? current
                          : (matchAndRemove(tokens, type::LTE).has_value())    ? current
                          : (matchAndRemove(tokens, type::GTE).has_value())    ? current
                          : (matchAndRemove(tokens, type::GT).has_value())     ? current
                          : (matchAndRemove(tokens, type::LT).has_value())     ? current
                          : (matchAndRemove(tokens, type::AND).has_value())    ? current
                          : (matchAndRemove(tokens, type::OR).has_value())     ? current
                          : (matchAndRemove(tokens, type::NOT_EQ).has_value()) ? current
                                                                               : nullopt;

    while (op.has_value())
    {

        unique_ptr<BoolExpressionNode> n = make_unique<BoolExpressionNode>();

        n->op = op.value();
        if (op.value().id == type::AND || op.value().id == type::OR)
        {
            n->right = BoolExpr(tokens);
        }
        else
        {
            n->right = BoolFactor(tokens);
        }

        n->left = move(left);
        left = move(n);
        op = (matchAndRemove(tokens, type::BOOL_EQ).has_value())  ? current
             : (matchAndRemove(tokens, type::LTE).has_value())    ? current
             : (matchAndRemove(tokens, type::GTE).has_value())    ? current
             : (matchAndRemove(tokens, type::GT).has_value())     ? current
             : (matchAndRemove(tokens, type::LT).has_value())     ? current
             : (matchAndRemove(tokens, type::AND).has_value())    ? current
             : (matchAndRemove(tokens, type::OR).has_value())     ? current
             : (matchAndRemove(tokens, type::NOT_EQ).has_value()) ? current
                                                                  : nullopt;
    }
    return left;
}

unique_ptr<Node> test(vector<Tokens> &tokens)
{
    return BoolExpr(tokens);
}

unique_ptr<Node> handleCalls(vector<Tokens> &tokens, Tokens checkIfFunct)
{
    // funcCallNode *f1 = new funcCallNode;
    unique_ptr<funcCallNode> f1 = make_unique<funcCallNode>();
    f1->funcCall = checkIfFunct;
    if (!matchAndRemove(tokens, type::OP_PARENTHISIS).has_value())
    {
        return nullptr;
    }
    vector<unique_ptr<Node>> vars;
    while (!matchAndRemove(tokens, type::CL_PARENTHISIS).has_value())
    {
        vars.push_back(move(expression(tokens)));
        matchAndRemove(tokens, type::COMMA);
    }
    f1->params = move(vars);
    return f1;
}
unique_ptr<Node> handleLoops(vector<Tokens> &tokens)
{
    unique_ptr<LoopNode> loop = make_unique<LoopNode>();
    matchAndRemove(tokens, type::OP_PARENTHISIS);
    loop->condition = handleBooleanExpression(tokens);
    matchAndRemove(tokens, type::CL_PARENTHISIS);
    matchAndRemove(tokens, type::DO);

    vector<shared_ptr<Node>> states;

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

unique_ptr<Node> handleIfStatements(vector<Tokens> &tokens)
{
    matchAndRemove(tokens, type::OP_PARENTHISIS);
    unique_ptr<IfSatementNode> ifStatement = make_unique<IfSatementNode>();
    unique_ptr<BoolExpressionNode> a = move(handleBooleanExpression(tokens));
    ifStatement->condition = move(a);
    if (!matchAndRemove(tokens, type::CL_PARENTHISIS).has_value() &&
        !matchAndRemove(tokens, type::THEN).has_value())
    {
        cout << "error" << endl;
        exit(EXIT_FAILURE);

        return nullptr;
    }

    vector<shared_ptr<Node>> states;
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

    ifStatement->statements = move(states);

    if (matchAndRemove(tokens, type::ELSE).has_value())
    {
        unique_ptr<ElseNode> elseNode = make_unique<ElseNode>();
        vector<shared_ptr<Node>> states;
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
                // cout << "test" << endl;
                RemoveEOLS(tokens);
            }
        }
        elseNode->statements = move(states);
        ifStatement->Else = move(elseNode);
    }
    else
    {
        ifStatement->Else = nullptr;
    }

    return ifStatement;
}

unique_ptr<Node> handleReturn(vector<Tokens> &tokens)
{
    unique_ptr<ReturnStatment> returns = make_unique<ReturnStatment>();
    returns->expression = expression(tokens);
    return returns;
}
unique_ptr<ForLoopNode> handleFor(vector<Tokens> &tokens)
{
    unique_ptr<ForLoopNode> forLoop = make_unique<ForLoopNode>();
    matchAndRemove(tokens, type::OP_PARENTHISIS);

    vector<shared_ptr<Node>> statements;
    forLoop->incrimentorVar = move(handleSatements(tokens));
    RemoveEOLS(tokens);
    forLoop->condition = move(handleBooleanExpression(tokens));
    RemoveEOLS(tokens);
    unique_ptr<Node> b = move(handleSatements(tokens)); // step
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
    statements.push_back(move(b));
    forLoop->statements = move(statements);
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
unique_ptr<Node> handle_step(vector<Tokens> &tokens)
{

    // VaraibleReference *var = new VaraibleReference;
    unique_ptr<VaraibleReference> var = make_unique<VaraibleReference>();
    unique_ptr<VaraibleReference> var2 = make_unique<VaraibleReference>();

    // // Node *word = expression(tokens);
    Tokens word = matchAndRemove(tokens, type::WORD).value();
    var->varaible = word;
    var2->varaible = word;
    unique_ptr<OperatorNode> op = make_unique<OperatorNode>();
    matchAndRemove(tokens, type::COMMA);
    op->left = move(expression(tokens));
    op->right = move(var2);
    Tokens toke;
    toke.id = type::ADDITION;
    op->token = toke;
    var->expression = move(op);
    return var;
}
unique_ptr<Node> parse_arr_Ref(vector<Tokens> &tokens, Tokens name)
{
    unique_ptr<ArrayRef> arrayRef = make_unique<ArrayRef>();
    arrayRef->name = name;
    arrayRef->RefedLocation = expression(tokens);
    matchAndRemove(tokens, type::CL_BRACKET);
    matchAndRemove(tokens, type::EQUALS);
    arrayRef->value = expression(tokens);
    return arrayRef;
}

unique_ptr<Node> parse_var_statements(vector<Tokens> &tokens, Tokens a)
{
    if (a.id == type::WORD)
    {
        unique_ptr<Node> functionCall = move(handleCalls(tokens, a));
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

unique_ptr<Node> handle_array_declaration(vector<Tokens> &tokens)
{
    unique_ptr<ArrayDeclaration> array = make_unique<ArrayDeclaration>();
    matchAndRemove(tokens, type::OP_BRACKET);
    Tokens type = getTypes(tokens).value();

    matchAndRemove(tokens, type::SEMI_COLON);

    unique_ptr<Node> size = move(expression(tokens));
    matchAndRemove(tokens, type::CL_BRACKET);
    array->size = move(size);
    array->typeOfVar = type;
    array->varaible = matchAndRemove(tokens, type::WORD).value();
    //  if(matchAndRemove(tokens, type::EQUALS).had_value){
    //
    //  } //for later :3c
    RemoveEOLS(tokens);
    return array;
}
unique_ptr<Node> handleSatements(vector<Tokens> &tokens)
{

#pragma region functionstate
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
vector<unique_ptr<FunctionNode>> functionParse(vector<Tokens> &tokens)
{

    vector<unique_ptr<FunctionNode>> functionNodes;
    while (matchAndRemove(tokens, type::FUNCTION).has_value())
    {

        vector<shared_ptr<Node>> states;
        unique_ptr<FunctionNode> pd = handleFunctions(tokens);
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
        pd->statements = move(states);
        functionNodes.push_back(move(pd));
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

vector<unique_ptr<FunctionNode>> parse(vector<Tokens> &tokens)
{

    return functionParse(tokens);
}
#pragma endregion
