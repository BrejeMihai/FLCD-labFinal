#include "scanner.h"
#include "node.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <map>
#include <regex>
#include <iostream>

bool SplitTokenRead(std::string Token, std::string fileLine, int numberOfLines, std::vector<std::pair<std::string, std::pair<int, int>>>& tokenList)
{
    bool check = false;
    std::regex e("([\\(\\)\\{\\},~\\[\\]:!=])|([^\\(\\)\\{\\},~\\[\\]:!=]+)");
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    while (rit != rend)
    {
        size_t found = fileLine.find(rit->str());
        tokenList.push_back(std::pair<std::string, std::pair<int, int>>(rit->str(), std::pair<int, int>(numberOfLines, found)));
        ++rit;
        check = true;
    }
    return check;
}

bool IsSeparatorOrOperator(std::string Token)
{
    const char* arr[] = { "!", "[", "]", "(", ")", "{", "}", " ", ":", ";", ",", "~", "+", "-", "*", "/", "^", "=" };
    for (auto c : arr)
    {
        if (0 == Token.compare(c))
        {
            return true;
        }
    }
    return false;
}
bool IsReservedWord(std::string Token)
{
    const char* arr[] = { "int", "char", "string", "listof", "incase", "ifnot", "then", "gountil", "do", "input", "echo", "lt", "lte", "equals", "ne", "gt", "gte", "increment", "decrement" };
    for (auto c : arr)
    {
        if (0 == Token.compare(c))
        {
            return true;
        }
    }
    return false;
}


bool CheckIfExpression(std::string Token)
{
    std::regex e("^-*[1-9][0-9]*[\\+\\*/-]-*[1-9][0-9]*$");
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    return !(rit == rend);
}

bool CheckIfProperNumericalValue(std::string Token)
{
    std::regex e("^-*[1-9][0-9]*$");
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    return !(rit == rend);
}

bool CheckIfProperIdentifier(std::string Token)
{
    std::regex e("^[a-zA-Z][0-9a-zA-Z]*$");
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    return !(rit == rend);
}

bool CheckIfConstChar(std::string Token)
{
    std::regex e("^\'.\'$");
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    return !(rit == rend);
}

bool CheckIfConstString(std::string Token)
{
    std::regex e("^\".*\"$");
    std::regex_iterator<std::string::iterator> rit(Token.begin(), Token.end(), e);
    std::regex_iterator<std::string::iterator> rend;

    return !(rit == rend);
}

bool CheckIfConst(std::string Token)
{
    return CheckIfProperNumericalValue(Token) | CheckIfConstString(Token) | CheckIfConstChar(Token) | CheckIfExpression(Token);
}

bool CheckIfIdentifier(std::string Token)
{
    return CheckIfProperIdentifier(Token);
}


void StartScanning(char* InputFilePath)
{
    std::vector<std::pair<std::string, std::pair<int, int>>> tokenList;


    std::string inputPath = InputFilePath;

    std::vector<std::pair<std::string, int>> pif;

    std::vector<std::string> listOfTokens;
    std::string token;



    std::string fileLine;

    int numberOfLines = 1;

    std::string stoutput;

    std::ifstream inputFile(inputPath);
    std::ofstream piffile("./PIF.out");
    std::ofstream stfile("./ST.out");

    HashTable hashTable;


    while (std::getline(inputFile, fileLine))
    {
        std::istringstream iss(fileLine);
        while (iss >> token)
        {
            if (!(SplitTokenRead(token, fileLine, numberOfLines, tokenList)))
            {
                size_t found = fileLine.find(token);
                tokenList.push_back(std::pair<std::string, std::pair<int, int>>(token, std::pair<int, int>(numberOfLines, found)));
            }
        }
        numberOfLines++;
    }


    for (std::pair<std::string, std::pair<int,int>> singleTokenPair : tokenList)
    {
        if (IsReservedWord(singleTokenPair.first) || IsSeparatorOrOperator(singleTokenPair.first))
        {
            pif.push_back(std::pair<std::string,int>(singleTokenPair.first,-1));
        }
        else
        {
            if (CheckIfIdentifier(singleTokenPair.first))
            {
                hashTable.insertNode(singleTokenPair.first);
                pif.push_back(std::pair<std::string, int>("0", hashTable.getPosition(singleTokenPair.first)));
            }
            else if (CheckIfConst(singleTokenPair.first))
            {
                hashTable.insertNode(singleTokenPair.first);
                pif.push_back(std::pair<std::string, int>("1", hashTable.getPosition(singleTokenPair.first)));     
            }
            else
            {
                fprintf_s(stderr, "LEXICAL ERROR\nLine %d, column %d\n", singleTokenPair.second.first, singleTokenPair.second.second);
                return;
            }
        }
    }
    fprintf_s(stdout, "Lexically correct!\n");

    piffile << "PIF\n";
    for (auto c : pif)
    {
        piffile << c.first << " " << c.second << std::endl;
    }

    stoutput = hashTable.displayToString();
    stfile << "Symbol Table\n";
    stfile << stoutput;

    piffile.flush();
    piffile.close();
    stfile.flush();
    stfile.close();
}