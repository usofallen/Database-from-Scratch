#pragma once
#include "datalogProgram.h"
#include "Database.h"
#include "Relation.h"

class Interpreter
{
private:
    datalogProgram program;
    Database database;

public:
    Interpreter(datalogProgram program) : program(program)
    {
    }

    vector<string> flatten(vector<Parameter> vectorOfParameters)
    {
        vector<string> flattenedParameters;
        for (unsigned int i = 0; i < vectorOfParameters.size(); i++)
        {
            flattenedParameters.push_back(vectorOfParameters.at(i).getValue());
        }
        return flattenedParameters;
    }

    void run()
    {
        evalSchemes();
        evalFacts();
        // evalRules(); IGNORE FOR NOW. Project 4.
        evalQueries();
    }

    void evalSchemes()
    {
        for (auto &scheme : program.getSchemes())
        {
            Relation newRelation(scheme.getName(), flatten(scheme.getParameters()));
            database.insert(newRelation);
        }
        // for each scheme s in program.schemes
        //  make a new relation r
        //  make a new scheme newScheme
        //  set name of relation to name of s
        //  for each paramater p in s
        //   push_back p.tostring into newScheme
        //   add newScheme into r
        //  add r to our database
    }

    void evalFacts()
    {
        for (auto &tuple : program.getFacts())
        {
            Relation &r = database.getRelationByReference(tuple.getName()); // CHECK:: RelationCOpy or by reference??
            vector<Parameter> parameter = tuple.getParameters();

            Tuple tSwag = Tuple(flatten(parameter));
            r.addTuple(tSwag);
        }
        // for each fact f in program.facts
        //  get relation r by reference from the database
        //  make a new tuple newTuple
        //  for each paramater p in f
        //   push_back p.tostring into tuple newTuple
        //   add newTuple into r
    }
    // MOVED TO DATABASE vvvvv
    void evalQueries()
    {
        cout << endl;
        cout << "Query Evaluation" << endl;
        for (auto &query : program.getQueries())
        {
            Relation queryEvaluated = database.evaluateQuery(query);

            cout << query.toString() << "? ";

            if (queryEvaluated.size() > 0)
            {
                cout << "Yes(" << queryEvaluated.size() << ")" << endl;
            }

            else
            {
                cout << "No" << endl;
            }

            cout << queryEvaluated.toString();
        }
    }

    Relation evaluatePredicate(Predicate predicate)
    {
        Relation currentRelation = database.getRelationCopy(predicate.getName());
        vector<int> newSchemePositions;
        vector<string> names;

        for (unsigned int i = 0; i < predicate.getParameters().size(); i++)
        {
            bool duplicate = false;
            Parameter value = predicate.getParameters().at(i);
            if (predicate.getParameters().at(i).at(0) != '\'')
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
                    names.push_back(predicate.getParameters().at(i).getValue());
                }
            }
        }

        currentRelation = currentRelation.project(newSchemePositions);
        currentRelation = currentRelation.rename(names);

        return currentRelation;
    }

    void evalRules()
    {
        unsigned int sizeBefore = 0;
        unsigned int sizeAfter = 0;
        int numPasses = 0;

        cout << "Rule Evaluation" << endl;

        do // PUT BACK IN THIS DO WHILE LOOP once you kNOW SINGLE PASS WORKS
        {
            numPasses++;
            sizeBefore = database.size();
            evalRuleListOnce(program.getRules());
            sizeAfter = database.size();
        } while (sizeBefore != sizeAfter);
        cout << endl;
        cout << "Schemes populated after " << numPasses << " passes through the Rules." << endl;
    }

    void evalRuleListOnce(vector<Rule> rules)
    {
        for (Rule currRule : rules)
        {
            for (Rule currRule : rules)
            {
                cout << currRule.toString() << "." << endl;
                vector<Relation> bodyRelations;
                for (Predicate currPredicate : currRule.getBodyPredicates())
                {
                    Relation currRelation = evaluatePredicate(currPredicate); // fix evaluatePredicate, or figure out how to do it differently.
                    bodyRelations.push_back(currRelation);
                }

                Relation resultRelation = bodyRelations.at(0);
                for (unsigned int i = 1; i < bodyRelations.size(); i++) // starst at 1
                {
                    resultRelation = resultRelation.natJoin(resultRelation, bodyRelations.at(i));
                }

                // vector<int> indicies;
                // for (unsigned int i = 0; i < rules.at(i).getHeadPredicate().getParameters().size(); i++)
                // {
                //     for (unsigned int j = 0; j < resultRelation.getScheme().size(); j++)
                //     {
                //         if (rules.at(i).getHeadPredicate().getParameters().at(j).getValue() == resultRelation.getScheme().at(j)) //.getValue might be wrong
                //         {
                //             indicies.push_back(j);
                //         }
                //     }
                // }

                // resultRelation = resultRelation.project(indicies);

                // cn(c,n) :- snap+csg(S,n,A,P,c,G)
                // Project({4,1})
                // TODO figure out project ish here.
                string name = currRule.getHeadPredicate().getName();
                resultRelation = resultRelation.rename(database.getRelationByReference(name).getScheme());

                database.getRelationByReference(name).unionize(resultRelation);
            }
        }
    }

    // RUN AN IMPUT AND RUN YOUR DATABASE TOSTRING AND MAKE SURE WHAT GOT PRINTED OUT MATCHES WHAT YOU WERE EXPECTING.
};