#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Scheme
{
private:
    vector<string> attributes;

public:
    Scheme(vector<string> attributes) : attributes(attributes) {}
    Scheme() {}

    unsigned int size() const
    {
        return attributes.size();
    }

    // if out of bounds check here.
    const string &at(unsigned int index) const
    {
        return attributes.at(index);
    }

    void pushBack(string newAttribute)
    {
        attributes.push_back(newAttribute);
    }

    bool operator<(const Scheme &s) const
    {
        return this->attributes < s.attributes;
    }

    // TODO add more delegation functions as needed
};