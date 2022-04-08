#pragma once
#include <ostream>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <set>
#include <map>
#include "Scheme.h"
#include "Tuple.h"
using namespace std;

class Relation
{

private:
    string name;
    Scheme scheme;
    set<Tuple> tuples;

public:
    Relation(const string &name, const Scheme &scheme) : name(name), scheme(scheme) {}
    Relation(const string &name, const Scheme &scheme, set<Tuple> &tuples) : name(name), scheme(scheme), tuples(tuples) {}
    Relation(const string &name, set<Tuple> &tuples) : name(name), tuples(tuples) {}
    Relation() {}

    // Getters & Setters vvvvvvvv
    const string &getName() const
    {
        return name;
    }

    void setName(const string &name)
    {
        Relation::name = name;
    }

    const Scheme &getScheme() const
    {
        return scheme;
    }

    void setScheme(const Scheme &scheme)
    {
        Relation::scheme = scheme;
    }

    const set<Tuple> &getTuples() const
    {
        return tuples;
    }

    void setTuples(const Scheme &scheme)
    {
        Relation::tuples = tuples;
    }
    // Getters & Setters ^^^^^^^

    bool addTuple(const Tuple &tuple) // if the tuple was not in the set this returns true, otherwise, false
    {
        return tuples.insert(tuple).second;
    }

    string toString() const
    {
        stringstream out;
        for (Tuple tuple : tuples)
        {
            if (tuple.size() != 0)
            {
                out << "  " << tuple.toString(scheme) << endl;
            }
        }

        return out.str();
    }

    Relation select(int index, const string &value) const
    {
        Relation output(name, scheme);
        for (auto &tuple : tuples)
        {
            if (tuple.at(index) == value)
            {
                output.addTuple(tuple);
            }
        }
        return output;
    }

    // how to do the version of select where it takes in two indexes. ??
    Relation select(unsigned int index1, unsigned int index2)
    {
        Relation output(name, scheme);
        for (auto &tuple : tuples)
        {
            if (tuple.at(index1) == tuple.at(index2)) // CHECK FOR BUGSSS!!
            {
                output.addTuple(tuple);
            }
        }
        return output;
    }

    Relation project(vector<int> colsToKeep)
    {
        Relation output; // not pass in something??
        output.setName(name);

        Scheme TempScheme;
        for (unsigned int i = 0; i < colsToKeep.size(); i++)
        {
            TempScheme.pushBack(scheme.at(colsToKeep.at(i)));
        }
        output.setScheme(TempScheme);

        for (auto tuple : tuples)
        {
            Tuple tempTuple;
            for (unsigned j = 0; j < colsToKeep.size(); j++)
            {
                tempTuple.pushBack(tuple.at(colsToKeep.at(j)));
            }
            output.addTuple(tempTuple);
        }
        return output;
    }

    // Only updates scheme
    Relation rename(Scheme newNames)
    {
        Relation output(name, tuples);
        Scheme TempScheme;
        output.setScheme(newNames);
        return output;
    }

    unsigned int size()
    {
        return tuples.size();
    }

    Relation natJoin(Relation r1, Relation r2)
    {
        Relation output;
        output.setName(r1.getName() + "+" + r2.getName());
        // identify overlap in the schemes(header)
        map<int, int> overlap;
        vector<unsigned int> uniqueColumns;

        for (unsigned int i = 0; i < r2.getScheme().size(); i++)
        {
            bool isUnique = true;
            for (unsigned int j = 0; j < r1.getScheme().size(); j++)
            {
                if (r2.getScheme().at(i) == r1.getScheme().at(j))
                {
                    overlap.insert({j, i});
                    isUnique = false;
                }
            }
            if (isUnique)
            {
                uniqueColumns.push_back(i);
            }
        }

        output.setScheme(combineSchemes(r1.getScheme(), r2.getScheme(), uniqueColumns));

        // TODO For each tuple check if you can combine them, then combine them., then insert into output relation
        // combineSchemes()
        for (Tuple t1 : r1.getTuples())
        {
            for (Tuple t2 : r2.getTuples())
            {
                if (canJoin(t1, t2, overlap))
                {
                    output.addTuple(combineTuples(t1, t2, uniqueColumns));
                }
            }
        }

        return output;

        // AT 20.19
    }

    Scheme combineSchemes(const Scheme &s1, const Scheme &s2, vector<unsigned int> &uniqueColumns) // MAYBE PASS BY REFERENCE IN THE FUTURE
    {
        Scheme output;
        for (unsigned int i = 0; i < s1.size(); i++)
        {
            output.pushBack(s1.at(i));
        }
        for (unsigned int i = 0; i < uniqueColumns.size(); i++)
        {
            output.pushBack(s2.at(uniqueColumns.at(i)));
        }
        return output; // check for bugs
    }

    Tuple combineTuples(Tuple &t1, Tuple &t2, vector<unsigned int> &uniqueColumns)
    {
        // SUPPOSED TO BE VERY SIMILAR TO COMBOINE SCHEMES> in first walter video.
        Tuple output;
        // start with left tuple, add unique columns of right tuple at end.
        for (unsigned int i = 0; i < t1.size(); i++)
        {
            output.pushBack(t1.at(i));

            // output.at(i) = t1.at(i);
            // give all of tuple 1, only add a row of tuple 2 if it is not in tuple1
        }
        for (unsigned int i = 0; i < uniqueColumns.size(); i++)
        {
            output.pushBack(t2.at(uniqueColumns.at(i)));
        }

        return output;
    }

    bool canJoin(Tuple &t1, Tuple &t2, map<int, int> overlap)
    {
        for (pair<unsigned int, unsigned int> p : overlap)
        {
            if (t1.at(p.first) != t2.at(p.second))
            {
                return false;
            }
        }
        return true;
    }

    bool unionize(Relation toAdd)
    {
        bool ifTrue = false;
        if (scheme.at(0) != toAdd.getScheme().at(0))
        {
            throw "Cannot combine b/c the schemes are different";
        }
        for (Tuple tuple : toAdd.getTuples())
        {
            if (addTuple(tuple))
            {
                if (tuple.size() > 0)
                {
                    cout << "  " << tuple.toString(scheme) << endl;
                }
                ifTrue = true;
                // know which ones you added. to get the rows you got out, make a different function. Call remove on every old one.
            }
        }
        return ifTrue;
    }

    //     // for the lab, check this out if i have questions but walters video is prolly best.
    //     static bool joinable(const Scheme &leftScheme, const Scheme &rightScheme, const Tuple &leftTuple, const Tuple &rightTuple)
    //     {
    //         for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++)
    //         {
    //             const string &leftName = leftScheme.at(leftIndex);
    //             const string &leftValue = leftTuple.at(leftIndex);
    //             cout << "left name: " << leftName << " value: " << leftValue << endl;
    //             for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++)
    //             {
    //                 const string &rightName = rightScheme.at(rightIndex);
    //                 const string &rightValue = rightTuple.at(rightIndex);
    //                 cout << "right name: " << rightName << " value: " << rightValue << endl;
    //             }
    //         }

    //         return true;
    //     }
    //     Relation join(const Relation &r)
    //     {
    //         const Scheme &leftScheme = scheme;
    //         for (Tuple leftTuple : tuples)
    //         {
    //             cout << "left tuple: " << leftTuple.toString(leftScheme) << endl;
    //             const Scheme &rightScheme = r.scheme;
    //             for (Tuple rightTuple : r.tuples)
    //             {
    //                 cout << "right tuple: " << rightTuple.toString(rightScheme) << endl;
    //             }
    //         }

    //         return Relation();
    //     }
};