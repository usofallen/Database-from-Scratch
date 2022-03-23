#pragma once

#include <string>
#include <sstream>
#include <iostream>
using namespace std;

enum TokenType
{
    COMMA,
    PERIOD,
    Q_MARK,
    LEFT_PAREN,
    RIGHT_PAREN,
    COLON,
    COLON_DASH,
    MULTIPLY,
    ADD,
    SCHEMES,
    FACTS,
    RULES,
    QUERIES,
    ID,
    STRING,
    COMMENT,
    UNDEFINED,
    END_OF_FILE // ADD EOF
};

string tokenToString(TokenType type)
{

    switch (type)
    {
    case TokenType::COMMA:
        return "COMMA";
    case TokenType::PERIOD:
        return "PERIOD";
    case TokenType::Q_MARK:
        return "Q_MARK";
    case TokenType::LEFT_PAREN:
        return "LEFT_PAREN";
    case TokenType::RIGHT_PAREN:
        return "RIGHT_PAREN";
    case TokenType::COLON:
        return "COLON";
    case TokenType::COLON_DASH:
        return "COLON_DASH";
    case TokenType::MULTIPLY:
        return "MULTIPLY";
    case TokenType::ADD:
        return "ADD";
    case TokenType::SCHEMES:
        return "SCHEMES";
    case TokenType::FACTS:
        return "FACTS";
    case TokenType::RULES:
        return "RULES";
    case TokenType::QUERIES:
        return "QUERIES";
    case TokenType::ID:
        return "ID";
    case TokenType::STRING:
        return "STRING";
    case TokenType::COMMENT:
        return "COMMENT";
    case TokenType::UNDEFINED:
        return "UNDEFINED";
    case TokenType::END_OF_FILE:
        return "EOF";
    default:
        return "BAD TYPE";
    }
}

class Token
{
private:
    string value; // not char*
    TokenType type;
    unsigned int lineNum;

public:
    Token(const std::string &value, TokenType type, unsigned int lineNum) : value(value), type(type), lineNum(lineNum)
    {
    }

    const string &getValue() const
    {
        return value;
    }

    void setValue(const string &value)
    {
        Token::value = value;
    }

    TokenType getType() const
    {
        return type;
    }

    void setType(TokenType type)
    {
        Token::type = type;
    }

    unsigned int getLineNum() const
    {
        return lineNum;
    }

    void setLineNum(unsigned int lineNum)
    {
        Token::lineNum = lineNum;
    }

    // }
    string toString()
    {
        stringstream outSS;
        outSS << "(" << tokenToString(type) << ","
              << "\"" << value << "\""
              << "," << lineNum << ")";
        return outSS.str();
    }
};