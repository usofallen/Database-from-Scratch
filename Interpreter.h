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
        // unsigned int sizeBefore = 0;
        // unsigned int sizeAfter = 0;
        int numPasses = 0;
        cout << "Rule Evaluation" << endl;
        // do // PUT BACK IN THIS DO WHILE LOOP once you kNOW SINGLE PASS WORKS
        // {
        numPasses++;
        // sizeBefore = database.size();
        evalRuleListOnce(program.getRules());
        // sizeAfter = database.returnRelationSize();
        //  } while (sizeBefore != sizeAfter);
        cout << endl;
        cout << "Schemes populated after " << numPasses << " passes through the Rules." << endl;
    }

    void evalRuleListOnce(vector<Rule> rules)
    {

        //     cout << "Rule Evaluation" << endl;
        //     bool moreTuples = true;
        //     int rulesPass = 0;
        //     while (moreTuples)
        //     {
        //         moreTuples = false;
        //         vector<bool> shouldGoOn;
        //         rulesPass++;
        //         for (unsigned int i = 0; i < rules.size(); i++)
        //         {
        //             cout << rules.at(i).toString() << endl;
        //             vector<Relation> theRelations;
        //             for (unsigned int j = 0; j < rules.at(i).getBodyPredicates().size(); j++)
        //             {
        //                 Relation thisRelation = evaluatePredicate(rules.at(i).getBodyPredicates().at(j));
        //                 theRelations.push_back(thisRelation);
        //             }

        //             // Join
        //             Relation thisRelation;
        //             if (theRelations.size() > 1)
        //             {
        //                 thisRelation = theRelations.at(0);
        //                 for (unsigned int j = 0; j < theRelations.size() - 1; j++)
        //                 {

        //                     thisRelation = thisRelation.natJoin(thisRelation, theRelations.at(j + 1));
        //                 }
        //             }
        //             else
        //             {
        //                 thisRelation = theRelations.at(0);
        //             }
        //             // Project
        //             // What the heck is going on???
        //             vector<int> Indicies;
        //             for (unsigned int j = 0; j < rules.at(i).getHeadPredicate().getParameters().size(); j++)
        //             {
        //                 // cout << Rules.at(i).Head.parameterList.at(j).ToString() << " | " << thisRelation.TheScheme.Fake();
        //                 for (unsigned int k = 0; k < thisRelation.getScheme().size(); k++)
        //                 {
        //                     // cout << Rules.at(i).Head.parameterList.at(j).theParameter << "|" << thisRelation.TheScheme.At(k) << endl;
        //                     if (rules.at(i).getHeadPredicate().getParameters().at(j).getValue() == thisRelation.getScheme().at(k))
        //                     {
        //                         Indicies.push_back(k);
        //                         // cout << "got pushed back" << endl;
        //                     }
        //                 }
        //             }
        //             // cout << "indicies: " <<Indicies.size() << endl;
        //             thisRelation = thisRelation.project(Indicies);
        //             // Rename
        //             Relation aRelation = thisRelation.setScheme(name);
        //             aRelation = rules.at(i).getHeadPredicate().getName();
        //             if (database.getRelationByReference(thisRelation.getName()).getScheme().size() == thisRelation.getScheme().size())
        //             {
        //                 thisRelation.getScheme() = (database.getRelationByReference(thisRelation.getName()).getScheme());
        //             }
        //             else
        //             {
        //                 // cout << "abort" << endl;
        //             }
        //             /*
        //             cout << "Scheme end:" ;
        //             for (unsigned int i = 0; i < theDatabase.databaseseses.at(thisRelation.name).TheScheme.values.size(); i++) {
        //                 cout << theDatabase.databaseseses.at(thisRelation.name).TheScheme.values.at(i);
        //             }
        //             cout << endl;
        //             thisRelation.Rename(theDatabase.databaseseses.at(thisRelation.name).TheScheme.values);
        //             cout << "Scheme end:" ;
        //             for (unsigned int i = 0; i < thisRelation.TheScheme.values.size(); i++) {
        //                 cout << thisRelation.TheScheme.values.at(i);
        //             }
        //             */
        //             // Union
        //             shouldGoOn.push_back(database.getRelationByReference(thisRelation.getName()).canJoin(thisRelation.getTuples);

        //             // output
        //             /*
        //             if (theDatabase.databaseseses.at(thisRelation.name).DemTuples.size() == 0) {}
        //             else {
        //                 if (theDatabase.databaseseses.at(thisRelation.name).BeenOutPutted == false) {
        //                     thisRelation.ToString();
        //                 }
        //             }
        //             */
        //         }
        //         for (unsigned int a = 0; a < shouldGoOn.size(); a++)
        //         {
        //             if (shouldGoOn.at(a))
        //             {
        //                 moreTuples = true;
        //             }
        //         }
        //     }
        //     cout << endl;
        //     cout << "Schemes populated after " << rulesPass << " passes through the Rules." << endl;
        //     cout << endl;
        //     return;
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

                vector<int> indicies; // check this code block for bugs
                for (unsigned int i = 0; i < currRule.getHeadPredicate().getParameters().size(); i++)
                {
                    for (unsigned int j = 0; j < resultRelation.getScheme().size(); j++)
                    {
                        if (currRule.getHeadPredicate().getParameters().at(i).getValue() == resultRelation.getScheme().at(j)) //.getValue might be wrong
                        {
                            indicies.push_back(j);
                        }
                    }
                }

                resultRelation = resultRelation.project(indicies);

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