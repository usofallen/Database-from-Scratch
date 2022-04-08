#pragma once
#include <string>
#include <vector>
#include "parameter.h"
using namespace std;

// SNAP(S, N, A, P)
class Predicate
{
public:
    string name;
    vector<Parameter> parameters;

    Predicate(const string &name) : name(name)
    {
        this->name = name;
        this->parameters = parameters;
    }

    void setName(string value)
    {
        name = value;
    }
    string getName()
    {
        return name;
    }

    Predicate()
    {
    }

    void addValue(Parameter value)
    {
        parameters.push_back(Parameter(value));
    }

    vector<Parameter> getParameters()
    {
        return parameters;
    }

    string toString()
    {
        stringstream ss;
        ss << name << "(";
        for (unsigned int i = 0; i < parameters.size(); i++)
        {
            ss << parameters.at(i).toString();
            if (i != parameters.size() - 1)
            {
                ss << ",";
            }
        }
        ss << ")";
        return ss.str();
    }
};