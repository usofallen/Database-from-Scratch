#pragma once
#include "predicate.h"
#include "parameter.h"
#include "rule.h"
//#include "Relation.h"
#include <set>
using namespace std;

class datalogProgram
{
private:
    vector<Predicate> Schemes;
    vector<Predicate> Facts;
    vector<Rule> Rules;
    vector<Predicate> Queries;

public:
    datalogProgram() {}

    datalogProgram(vector<Predicate> Schemes, vector<Predicate> Facts, vector<Rule> Rules, vector<Predicate> Queries)
    {
        this->Schemes = Schemes;
        this->Facts = Facts;
        this->Rules = Rules;
        this->Queries = Queries;
    }

    set<string> getDomain()
    {
        set<string> domain;

        for (unsigned int i = 0; i < Facts.size(); i++)
        {
            Predicate fact = Facts.at(i);
            vector<Parameter> parameters = fact.getParameters();

            for (unsigned int j = 0; j < parameters.size(); j++)
            {
                Parameter parameter = parameters.at(j);

                string paramaterValue = parameter.getValue();

                if (!(domain.find(paramaterValue) != domain.end()))
                {
                    domain.insert(paramaterValue);
                }
            }
        }
        return domain;
    }
    // datalogProgram getDatalogProgram()
    // {
    // }

    void setSchemes(vector<Predicate> input)
    {
        Schemes = input;
    }

    vector<Predicate> getSchemes()
    {
        return Schemes;
    }

    // Relation getRelation(Relation value)
    // {
    //     return value;
    // }

    void setFacts(vector<Predicate> input)
    {
        Facts = input;
    }

    vector<Predicate> getFacts()
    {
        return Facts;
    }
    void setQueries(vector<Predicate> input)
    {
        Queries = input;
    }

    vector<Predicate> getQueries()
    {
        return Queries;
    }

    void setRules(vector<Rule> input)
    {
        Rules = input;
    }

    vector<Rule> getRules()
    {
        return Rules;
    }

    string toString()
    {
        stringstream ss;
        set domain = getDomain();
        ss << "Schemes(" << Schemes.size() << "):" << endl;
        for (unsigned int i = 0; i < Schemes.size(); i++)
        {
            ss << "  " << Schemes.at(i).toString() << endl;
        }

        ss << "Facts(" << Facts.size() << "):" << endl;
        for (unsigned int i = 0; i < Facts.size(); i++)
        {
            ss << "  " << Facts.at(i).toString() << "." << endl;
        }

        ss << "Rules(" << Rules.size() << "):" << endl;
        for (unsigned int i = 0; i < Rules.size(); i++)
        {
            ss << "  " << Rules.at(i).toString() << "." << endl;
        }

        ss << "Queries(" << Queries.size() << "):" << endl;
        for (unsigned int i = 0; i < Queries.size(); i++)
        {
            ss << "  " << Queries.at(i).toString() << "?" << endl;
        }

        ss << "Domain(" << domain.size() << "):" << endl;
        set<string>::iterator itr;
        for (itr = domain.begin(); itr != domain.end(); itr++)
        {
            ss << *itr << endl;
        }

        return ss.str();
    }
    // tostring call tostring of all children
};

// Vecor predicates called schemes
// vector predicates called facts
// vector rules called rules
// vector of predicates called queries

// tostring function...