#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <regex>
#include <map>

using namespace std;

enum type
{
    ADDITION,
    MULTIPLY,
    SUBTRACT,
    DIVISION,
    NUMBER
};
#ifndef TOKENS_H
#define TOKENS_H
struct Tokens
{
    string buffer;
    type id;
    map<type,string> dictionary;
};
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
#endif
void
throwException(string message);
vector<string> readFile(string file);
vector<Tokens> lex(vector<string> lines);
void printList(vector<Tokens> a);
void modifyStruct(Tokens &token, type enumType1, map<type, string> dictionary1, string buffer1);