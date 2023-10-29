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
    RANGE,
    ARRAY,
    OP_BRACKET,
    CL_BRACKET
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
bool isNumber(const std::string &str)
{
    std::istringstream ss(str);
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
    op["<="] = type::LTE;
    op[">="] = type::GTE;
    op[">"] = type::GT;
    op["<"] = type::LT;
    op[".."] = type::ELLIPSIS;
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
    op["string"] = type::STRING;

    op["while"] = type::LOOP;
    op["for"] = type::FOR_LOOP;
    op["step"] = type::STEP;
    op["range"] = type::RANGE;

    op["Array"] = type::ARRAY;
    op["["] = type::OP_BRACKET;
    op["]"] = type::CL_BRACKET;

    op[":"] = type::SEMI_COLON;
    op[","] = type::COMMA;

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
    if (str == "=")
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
        if (buffer != "")
        {
            groupings(token_list, buffer);
        }
        state = 2;
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

    for (int i = 0; i < a.size(); i++)
    {
        cout << dictionary[a[i].id] + "(" + a[i].buffer + ")" << endl;
    }
}

// vector<Tokens> lex(vector<string> lines)
// {

// #pragma region Dict stuff
//     map<type, string> dictionary;
//     dictionary[type::NUMBER] = "NUMBER";
//     dictionary[type::ADDITION] = "ADDITION";
//     dictionary[type::SUBTRACT] = "SUBTRACT";
//     dictionary[type::DIVISION] = "DIVISION";
//     dictionary[type::MULTIPLY] = "MULTIPLY";
//     dictionary[type::OP_PARENTHISIS] = "OP_PARENTHISIS";
//     dictionary[type::CL_PARENTHISIS] = "CL_PARENTHISIS";
//     dictionary[type::CL_PARENTHISIS] = "CL_PARENTHISIS";
//     dictionary[type::MOD] = "MOD";
//     dictionary[type::WORD] = "WORD";
//     dictionary[type::EQUALS] = "EQUALS";
//     dictionary[type::COMMA] = "COMMA";
//     dictionary[type::BEGIN] = "BEGIN";
//     dictionary[type::END] = "END";
//     dictionary[type::FUNCTION] = "FUNCTION";
//     dictionary[type::VAR] = "VAR";
//     dictionary[type::END_OF_LINE] = "END_OF_LINE";
//     dictionary[type::PRINT] = "PRINT";
//     dictionary[type::SCAN] = "SCAN";
//     dictionary[type::EXIT] = "EXIT";
//     dictionary[type::NUMBER] = "NUMBER";
//     dictionary[type::INT] = "INT";
//     dictionary[type::STRING] = "STRING";
//     dictionary[type::FLOAT] = "FLOAT";
//     dictionary[type::MACRO] = "MACRO";
//     dictionary[type::RETURN] = "RETURN";
//     dictionary[type::STRING_LITERAL] = "STRING_LITERAL";
//     dictionary[type::BOOL_EQ] = "BOOL_EQ";
//     dictionary[type::IF] = "IF";
//     dictionary[type::TRUE] = "TRUE";
//     dictionary[type::FALSE] = "FALSE";
//     dictionary[type::GT] = "GT";
//     dictionary[type::LT] = "LT";
//     dictionary[type::GTE] = "GTE";
//     dictionary[type::LTE] = "LTE";
//     dictionary[type::BOOL] = "BOOL";
//     dictionary[type::CONSTANT] = "CONST";
//     dictionary[type::RETURNS] = "RETURNS";

//     dictionary[type::SEMI_COLON] = "SEMI_COLON";
//     dictionary[type::ELSE] = "ELSE";

//     map<string, type> typeOfOP;
//     typeOfOP["+"] = type::ADDITION;
//     typeOfOP["-"] = type::SUBTRACT;
//     typeOfOP["/"] = type::DIVISION;
//     typeOfOP["*"] = type::MULTIPLY;
//     typeOfOP["("] = type::OP_PARENTHISIS;
//     typeOfOP[")"] = type::CL_PARENTHISIS;
//     typeOfOP["%"] = type::MOD;
//     // typeOfOP["function"] = type::FUNCTION;
//     typeOfOP["fn"] = type::FUNCTION;

//     typeOfOP["if"] = type::IF;
//     typeOfOP["and"] = type::AND;
//     typeOfOP["or"] = type::OR;
//     typeOfOP["while"] = type::LOOP;
//     typeOfOP["for"] = type::FOR_LOOP;
//     typeOfOP["else"] = type::ELSE;

//     typeOfOP["var"] = type::VAR;
//     typeOfOP["let"] = type::VAR;
//     typeOfOP["print"] = type::PRINT;
//     typeOfOP["scan"] = type::SCAN;
//     typeOfOP["exit"] = type::EXIT;
//     typeOfOP["int"] = type::INT;
//     typeOfOP["float"] = type::FLOAT;
//     typeOfOP["string"] = type::STRING;
//     typeOfOP["char"] = type::CHAR;

//     typeOfOP["#define"] = type::MACRO;
//     typeOfOP["return"] = type::RETURN;
//     typeOfOP["equals"] = type::BOOL_EQ;
//     typeOfOP["=="] = type::BOOL_EQ;
//     typeOfOP["<="] = type::LTE;
//     typeOfOP[">="] = type::GTE;
//     typeOfOP[">"] = type::GT;
//     typeOfOP["<"] = type::LT;

//     typeOfOP["="] = type::EQUALS;
//     typeOfOP["true"] = type::TRUE;
//     typeOfOP["false"] = type::FALSE;
//     typeOfOP["bool"] = type::BOOL;
//     typeOfOP["const"] = type::CONSTANT;
//     typeOfOP["returns"] = type::RETURNS;
//     typeOfOP[":"] = type::SEMI_COLON;

// #pragma endregion
//     int wordstate = 1;
//     string wordBuffer = "";
//     int state = 1;
//     regex opRegex("[+*/%]"); // operator regex

//     regex numReg("[0-9.]"); // num regex
//     std::smatch myMatch;
//     int stateIsNum = 1;
//     Tokens token;

//     vector<Tokens> a;

//     for (int i = 0; i < lines.size(); i++)
//     {
//         string line = lines[i];
//         string buffer = "";
//         int isComment = 0;
//         int isString = 0;
//         int isChar = 0;
//         string wordBuffer = "";
//         string stringBuffer = "";
//         for (int i2 = 0; i2 < line.length(); i2++)
//         {
//             char current = line.at(i2);
//             if (current == '/' && line.at(i2 + 1) == '/')
//             {
//                 isComment = 1;
//                 continue;
//             }
//             if (isComment == 1)
//             {
//                 continue;
//             }
//             if (current == '\"' || current == '\'')
//             {
//                 if (isString == 0)
//                 {
//                     isString = 1;
//                     continue;
//                 }
//                 else
//                 {
//                     isString = 0;
//                     if (current == '\'')
//                     {
//                         modifyStruct(token, type::CHAR_LITERAL, dictionary, stringBuffer);
//                     }
//                     else
//                     {
//                         modifyStruct(token, type::STRING_LITERAL, dictionary, stringBuffer);
//                     }
//                     stringBuffer = "";
//                     a.push_back(token);
//                     continue;
//                 }
//             }
//             if (isString == 1)
//             {
//                 stringBuffer += current;
//                 continue;
//             }

//             if (current != ' ' && current != '\t' && current != '\0')
//             {

//                 string str(1, current);
//                 // cout << "word buffer: " + wordBuffer << endl;
//                 // cout << "state is num: " + stateIsNum << endl;
//                 // cout << "buffer: " + buffer << endl;
//                 // cout << "state: " + state << endl;
//                 if (str == ")" || str == "(")
//                 {
//                     stateIsNum = 1;
//                 }
//                 else if (regex_search(str, myMatch, numReg) || regex_search(str, myMatch, opRegex) || str == ")" || str == "(" || str == "-")
//                 {
//                 }
//                 else
//                 {
//                     stateIsNum = 0;
//                 }

//                 if (stateIsNum == 1)
//                 {

//                     // cout << "string: " + str + "\n"
//                     //      << endl;
//                     // cout << "buffer: " + buffe r + "\n";
//                     // cout << "state: " + to_string(state) << endl;
//                     if (wordBuffer != "")
//                     {

//                         if (wordBuffer == "=")
//                         {
//                             modifyStruct(token, type::EQUALS, dictionary, wordBuffer);
//                             a.push_back(token);
//                             wordBuffer = "";
//                         }
//                         else
//                         {
//                             modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);

//                             a.push_back(token);
//                             wordBuffer = "";
//                         }
//                     }
//                     wordstate = 1;

// #pragma region num

//                     // 1-9
//                     if (state == 1)
//                     {

//                         // cout << current;

//                         if (regex_search(str, myMatch, numReg))
//                         {
//                             buffer += str;
//                         }
//                         else if (regex_search(str, myMatch, opRegex))
//                         {
//                             if (buffer != "")
//                             {

//                                 // Tokens token(buffer, type::NUMBER);
//                                 // token.dictionary = dictionary;
//                                 modifyStruct(token, type::NUMBER, dictionary, buffer);
//                                 a.push_back(token);
//                             }
//                             buffer = "";
//                             buffer += str;
//                             modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                             a.push_back(token);
//                             buffer = "";
//                             state = 2;
//                         }
//                         else if (str == "-")
//                         {

//                             if (buffer == "")
//                             {
//                                 // cout << "sub \n";

//                                 buffer += str;
//                             }
//                             else
//                             {

//                                 if (buffer != "")
//                                 {
//                                     modifyStruct(token, type::NUMBER, dictionary, buffer);
//                                     a.push_back(token);
//                                 }
//                                 buffer = "";
//                                 buffer += str;
//                                 // cout <<  << endl;

//                                 modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                                 a.push_back(token);
//                                 buffer = "";
//                             }
//                             state = 2;
//                         }
//                         else if (str == ")" || str == "(")
//                         {
//                             if (buffer != "")
//                             {
//                                 if (typeOfOP.find(buffer) != typeOfOP.end())
//                                 {
//                                     // Tokens token(buffer, typeOfOP[buffer]);
//                                     // token.dictionary = dictionary;
//                                     modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                                     a.push_back(token);
//                                 }
//                                 else
//                                 {
//                                     // Tokens token(buffer, type::NUMBER);
//                                     // token.dictionary = dictionary;
//                                     modifyStruct(token, type::NUMBER, dictionary, buffer);
//                                     a.push_back(token);
//                                 }
//                                 buffer = "";
//                             }
//                             buffer += str;
//                             modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                             a.push_back(token);
//                             buffer = "";

//                             state = 2;
//                         }
//                     }
//                     //+,*,/,%
//                     else if (state == 2)
//                     {
//                         if (buffer != "")
//                         {
//                             if (typeOfOP.find(buffer) != typeOfOP.end())
//                             {
//                                 // Tokens token(buffer, typeOfOP[buffer]);
//                                 // token.dictionary = dictionary;
//                                 modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                                 a.push_back(token);
//                             }
//                             else
//                             {
//                                 // Tokens token(buffer, type::NUMBER);
//                                 // token.dictionary = dictionary;
//                                 modifyStruct(token, type::NUMBER, dictionary, buffer);
//                                 a.push_back(token);
//                             }
//                             buffer = "";
//                         }
//                         if (buffer == "-" && str == ">")
//                         {
//                             buffer += str;
//                             if (buffer == "->")
//                             {
//                                 modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                                 buffer = "";
//                             }
//                             state = 1;
//                         }
//                         else if (str == "-")
//                         {
//                             // cout << "here" << endl;
//                             buffer += str;
//                             state = 1;
//                         }
//                         else if (str == ")" || str == "(")
//                         {
//                             if (buffer != "")
//                             {
//                                 modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                                 a.push_back(token);
//                                 buffer = "";
//                             }

//                             buffer += str;

//                             modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                             a.push_back(token);
//                             buffer = "";

//                             state = 1;
//                         }
//                         else if (regex_search(str, myMatch, numReg))
//                         {

//                             buffer += str;
//                             // cout << buffer << endl;

//                             state = 1;
//                         }
//                         else if (str == "+" || str == "*" || str == "/")
//                         {
//                             buffer += str;
//                             modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                             a.push_back(token);
//                             buffer = "";
//                             state = 2;
//                         }
//                     }
//                     else if (state == 3)
//                     {
//                     }
// #pragma endregion num
//                 }
//                 else if (stateIsNum == 0)
//                 {
//                     // cout << "string: " + str << endl;
//                     // cout << "wordBuffer: " + wordBuffer + "\n";
// #pragma region word

//                     if (buffer != "")
//                     {
//                         if (typeOfOP.find(buffer) != typeOfOP.end())
//                         {
//                             // Tokens token(buffer, typeOfOP[buffer]);
//                             // token.dictionary = dictionary;
//                             modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                             a.push_back(token);
//                         }
//                         else
//                         {
//                             // Tokens token(buffer, type::NUMBER);
//                             // token.dictionary = dictionary;
//                             modifyStruct(token, type::NUMBER, dictionary, buffer);
//                             a.push_back(token);
//                         }
//                         buffer = "";
//                     }
//                     if (wordstate == 1)
//                     {
//                         if (str == "=" || str == ">" || str == "<")
//                         {
//                             wordstate = 2;
//                             if (wordBuffer != "")
//                             {
//                                 modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);
//                                 a.push_back(token);
//                                 wordBuffer = "";
//                             }

//                             wordBuffer += str;
//                             stateIsNum = 1;
//                         }
//                         else if (str == "," || str == "}" || str == "{" || str == ";" || regex_search(str, myMatch, opRegex) || str == "-")
//                         {
//                             if (wordBuffer != "")
//                             {

//                                 modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);
//                                 a.push_back(token);
//                                 wordBuffer = "";
//                             }

//                             wordBuffer += str;
//                             modifyStruct(token, (wordBuffer == ",") ? type::COMMA : (wordBuffer == "{")                         ? type::BEGIN
//                                                                                 : (wordBuffer == "}")                           ? type::END
//                                                                                 : (wordBuffer == ";")                           ? type::END_OF_LINE
//                                                                                 : (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer]
//                                                                                                                                 : type::WORD,
//                                          dictionary, wordBuffer);
//                             a.push_back(token);
//                             // cout << "equals" << endl;
//                             wordBuffer = "";
//                             stateIsNum = 1;
//                         }
//                         else
//                         {
//                             wordBuffer += str;
//                         }
//                     }
//                     else if (wordstate == 2)
//                     {
//                         if (str == "=")
//                         {
//                             if (wordBuffer == "<")
//                             {
//                                 wordBuffer += str;
//                                 // cout << wordBuffer << endl;
//                                 modifyStruct(token, type::LTE, dictionary, wordBuffer);
//                                 a.push_back(token);
//                                 wordBuffer = "";
//                                 stateIsNum = 1;
//                                 wordstate = 1;
//                                 state = 1;
//                             }
//                             else if (wordBuffer == ">")
//                             {
//                                 wordBuffer += str;
//                                 cout << wordBuffer << endl;
//                                 modifyStruct(token, type::GTE, dictionary, wordBuffer);
//                                 a.push_back(token);
//                                 wordBuffer = "";
//                                 stateIsNum = 1;
//                                 wordstate = 1;
//                                 state = 1;
//                             }
//                             else
//                             {
//                                 wordBuffer += str;
//                                 cout << wordBuffer << endl;
//                                 modifyStruct(token, type::BOOL_EQ, dictionary, wordBuffer);
//                                 a.push_back(token);
//                                 wordBuffer = "";
//                                 stateIsNum = 1;
//                                 wordstate = 1;
//                                 state = 1;
//                             }
//                         }
//                         else
//                         {
//                             // cout << "word: " << wordBuffer << endl;

//                             if (wordBuffer != "")
//                             {
//                                 // cout << "word: " << wordBuffer << endl;

//                                 modifyStruct(token, type::EQUALS, dictionary, wordBuffer);
//                                 wordBuffer = "";
//                                 a.push_back(token);
//                             }

//                             wordBuffer += str;
//                             // cout << "buffer: " << wordBuffer << endl;
//                             // modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);
//                             // wordBuffer = "";
//                             // a.push_back(token);
//                             wordstate = 1;
//                             // stateIsNum = 1;
//                             state = 1;
//                         }

//                         // cout << "word state != 1 \n";
//                     }
//                 }
// #pragma endregion
//             }
//             else
//             {
//                 if (buffer != "")
//                 {
//                     if (typeOfOP.find(buffer) != typeOfOP.end())
//                     {
//                         // Tokens token(buffer, typeOfOP[buffer]);
//                         // token.dictionary = dictionary;
//                         modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                         a.push_back(token);
//                     }
//                     else
//                     {
//                         // Tokens token(buffer, type::NUMBER);
//                         // token.dictionary = dictionary;
//                         modifyStruct(token, type::NUMBER, dictionary, buffer);
//                         a.push_back(token);
//                     }
//                     buffer = "";
//                 }
//                 if (wordBuffer != "")
//                 {
//                     modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);
//                     a.push_back(token);
//                     wordBuffer = "";
//                 }
//                 stateIsNum = 1;
//             }
//         }
//         if (buffer != "")
//         {
//             if (typeOfOP.find(buffer) != typeOfOP.end())
//             {
//                 // Tokens token(buffer, typeOfOP[buffer]);
//                 // token.dictionary = dictionary;
//                 modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
//                 a.push_back(token);
//             }
//             else
//             {
//                 // Tokens token(buffer, type::NUMBER);
//                 // token.dictionary = dictionary;
//                 modifyStruct(token, type::NUMBER, dictionary, buffer);
//                 a.push_back(token);
//             }
//         }
//     }
//     return a;
// }
