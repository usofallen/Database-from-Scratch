#pragma once
#include <string>
#include <vector>
#include "parameter.h"
#include "predicate.h"
using namespace std;
class Rule
{
public:
    Predicate headPredicate;
    vector<Predicate> bodyPredicates;

    Rule()
    {
    }

    void setHeadPredicate(Predicate input)
    {
        headPredicate = input;
    }

    Predicate getHeadPredicate()
    {
        return headPredicate;
    }

    void setBodyPredicates(vector<Predicate> input)
    {
        bodyPredicates = input;
    }

    vector<Predicate> getBodyPredicates()
    {
        return bodyPredicates;
    }

    void addValue(Predicate value)
    {
        bodyPredicates.push_back(value);
    }

    // a function that returns a rules size
    unsigned int size()
    {
        return bodyPredicates.size();
    }

    string toString()
    {
        stringstream ss;
        ss << headPredicate.toString() << " :- ";
        for (unsigned int i = 0; i < bodyPredicates.size(); i++)
        {
            ss << bodyPredicates.at(i).toString();
            if (i != bodyPredicates.size() - 1)
            {
                ss << ",";
            }
        }
        ss << ".";
        return ss.str();
    }
};