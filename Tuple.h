#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "Scheme.h"
using namespace std;

class Tuple
{

private:
    vector<string> values;

public:
    Tuple(vector<string> values) : values(values) {}
    Tuple() {}

    unsigned int size()
    {
        return values.size();
    }

    const string &at(int index) const
    {
        return values.at(index);
    }

    void pushBack(string newValue)
    {
        values.push_back(newValue);
    }

    bool operator<(const Tuple t) const
    {
        return this->values < t.values;
    }

    string toString(const Scheme &scheme)
    {
        stringstream out;
        if (scheme.size() != values.size())
        {
            throw "Scheme and tuple are of different sizes";
        }

        string sep = "";
        for (unsigned i = 0; i < size(); i++)
        {
            out << sep << scheme.at(i) << "=" << values.at(i);
            sep = ", ";
        }
        return out.str();
    }

    // TODO: add more delegation functions as needed
};