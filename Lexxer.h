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
enum type
{
    ADDITION,
    MULTIPLY,
    SUBTRACT,
    DIVISION,
    NUMBER,
    OP_PARENTHISIS,
    CL_PARENTHISIS
};
#endif // NODE_H

#ifndef TOKENS_H
#define TOKENS_H
struct Tokens
{
    string buffer;
    type id;
    map<type, string> dictionary;
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