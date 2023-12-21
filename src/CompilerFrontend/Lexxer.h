#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <cstdlib>

using namespace std;

#ifndef TYPE_H
#define TYPE_H
enum class type
{
    ADDITION,
    MULTIPLY,
    SUBTRACT,
    DIVISION,
    NUMBER,
    OP_PARENTHISIS,
    CL_PARENTHISIS,
    MOD,
    EQUALS,
    WORD,
    COMMA,
    BEGIN,
    END,
    FUNCTION,
    VAR,
    IF,
    LOOP,
    AND,
    OR,
    END_OF_LINE,
    PRINT,
    SCAN,
    EXIT,
    INT,
    FLOAT,
    STRING,
    MACRO,
    RETURN,
    STRING_LITERAL,
    BOOL_EQ,
    TRUE,
    FALSE,
    LT,
    GT,
    LTE,
    GTE,
    BOOL,
    CONSTANT,
    RETURNS,
    SEMI_COLON,
    ELSE,
    FOR_LOOP,
    CHAR,
    CHAR_LITERAL,
    STEP,
    ELLIPSIS,
    FROM,
    ARRAY,
    OP_BRACKET,
    CL_BRACKET,
    SLL,
    SRR,
    B_AND,
    B_OR,
    THEN,
    DO,
    NOT_EQ
};
#endif // TYPE

#ifndef TOKENS_H
#define TOKENS_H
struct Tokens
{
    string buffer;
    type id;
    map<type, string> dictionary;
    virtual string to_string();
};
#endif // NODE_H

// class Tokens
// {
// public:
//     string buffer;
//     type id;
//     map<type, string> dictionary;
//     Tokens(string buffer, type id); // Constructor
//     string toString();
// };
// string Tokens::toString()
// {

//     return dictionary[id] + "(" + buffer + ")";
// }
void throwException(string message);
vector<string> readFile(string file);
vector<Tokens> lex(vector<string> lines);
void printList(vector<Tokens> a);
void modifyStruct(Tokens &token, type enumType1, map<type, string> dictionary1, string buffer1);
