#include <iostream>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <string>
#include <regex>
#include <map>
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
    CHAR,
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
    BOOL
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
    return dictionary[id] + "(" + buffer + ")";
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
void modifyStruct(Tokens &token, type enumType1, map<type, string> dictionary1, string buffer1)
{
    token.buffer = buffer1;
    token.id = enumType1;
    token.dictionary = dictionary1;
}

vector<Tokens> lex(vector<string> lines)
{

#pragma region Dict stuff
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
    

    map<string, type> typeOfOP;
    typeOfOP["+"] = type::ADDITION;
    typeOfOP["-"] = type::SUBTRACT;
    typeOfOP["/"] = type::DIVISION;
    typeOfOP["*"] = type::MULTIPLY;
    typeOfOP["("] = type::OP_PARENTHISIS;
    typeOfOP[")"] = type::CL_PARENTHISIS;
    typeOfOP["%"] = type::MOD;
    typeOfOP["function"] = type::FUNCTION;
    typeOfOP["if"] = type::IF;
    typeOfOP["and"] = type::AND;
    typeOfOP["or"] = type::OR;
    typeOfOP["while"] = type::LOOP;
    typeOfOP["for"] = type::LOOP;
    typeOfOP["var"] = type::VAR;
    typeOfOP["let"] = type::VAR;
    typeOfOP["print"] = type::PRINT;
    typeOfOP["scan"] = type::SCAN;
    typeOfOP["exit"] = type::EXIT;
    typeOfOP["int"] = type::INT;
    typeOfOP["float"] = type::FLOAT;
    typeOfOP["string"] = type::STRING;
    typeOfOP["#define"] = type::MACRO;
    typeOfOP["return"] = type::RETURN;
    typeOfOP["equals"] = type::BOOL_EQ;
    typeOfOP["=="] = type::BOOL_EQ;
    typeOfOP["<="] = type::LTE;
    typeOfOP[">="] = type::GTE;
    typeOfOP[">"] = type::GT;
    typeOfOP["<"] = type::LT;

    typeOfOP["="] = type::EQUALS;
    typeOfOP["true"] = type::TRUE;
    typeOfOP["false"] = type::FALSE;
    typeOfOP["bool"] = type::BOOL;

#pragma endregion
    int wordstate = 1;
    string wordBuffer = "";
    int state = 1;
    regex opRegex("[+*/%]"); // operator regex

    regex numReg("[0-9.]"); // num regex
    std::smatch myMatch;
    int stateIsNum = 1;
    Tokens token;

    vector<Tokens> a;

    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        string buffer = "";
        int isComment = 0;
        int isString = 0;
        string wordBuffer = "";
        string stringBuffer = "";
        for (int i2 = 0; i2 < line.length(); i2++)
        {
            char current = line.at(i2);
            if (current == '/' && line.at(i2 + 1) == '/')
            {
                isComment = 1;
                continue;
            }
            if (isComment == 1)
            {
                continue;
            }
            if (current == '\"' || current == '\'')
            {
                if (isString == 0)
                {
                    isString = 1;
                    continue;
                }
                else
                {
                    isString = 0;
                    modifyStruct(token, type::STRING_LITERAL, dictionary, stringBuffer);
                    stringBuffer = "";
                    a.push_back(token);
                    continue;
                }
            }
            if (isString == 1)
            {
                stringBuffer += current;
                continue;
            }

            if (current != ' ' && current != '\t' && current != '\0')
            {

                string str(1, current);
                // cout << "word buffer: " + wordBuffer << endl;
                // cout << "state is num: " + stateIsNum << endl;
                // cout << "buffer: " + buffer << endl;
                // cout << "state: " + state << endl;
                if (str == ")" || str == "(")
                {
                    stateIsNum = 1;
                }
                if (regex_search(str, myMatch, numReg) || regex_search(str, myMatch, opRegex) || str == ")" || str == "(" || str == "-")
                {
                }
                else
                {
                    stateIsNum = 0;
                }

                if (stateIsNum == 1)
                {

                    cout << str << endl;

                    if (wordBuffer != "")
                    {

                        if (wordBuffer == "=")
                        {
                            modifyStruct(token, type::EQUALS, dictionary, wordBuffer);
                            a.push_back(token);
                            wordBuffer = "";
                        }
                        else
                        {
                            modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);

                            a.push_back(token);
                            wordBuffer = "";
                        }
                    }
                    wordstate = 1;

#pragma region num

                    // 1-9
                    if (state == 1)
                    {

                        // cout << current;
                        // cout << "string: " + str + "\n";
                        // cout << "buffer: " + buffer + "\n";
                        if (regex_search(str, myMatch, numReg))
                        {
                            buffer += str;
                        }
                        else if (regex_search(str, myMatch, opRegex))
                        {
                            if (buffer != "")
                            {

                                // Tokens token(buffer, type::NUMBER);
                                // token.dictionary = dictionary;
                                modifyStruct(token, type::NUMBER, dictionary, buffer);
                                a.push_back(token);
                            }
                            buffer = "";
                            buffer += str;
                            modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                            a.push_back(token);
                            buffer = "";
                            state = 2;
                        }
                        else if (str == "-")
                        {

                            if (buffer == "")
                            {
                                // cout << "sub \n";

                                buffer += str;
                            }
                            else
                            {

                                if (buffer != "")
                                {
                                    modifyStruct(token, type::NUMBER, dictionary, buffer);
                                    a.push_back(token);
                                }
                                buffer = "";
                                buffer += str;
                                // cout <<  << endl;

                                modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                                a.push_back(token);
                                buffer = "";
                            }
                            state = 2;
                        }
                        else if (str == ")" || str == "(")
                        {
                            if (buffer != "")
                            {
                                if (typeOfOP.find(buffer) != typeOfOP.end())
                                {
                                    // Tokens token(buffer, typeOfOP[buffer]);
                                    // token.dictionary = dictionary;
                                    modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                                    a.push_back(token);
                                }
                                else
                                {
                                    // Tokens token(buffer, type::NUMBER);
                                    // token.dictionary = dictionary;
                                    modifyStruct(token, type::NUMBER, dictionary, buffer);
                                    a.push_back(token);
                                }
                                buffer = "";
                            }
                            buffer += str;
                            modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                            a.push_back(token);
                            buffer = "";

                            state = 2;
                        }
                    }
                    //+,*,/,%
                    else if (state == 2)
                    {
                        // if (buffer != "")
                        // {
                        //     if (typeOfOP.find(buffer) != typeOfOP.end())
                        //     {
                        //         // Tokens token(buffer, typeOfOP[buffer]);
                        //         // token.dictionary = dictionary;
                        //         modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                        //         a.push_back(token);
                        //     }
                        //     else
                        //     {
                        //         // Tokens token(buffer, type::NUMBER);
                        //         // token.dictionary = dictionary;
                        //         modifyStruct(token, type::NUMBER, dictionary, buffer);
                        //         a.push_back(token);
                        //     }
                        //     buffer = "";
                        // }
                        if (str == "-")
                        {
                            buffer += str;
                            state = 1;
                        }
                        if (str == ")" || str == "(")
                        {
                            if (buffer != "")
                            {
                                modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                                a.push_back(token);
                                buffer = "";
                            }

                            buffer += str;

                            modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                            a.push_back(token);
                            buffer = "";

                            state = 1;
                        }
                        if (regex_search(str, myMatch, numReg))
                        {
                            buffer += str;
                            // cout << buffer << endl;

                            state = 1;
                        }
                    }
                    else if (state == 3)
                    {
                    }
#pragma endregion num
                }
                else if (stateIsNum == 0)
                {
#pragma region word

                    if (buffer != "")
                    {
                        if (typeOfOP.find(buffer) != typeOfOP.end())
                        {
                            // Tokens token(buffer, typeOfOP[buffer]);
                            // token.dictionary = dictionary;
                            modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                            a.push_back(token);
                        }
                        else
                        {
                            // Tokens token(buffer, type::NUMBER);
                            // token.dictionary = dictionary;
                            modifyStruct(token, type::NUMBER, dictionary, buffer);
                            a.push_back(token);
                        }
                        buffer = "";
                    }
                    if (wordstate == 1)
                    {
                        cout << "state == 1" << endl;
                        if (str == "=" || str == ">" || str == "<")
                        {
                            wordstate = 2;
                            if (wordBuffer != "")
                            {
                                modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);
                                a.push_back(token);
                                wordBuffer = "";
                            }

                            wordBuffer += str;
                            stateIsNum = 1;
                        }
                        else if (str == "," || str == "}" || str == "{" || str == ";" || regex_search(str, myMatch, opRegex) || str == "-")
                        {
                            if (wordBuffer != "")
                            {

                                modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);
                                a.push_back(token);
                                wordBuffer = "";
                            }
                            wordBuffer += str;
                            modifyStruct(token, (wordBuffer == ",") ? type::COMMA : (wordBuffer == "{")                         ? type::BEGIN
                                                                                : (wordBuffer == "}")                           ? type::END
                                                                                : (wordBuffer == ";")                           ? type::END_OF_LINE
                                                                                : (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer]
                                                                                                                                : type::WORD,
                                         dictionary, wordBuffer);
                            a.push_back(token);
                            // cout << "equals" << endl;
                            wordBuffer = "";
                            stateIsNum = 1;
                        }
                        else
                        {
                            wordBuffer += str;
                        }
                    }
                    else if (wordstate == 2)
                    {
                        if (str == "=")
                        {
                            if (wordBuffer == "<")
                            {
                                wordBuffer += str;
                                cout << wordBuffer << endl;
                                modifyStruct(token, type::LTE, dictionary, wordBuffer);
                                a.push_back(token);
                                wordBuffer = "";
                                stateIsNum = 1;
                                wordstate = 1;
                            }
                            else if (wordBuffer == ">")
                            {
                                wordBuffer += str;
                                cout << wordBuffer << endl;
                                modifyStruct(token, type::GTE, dictionary, wordBuffer);
                                a.push_back(token);
                                wordBuffer = "";
                                stateIsNum = 1;
                                wordstate = 1;
                            }
                            else
                            {
                                wordBuffer += str;
                                cout << wordBuffer << endl;
                                modifyStruct(token, type::BOOL_EQ, dictionary, wordBuffer);
                                a.push_back(token);
                                wordBuffer = "";
                                stateIsNum = 1;
                                wordstate = 1;
                            }
                        }
                        else
                        {
                            cout << "word: " << wordBuffer << endl;

                            if (wordBuffer != "")
                            {
                                cout << "word: " << wordBuffer << endl;

                                modifyStruct(token, type::EQUALS, dictionary, wordBuffer);
                                wordBuffer = "";
                                a.push_back(token);
                            }

                            wordBuffer += str;
                            cout << "buffer" << str << endl;
                            // modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);
                            // wordBuffer = "";
                            // a.push_back(token);
                            wordstate = 1;
                            stateIsNum = 1;
                        }

                        // cout << "word state != 1 \n";
                    }
                }
#pragma endregion
            }
            else
            {
                if (buffer != "")
                {
                    if (typeOfOP.find(buffer) != typeOfOP.end())
                    {
                        // Tokens token(buffer, typeOfOP[buffer]);
                        // token.dictionary = dictionary;
                        modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                        a.push_back(token);
                    }
                    else
                    {
                        // Tokens token(buffer, type::NUMBER);
                        // token.dictionary = dictionary;
                        modifyStruct(token, type::NUMBER, dictionary, buffer);
                        a.push_back(token);
                    }
                    buffer = "";
                }
                if (wordBuffer != "")
                {
                    modifyStruct(token, (typeOfOP.find(wordBuffer) != typeOfOP.end()) ? typeOfOP[wordBuffer] : type::WORD, dictionary, wordBuffer);
                    a.push_back(token);
                    wordBuffer = "";
                }
                // stateIsNum = 1;
            }
        }
        if (buffer != "")
        {
            if (typeOfOP.find(buffer) != typeOfOP.end())
            {
                // Tokens token(buffer, typeOfOP[buffer]);
                // token.dictionary = dictionary;
                modifyStruct(token, typeOfOP[buffer], dictionary, buffer);
                a.push_back(token);
            }
            else
            {
                // Tokens token(buffer, type::NUMBER);
                // token.dictionary = dictionary;
                modifyStruct(token, type::NUMBER, dictionary, buffer);
                a.push_back(token);
            }
        }
    }
    return a;
}
void printList(vector<Tokens> a)
{
    for (int i = 0; i < a.size(); i++)
    {
        cout << a[i].to_string() << endl;
    }
}
