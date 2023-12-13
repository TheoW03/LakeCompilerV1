#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <regex>
#include <map>
#include <sstream>

using namespace std;

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
struct Tokens
{

    string buffer;
    type id;
    map<type, string> dictionary;
    virtual string to_string();
};
string Tokens::to_string()
{
    return "C++ is great lang meowing";
}
// string Tokens::to_string()
// {
//     return dictionary[id] + "(" + buffer + ")";
// }
bool isNumber(const string &str)
{
    istringstream ss(str);
    double num;
    ss >> num;
    // Check if the entire string was successfully converted to a number
    return !ss.fail() && ss.eof();
}
void throwException(string message)
{
    cerr << message;
    exit(1);
}
vector<string> readFile(string file)
{
    vector<string> vec;
    ifstream infile(file); // Open the file for reading
    string line;
    // cout << "input: " << endl;
    while (getline(infile, line))
    {
        // cout << line + "\n";
        vec.push_back(line);
    }
    infile.close();
    return vec;
}
void modifyStruct(Tokens &token, type enumType, string buffer)
{
    token.buffer = buffer;
    token.id = enumType;
}
void groupings(vector<Tokens> &token_list, string &buffer)
{
    map<string, type> op;
    op["+"] = type::ADDITION;
    op["-"] = type::SUBTRACT;
    op["*"] = type::MULTIPLY;
    op["%"] = type::MOD;
    op["/"] = type::DIVISION;
    op["="] = type::EQUALS;
    op["("] = type::OP_PARENTHISIS;
    op[")"] = type::CL_PARENTHISIS;
    op["=="] = type::BOOL_EQ;
    op["/="] = type::NOT_EQ;

    op["<="] = type::LTE;
    op[">="] = type::GTE;
    op[">"] = type::GT;
    op["<"] = type::LT;
    op[".."] = type::ELLIPSIS;

    op["then"] = type::THEN;
    op["do"] = type::DO;
    op["if"] = type::IF;
    op["else"] = type::ELSE;

    op["{"] = type::BEGIN;
    op["}"] = type::END;

    op["and"] = type::AND;
    op["or"] = type::OR;

    op["fn"] = type::FUNCTION;
    op["returns"] = type::RETURNS;
    op["return"] = type::RETURN;

    op["int"] = type::INT;
    op["float"] = type::FLOAT;
    op["char"] = type::CHAR;
    op["bool"] = type::BOOL;
    op["true"] = type::TRUE;
    op["false"] = type::FALSE;

    op["string"] = type::STRING;

    op["while"] = type::LOOP;
    op["for"] = type::FOR_LOOP;
    op["step"] = type::STEP;
    op["from"] = type::FROM;

    op["Array"] = type::ARRAY;
    op["["] = type::OP_BRACKET;
    op["]"] = type::CL_BRACKET;

    op[":"] = type::SEMI_COLON;
    op[","] = type::COMMA;

    op["print"] = type::PRINT;
    op["exit"] = type::PRINT;
    op[";"] = type::END_OF_LINE;
    op[">>"] = type::SLL;
    op["<<"] = type::SRR;
    op["|"] = type::B_OR;
    op["&"] = type::B_AND;

    if (op.find(buffer) != op.end())
    {
        Tokens token;
        modifyStruct(token, op[buffer], buffer);
        token_list.push_back(token);
        buffer = "";
    }
    else if (isNumber(buffer))
    {
        Tokens token;
        modifyStruct(token, type::NUMBER, buffer);
        token_list.push_back(token);
        buffer = "";
    }
    else
    {
        Tokens token;
        modifyStruct(token, type::WORD, buffer);
        token_list.push_back(token);
        buffer = "";
    }
}
void number(int &state, string str, vector<Tokens> &token_list, string &buffer);

void equals_lex(int &state, string str, vector<Tokens> &token_list, string &buffer)
{
    if (str == "=" || str == ">" || str == "<")
    {
        buffer += str;
        if (buffer != "")
        {
            groupings(token_list, buffer);
        }
        state = 1;
    }
    else
    {
        if (buffer != "")
        {
            groupings(token_list, buffer);
        }
        number(state, str, token_list, buffer);
        state = 1;
    }
}
void operation(int &state, string str, vector<Tokens> &token_list, string &buffer)
{

    if (buffer != "")
    {

        groupings(token_list, buffer);
    }
    buffer += str;

    if (str == "(" || str == ")")
    {
        groupings(token_list, buffer);
    }
    state = 1;
}

void number(int &state, string str, vector<Tokens> &token_list, string &buffer)
{

    if (str == "-")
    {
        if (buffer == "")
        {
            buffer += str;
            return;
        }
    }
    map<string, type> seperator;
    seperator["{"] = type::ADDITION;
    seperator["}"] = type::SUBTRACT;
    seperator[":"] = type::MULTIPLY;
    seperator[";"] = type::MOD;
    seperator["["] = type::DIVISION;
    seperator["]"] = type::OP_PARENTHISIS;
    seperator[")"] = type::CL_PARENTHISIS;
    seperator["("] = type::FUNCTION;
    seperator[","] = type::FUNCTION;

    if (seperator.find(str) != seperator.end())
    {
        if (buffer != "")
        {

            groupings(token_list, buffer);
        }
        buffer += str;
        groupings(token_list, buffer);
        return;
    }
    if (str == "=" || str == ">" || str == "<")
    {
        if (buffer != "")
        {
            groupings(token_list, buffer);
        }
        state = 3;
        buffer += str;
        return;
    }
    if (str == "+" || str == "*" || str == "/" || str == "-")
    {
        if (str == "/")
        {
            state = 3;
        }
        else
        {
            state = 2;
        }
        if (buffer != "")
        {
            groupings(token_list, buffer);
        }
    }
    buffer += str;
}
vector<Tokens> lex(vector<string> lines)
{
    Tokens token;
    map<string, type> op;
    int isString = 0;
    vector<Tokens> token_list;
    int state = 1;
    string buffer = "";
    int isComment = 0;
    for (int line_loc = 0; line_loc < lines.size(); line_loc++)
    {
        string line = lines[line_loc];
        for (int next_char = 0; next_char < line.length(); next_char++)
        {
            if (isComment == 1)
            {
                continue;
            }
            string str(1, line.at(next_char));

            if (str == "\"")
            {
                if (isString == 0)
                {
                    if (buffer != "")
                    {

                        groupings(token_list, buffer);
                    }
                    isString = 1;
                }
                else
                {
                    Tokens token;
                    token.buffer = buffer;
                    token.id = type::STRING_LITERAL;

                    token_list.push_back(token);
                    buffer = "";
                    isString = 0;
                }
                continue;
            }
            if (str == "\'")
            {
                if (isString == 0)
                {
                    if (buffer != "")
                    {

                        groupings(token_list, buffer);
                    }
                    isString = 1;
                }
                else
                {
                    Tokens token;
                    token.buffer = buffer;
                    cout << buffer << endl;

                    token.id = type::CHAR_LITERAL;
                    token_list.push_back(token);
                    buffer = "";
                    isString = 0;
                }
                continue;
            }
            if (isString == 1)
            {
                buffer += str;
                continue;
            }

            if ((int)line.at(next_char) == 32 || (int)line.at(next_char) == 13 || (int)line.at(next_char) == 9)
            {
                if (buffer != "")
                {

                    groupings(token_list, buffer);
                }
                continue;
            }

            if (next_char < (line.length() - 1))
            {
                if (str == "/" && line.at(next_char + 1) == '/')
                {
                    isComment = 1;
                    continue;
                }
                if (str == "." && line.at(next_char + 1) == '.')
                {
                    if (buffer != "")
                    {
                        groupings(token_list, buffer);
                    }
                    buffer += str;
                    state = 4;
                    continue;
                }
            }

            if (state == 1)
            {
                number(state, str, token_list, buffer);
            }
            else if (state == 2)
            {
                operation(state, str, token_list, buffer);
            }
            else if (state == 3)
            {
                equals_lex(state, str, token_list, buffer);
            }
            else if (state == 4)
            {
                buffer += str;
                groupings(token_list, buffer);
                state = 1;
            }
        }
        isComment = 0;
    }
    if (buffer != "")
    {
        groupings(token_list, buffer);
    }
    return token_list;
}
void printList(vector<Tokens> a)
{
    map<type, string> dictionary;

    dictionary[type::NUMBER] = "NUMBER";
    dictionary[type::ADDITION] = "ADDITION";
    dictionary[type::SUBTRACT] = "SUBTRACT";
    dictionary[type::DIVISION] = "DIVISION";
    dictionary[type::MULTIPLY] = "MULTIPLY";
    dictionary[type::OP_PARENTHISIS] = "OP_PARENTHISIS";
    dictionary[type::CL_PARENTHISIS] = "CL_PARENTHISIS";
    dictionary[type::CL_PARENTHISIS] = "CL_PARENTHISIS";
    dictionary[type::MOD] = "MOD";
    dictionary[type::WORD] = "WORD";
    dictionary[type::EQUALS] = "EQUALS";
    dictionary[type::COMMA] = "COMMA";
    dictionary[type::BEGIN] = "BEGIN";
    dictionary[type::END] = "END";
    dictionary[type::FUNCTION] = "FUNCTION";
    dictionary[type::VAR] = "VAR";
    dictionary[type::END_OF_LINE] = "END_OF_LINE";
    dictionary[type::PRINT] = "PRINT";
    dictionary[type::SCAN] = "SCAN";
    dictionary[type::EXIT] = "EXIT";
    dictionary[type::NUMBER] = "NUMBER";
    dictionary[type::INT] = "INT";
    dictionary[type::STRING] = "STRING";
    dictionary[type::FLOAT] = "FLOAT";
    dictionary[type::MACRO] = "MACRO";
    dictionary[type::RETURN] = "RETURN";
    dictionary[type::STRING_LITERAL] = "STRING_LITERAL";
    dictionary[type::BOOL_EQ] = "BOOL_EQ";
    dictionary[type::IF] = "IF";
    dictionary[type::TRUE] = "TRUE";
    dictionary[type::FALSE] = "FALSE";
    dictionary[type::GT] = "GT";
    dictionary[type::LT] = "LT";
    dictionary[type::GTE] = "GTE";
    dictionary[type::LTE] = "LTE";
    dictionary[type::BOOL] = "BOOL";
    dictionary[type::CONSTANT] = "CONST";
    dictionary[type::RETURNS] = "RETURNS";
    dictionary[type::ELLIPSIS] = "ELLIPSIS";
    dictionary[type::STEP] = "STEP";
    dictionary[type::OP_BRACKET] = "OP_BRACKET";
    dictionary[type::CL_BRACKET] = "CL_BRACKET";
    dictionary[type::ARRAY] = "ARRAY";
    dictionary[type::SEMI_COLON] = "SEMICOLON";
    dictionary[type::CHAR] = "CHAR";
    dictionary[type::CHAR_LITERAL] = "CHAR_LITERAL";
    dictionary[type::SRR] = "SRR";
    dictionary[type::SLL] = "SLL";
    dictionary[type::B_AND] = "B_AND";
    dictionary[type::B_OR] = "B_OR";

    for (int i = 0; i < a.size(); i++)
    {
        cout << dictionary[a[i].id] + "(" + a[i].buffer + ")" << endl;
    }
}
