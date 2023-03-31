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
    CL_PARENTHISIS
};
struct Tokens
{

    string buffer;
    type id;
    map<type, string> dictionary;
};

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
    cout << "input: " << endl;
    while (getline(infile, line))
    {
        cout << line + "\n";
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
    map<type, string> dictionary;
    dictionary[type::NUMBER] = "NUMBER";
    dictionary[type::ADDITION] = "ADDITION";
    dictionary[type::SUBTRACT] = "SUBTRACT";
    dictionary[type::DIVISION] = "DIVISION";
    dictionary[type::MULTIPLY] = "MULTIPLY";
    dictionary[type::OP_PARENTHISIS] = "OP_PARENTHISIS";
    dictionary[type::CL_PARENTHISIS] = "CL_PARENTHISIS";

    map<string, type> typeOfOP;
    typeOfOP["+"] = type::ADDITION;
    typeOfOP["-"] = type::SUBTRACT;
    typeOfOP["/"] = type::DIVISION;
    typeOfOP["*"] = type::MULTIPLY;
    typeOfOP["("] = type::OP_PARENTHISIS;
    typeOfOP[")"] = type::CL_PARENTHISIS;

    int state = 1;
    regex opRegex("[+*/]"); // Match any word that starts with 'q'

    regex numReg("[0-9]"); // Match any word that starts with 'q'
    std::smatch myMatch;
    Tokens token;
    vector<Tokens> a;
    for (int i = 0; i < lines.size(); i++)
    {
        string line = lines[i];
        string buffer = "";
        for (int i2 = 0; i2 < line.length(); i2++)
        {
            char current = line.at(i2);
            string str(1, current);
            if (current != ' ')
            {
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
                            cout << "sub \n";

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
                        cout << buffer << endl;

                        state = 1;
                    }
                }
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
                }
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
        cout << a[i].dictionary[a[i].id] + "(" + a[i].buffer + ") \n";
    }
}
