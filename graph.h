#pragma once
#include "node.h"
#include <string>
#include <sstream>
#include <map>
#include <stack>

class Graph
{
private:
    map<unsigned int, Node> nodes;
    // vector that keeps track of which nodes have been marked or visited:
    stack<unsigned int> workingStack;

public:
    // default graph constructor
    Graph()
    {
    }
    // function that gets nodes
    Node &getNode(unsigned int nodeID)
    {
        return nodes.at(nodeID);
    }

    // function that removes an edge from a graph
    void removeEdge(unsigned int fromNode, unsigned int toNode)
    {
        nodes.at(fromNode).removeEdge(toNode);
    }

    // function that gets all the nodes in a graph
    vector<Node> getNodes()
    {
        vector<Node> nodes;
        for (auto node : this->nodes)
        {
            nodes.push_back(node.second);
        }
        return nodes;
    }

    // function that returns whether or not a node has been visited
    bool isVisited(unsigned int nodeID)
    {
        return getNode(nodeID).getVisited();
    }

    Graph(unsigned int size)
    {
        for (unsigned int i = 0; i < size; i++)
        {
            nodes.insert({i, Node(i)});
        }
    }

    void addEdge(unsigned int fromNode, unsigned int toNode)
    {
        nodes.at(fromNode).addEdge(toNode);
    }

    string toString()
    {
        stringstream out;
        string sep = "";
        for (auto node : nodes)
        {
            out << sep << "R" << node.first << ":" << node.second.toString();
            sep = "\n";
        }

        return out.str();
    }

    // stack<unsigned int> depthFirstSearch(unsigned int startNode)
    // {
    //     // check if startNode has been visited or not
    //     if (marked[startNode])
    //     {
    //         return stack<unsigned int>();
    //     }
    //     // make startNode visited
    //     marked[startNode] = true;
    //     // for each child in nodes.at(startNode)
    //     for (auto child : nodes.at(startNode).getAdjacentNodeIDs())
    //     {
    //         // if the child has not yet been visited, call depthFirstSearch(child)
    //         if (!marked[child])
    //         {
    //             stack<unsigned int> temp = depthFirstSearch(child);
    //             while (!temp.empty())
    //             {
    //                 workingStack.push(temp.top());
    //                 temp.pop();
    //             }
    //         }
    //     }
    //     // return the working stack
    //     return workingStack;
    // }

    // stack<unsigned int> depthFirstSearchForest()
    // {
    //     for (unsigned int i = 0; i < marked.size(); i++)
    //     {
    //         // if the current node i has not been marked call depthFirstSearch(i);
    //         if (!marked[i])
    //         {
    //             stack<unsigned int> temp = depthFirstSearch(i);
    //             while (!temp.empty())
    //             {
    //                 workingStack.push(temp.top());
    //                 temp.pop();
    //             }
    //         }
    //     }
    //     return workingStack;
    // }

    // set<unsigned int> depthFirstSearchSCC(unsigned int startNode)
    // {
    //     set<unsigned int> result;
    //     // check if startNode has been visited or not
    //     if (marked[startNode])
    //     {
    //         return set<unsigned int>();
    //     }
    //     // make startNode visited
    //     marked[startNode] = true;

    //     // for each child in nodes.at(startNode) if the child has not yet been visited, call depthFirstSearch(child)
    //     for (auto child : nodes.at(startNode).getAdjacentNodeIDs())
    //     {
    //         if (!marked[child])
    //         {
    //             set<unsigned int> temp = depthFirstSearchSCC(child);
    //             result.insert(temp.begin(), temp.end());
    //         }
    //     }
    //     // add the start node to the working stack

    //     return result;
    // }

    // vector<set<unsigned int>> depthFirstSearchForestSCC(stack<unsigned int> priorQueue)
    // {
    //     vector<set<unsigned int>> SCC;
    //     // while priorQueue is not empty
    //     unsigned int i = priorQueue.top();
    //     priorQueue.pop();
    //     while (!priorQueue.empty())
    //     {
    //         // if the current node i has not been marked call depthFirstSearch(i);
    //         if (!marked[i])
    //         {
    //             set<unsigned int> temp = depthFirstSearchSCC(priorQueue.top());
    //             SCC.push_back(temp);
    //         }
    //     }
    //     return SCC;
    // }

    // a toString function for a set
    string setToString(set<unsigned int> set)
    {
        stringstream out;
        string sep = "";
        for (auto node : set)
        {
            out << sep << "R" << node;
            sep = ",";
        }

        return out.str();
    }
};