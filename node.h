#pragma once
#include <string>
#include <sstream>
#include <set>
using namespace std;

class Node
{
private:
public:
    set<unsigned int> adjacentNodeIDs;
    unsigned int nodeID;
    bool visited = false;
    // default node constructor
    Node() : nodeID(0) {}
    // generate a constructor that takes in a nodeID
    Node(unsigned int nodeID) : nodeID(nodeID) {}

    // function that sets the nodeID
    void setNodeID(unsigned int nodeID)
    {
        this->nodeID = nodeID;
    }

    // function that gets the nodeID
    unsigned int getNodeID()
    {
        return nodeID;
    }

    void addEdge(unsigned int nodeID)
    {
        adjacentNodeIDs.insert(nodeID);
    }

    // get visited
    bool getVisited()
    {
        return visited;
    }

    // set visited
    void setVisited(bool visited)
    {
        this->visited = visited;
    }

    bool contains(unsigned int nodeID)
    {
        return (adjacentNodeIDs.find(nodeID) != adjacentNodeIDs.end());
    }

    // getter and setter for adjacentNodeIDs
    set<unsigned int> getAdjacentNodeIDs()
    {
        return adjacentNodeIDs;
    }

    // function that removes an edge from a graph
    void removeEdge(unsigned int nodeID)
    {
        adjacentNodeIDs.erase(nodeID);
    }

    void setAdjacentNodeIDs(set<unsigned int> adjacentNodeIDs)
    {
        this->adjacentNodeIDs = adjacentNodeIDs;
    }

    string toString()
    {
        stringstream out;
        string sep = "";
        for (auto nodeID : adjacentNodeIDs)
        {
            out << sep << "R" << nodeID;
            sep = ",";
        }

        return out.str();
    }
};