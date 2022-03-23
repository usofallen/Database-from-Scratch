#pragma once
#include <string>
#include <sstream>
using namespace std;

class Parameter
{
public:
    // TokenType type2;
    string data;
    Parameter(const string &data) : data(data)
    {
    }
    Parameter()
    {
    }

    const char &at(unsigned int index) const
    {
        return data.at(index);
    }

    Parameter getParameters()
    {
        return data;
    }
    void setValue(string value)
    {
        data = value;
    }
    string getValue()
    {
        return data;
    }

    // TokenType getType() const
    // {
    //     return type2;
    // }

    string toString()
    {
        stringstream ss;
        ss << data;
        return ss.str();
    }
};