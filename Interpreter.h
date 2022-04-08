#pragma once
#include "datalogProgram.h"
#include "Database.h"
#include "Relation.h"
#include "graph.h"

class Interpreter
{
private:
    datalogProgram program;
    Database database;

public:
    Interpreter() {}
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

    void evalSchemes()
    {
        for (auto &scheme : program.getSchemes())
        {
            Relation newRelation(scheme.getName(), flatten(scheme.getParameters()));
            database.insert(newRelation);
        }
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
            Parameter swagParameter = predicate.getParameters().at(i);

            if (swagParameter.at(0) == '\'')
            {
                currentRelation = currentRelation.select(i, swagParameter.getValue());
            }

            // it must be an ID
            else
            {
                // FIX THIS
                // bool duplicate = false;
                for (unsigned int j = 0; j < predicate.getParameters().size(); j++)
                {
                    if (swagParameter.getValue() == predicate.getParameters().at(j).getValue())
                    {
                        // duplicate = true;
                        currentRelation = currentRelation.select(i, j);
                    }
                }
            }
        }
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

    pair<Graph, Graph> makeGraph(vector<Rule> rules) // clues at 13.00 of Walter's first video
    {
        Graph graph1(rules.size());
        Graph graph2(rules.size());
        for (unsigned int fromID = 0; fromID < rules.size(); fromID++)
        {
            Rule fromRule = rules.at(fromID);
            vector<Predicate> predicates = fromRule.getBodyPredicates();
            // cout << "from rule R" << fromID << ": " << fromRule.toString() << endl;
            for (unsigned int predID = 0; predID < predicates.size(); predID++)
            {
                Predicate currPredicate = predicates.at(predID);
                for (unsigned int toID = 0; toID < rules.size(); toID++)
                {
                    Rule toRule = rules.at(toID);
                    if (toRule.getHeadPredicate().getName() == currPredicate.getName())
                    {
                        // cout << "to rule R" << toID << ": " << toRule.toString() << endl;
                        graph1.addEdge(fromID, toID);
                        graph2.addEdge(toID, fromID);
                    }
                }
            }
        }
        return {graph1, graph2};
    }

    // TIPS: make the EvalRules only call one function, all that happens below should be in one function. That will make your life easier.

    bool ruleDependsOnSelf(Graph graph, unsigned int index)
    {
        Node node = graph.getNode(index);
        for (auto &adjacentNodeID : node.getAdjacentNodeIDs())
        {
            if (adjacentNodeID == index)
            {
                return true;
            }
        }
        return false;
    }
    // scc tostring
    string sccToString(vector<int> scc)
    {
        stringstream ss;
        for (auto &nodeID : scc)
        {
            ss << "R" << nodeID << (nodeID == scc.back() ? "" : ",");
        }
        return ss.str();
    }

    // a tostring function for getTopologicalSort
    string toString(vector<int> v)
    {
        string s = "";
        for (unsigned int i = 0; i < v.size(); i++)
        {
            s += to_string(v.at(i)) + " ";
        }
        return s;
    }

    void evaluateSCC(vector<int> scc)
    {

        // int numPasses = 0;
        //  bool addTuples = false;
        cout << sccToString(scc) << endl;
        return;
        // for (unsigned int j = 0; j < scc.size(); j++)
        //{

        // numPasses++;
        // for (unsigned int i = 0; i < scc.size(); i++)
        // {
        //     // int id = scc.at(i);

        //     Rule rule = rules.at(i);

        //     // cout << rule.toString() << "." << endl;
        //     evalRuleListOnce(rules, scc);

        //     Scheme resultScheme = database.getRelationByReference(rule.getHeadPredicate().getName()).getScheme();
        // }

        // cout << numPasses << " passes: ";

        // for (auto &nodeID : scc)
        // {
        //     cout << "R" << nodeID;
        //     cout << (nodeID == scc.back() ? "" : ",");
        // }
        // cout << endl;
    }

    void evalRule(Rule currRule)
    {
        cout << currRule.toString() << endl;
    }

    bool evalRuleListOnce(vector<Rule> rules)
    {
        bool found = false;
        for (Rule currRule : rules)
        {

            cout << currRule.toString() << endl;
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

            // a nested for loop that goes through the parameters of the head predicate and checks if they are in the scheme of the result relation.
            // if they are not, then the scheme is updated.
            vector<int> indicies; // check this code block for bugs
            for (unsigned int i = 0; i < currRule.getHeadPredicate().getParameters().size(); i++)
            {
                for (unsigned int j = 0; j < resultRelation.getScheme().size(); j++)
                {
                    if (currRule.getHeadPredicate().getParameters().at(i).getValue() == resultRelation.getScheme().at(j))
                    {
                        indicies.push_back(j);
                    }
                }
            }
            resultRelation = resultRelation.project(indicies);
            string name = currRule.getHeadPredicate().getName();
            resultRelation = resultRelation.rename(database.getRelationByReference(name).getScheme());

            if (database.getRelationByReference(name).unionize(resultRelation))
            {
                found = true;
            }
            //}
        }
        return found;
    }

    void depthFirstSearchForest(Graph &graph, stack<int> &nodeStack)
    {

        // for each node
        for (auto &pair : graph.getNodes())
        {
            int nodeID = pair.getNodeID();

            // recursively call depthFirstSearch
            depthFirstSearch(graph, nodeID, nodeStack);
        }
    }

    static void depthFirstSearch(Graph &graph, int startNodeID, stack<int> &nodeStack)
    {

        if (graph.getNode(startNodeID).getVisited())
        {
            return;
        }

        // get the node by the id
        Node &startNode = graph.getNode(startNodeID);

        // mark the node as visited
        startNode.setVisited(true);

        // for each adjacent node
        for (auto &adjacentNodeID : startNode.getAdjacentNodeIDs())
        {
            // if the adjacent node is not visited
            if (!graph.getNode(adjacentNodeID).getVisited())
            {
                // recursively call depthFirstSearch
                depthFirstSearch(graph, adjacentNodeID, nodeStack);
            }
        }

        // push the node to the stack
        nodeStack.push(startNodeID);
    }

    // a function that reverses the odeder of the edges in the graph
    Graph reverseGraph(Graph &graph)
    {
        for (auto &pair : graph.getNodes())
        {
            int nodeID = pair.getNodeID();
            Node &node = graph.getNode(nodeID);
            set<unsigned int> adjacentNodeIDs = node.getAdjacentNodeIDs();
            for (auto &adjacentNodeID : adjacentNodeIDs)
            {
                graph.removeEdge(nodeID, adjacentNodeID);
                graph.addEdge(adjacentNodeID, nodeID);
            }
        }
        return graph;
    }

    vector<int> getTopologicalSort(Graph graph)
    {
        // reverse the graph
        // Graph reverseGraphSwag = reverseGraph(graph);

        stack<int> stack;

        vector<int> topologicalSort;

        depthFirstSearchForest(graph, stack);

        while (!stack.empty())
        {
            topologicalSort.push_back(stack.top());
            stack.pop();
        }

        return topologicalSort;
    }

    vector<vector<int>> getSCCs(Graph forward, Graph backward)
    {
        vector<vector<int>> sccs;

        // get topological sort
        vector<int> topologicalSort = getTopologicalSort(backward);

        // for each node in topological sort
        for (int nodeID : topologicalSort)
        {
            // if the node is not visited
            if (!forward.getNode(nodeID).getVisited())
            {
                // create a new stack
                stack<int> scc;

                // call depth first search on the node
                depthFirstSearch(forward, nodeID, scc);

                // make a set out of the scc
                set<int> sccSet;

                while (!scc.empty())
                {
                    sccSet.insert(scc.top());
                    scc.pop();
                }

                vector<int> sccList;
                for (int i : sccSet)
                {
                    sccList.push_back(i);
                }

                sccs.push_back(sccList);
            }
        }
        return sccs;
    }

    // RUN AN IMPUT AND RUN YOUR DATABASE TOSTRING AND MAKE SURE WHAT GOT PRINTED OUT MATCHES WHAT YOU WERE EXPECTING.
};