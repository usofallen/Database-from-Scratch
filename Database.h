#pragma once
#include "Relation.h"
#include "predicate.h"
#include <map>

// AT 2nd recorded lab session 6:00 minutes
class Database
{
private:
public:
    map<string, Relation> dataMap;
    string toString()
    {
        stringstream out;
        for (pair<string, Relation> relationEntry : dataMap)
        {
            out << relationEntry.first;
            out << endl;
            out << relationEntry.second.toString();
        }

        return out.str();
    }

    Relation &getRelationByReference(string relationName)
    {
        return dataMap.at(relationName);
    }

    Relation getRelationCopy(string relationName)
    {
        return dataMap.at(relationName);
    }

    void insert(Relation r)
    {
        dataMap.insert({r.getName(), r});
    }

    unsigned int size()
    {
        return dataMap.size();
    }

    unsigned int returnRelationSize()
    {
        int totRelations = 0;
        //   map<string, Relation>::iterator it;
        // Iterate over idems in map, add numtuple for each
        for (pair<string, Relation> relationEntry : dataMap)
        {

            totRelations = relationEntry.second.getTuples().size();
        }
        return totRelations;
    }

    unsigned int numTuples()
    {
        unsigned int count = 0;
        for (auto p : dataMap)
        {
            count += p.second.size();
        }
        return count;
    }

    Relation evaluateQuery(Predicate query)
    {
        Relation realRelation = getRelationCopy(query.getName());
        //  vector<int> newIndices;
        // vector<string> newStrings;

        for (unsigned int i = 0; i < query.getParameters().size(); i++)
        {
            Parameter swagParameter = query.getParameters().at(i);

            if (swagParameter.at(0) == '\'')
            {
                realRelation = realRelation.select(i, swagParameter.getValue());
            }

            // it must be an ID
            else
            {
                // FIX THIS
                // bool duplicate = false;
                for (unsigned int j = 0; j < query.getParameters().size(); j++)
                {
                    if (swagParameter.getValue() == query.getParameters().at(j).getValue())
                    {
                        // duplicate = true;
                        realRelation = realRelation.select(i, j);
                    }
                }
            }
        }

        // use project to get rid of extra columns
        vector<int> newSchemePositions;
        vector<string> names;

        for (unsigned int i = 0; i < query.getParameters().size(); i++)
        {
            bool duplicate = false;
            Parameter value = query.getParameters().at(i);
            if (query.getParameters().at(i).at(0) != '\'')
            {
                // check if duplicate by popping through namea
                for (unsigned int j = 0; j < names.size(); j++)
                {
                    if (value.getValue() == names.at(j))
                    {
                        duplicate = true;
                    }
                }

                if (!duplicate)
                {
                    newSchemePositions.push_back(i);
                    // newStrings.push_back(query.getParameters().at(i).getValue());
                    names.push_back(query.getParameters().at(i).getValue());
                }
            }
        }

        realRelation = realRelation.project(newSchemePositions);
        realRelation = realRelation.rename(names);

        return realRelation;
    }

    void evalRules()
    {
    }
};