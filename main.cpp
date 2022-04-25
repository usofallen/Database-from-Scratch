#include <iostream>
#include <istream>
#include <fstream>
#include <vector>
#include <sstream>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "predicate.h"
#include "parameter.h"
#include "datalogProgram.h"
#include "Scheme.h"
#include "Tuple.h"
#include "Relation.h"
#include "Database.h"
#include "Interpreter.h"
#include "node.h"
#include "graph.h"

vector<string> flatten(vector<Parameter> vectorOfParameters)
{
    vector<string> flattenedParameters;
    for (unsigned int i = 0; i < vectorOfParameters.size(); i++)
    {
        flattenedParameters.push_back(vectorOfParameters.at(i).getValue());
    }
    return flattenedParameters;
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        return 1;
    }

    string inputFileName = argv[1];
    ifstream input(inputFileName);
    int realTokenCount = 0;
    vector<Token> tokens;
    if (!input)
    {
        throw runtime_error("Could not open file");
    }
    stringstream buffer;
    buffer << input.rdbuf();
    ofstream outfile;
    string inputString = buffer.str();

    Scanner scanner(inputString);

    while (fileReadCompleted == false)
    {

        realTokenCount++;

        Token currToken = scanner.scanToken();
        if (currToken.getType() != TokenType::COMMENT)
        {
            tokens.push_back(currToken);
        }
    }

    Parser parser(tokens);

    parser.parse();
    datalogProgram dp = parser.getDatalogProgram();
    Database database;

    Interpreter interpreter(dp);

    pair<Graph, Graph> graphs = interpreter.makeGraph(dp.getRules());
    cout << "Dependency Graph" << endl;
    cout << graphs.first.toString() << endl
         << endl;

    // cout << graphs.second.toString() << endl;
    // cout << endl;

    // vector<int> topologicalSort = interpreter.getTopologicalSort(graphs.second);

    // // for loop that prints out the topological sort
    // cout << "Topological Sort" << endl;
    // for (unsigned int i = 0; i < topologicalSort.size(); i++)
    // {
    //     cout << topologicalSort.at(i) << endl;
    // }
    interpreter.evalSchemes();
    interpreter.evalFacts();

    vector<vector<int>> SCCs = interpreter.getSCCs(graphs.first, graphs.second);
    vector<Rule> rules = dp.getRules();
    cout << "Rule Evaluation" << endl;
    bool differentSizes = false;

    vector<int> theSCC;
    for (unsigned int i = 0; i < SCCs.size(); i++)
    {
        theSCC = SCCs.at(i);
        cout << "SCC: " << interpreter.sccToString(theSCC) << endl;

        int numPasses = 0;

        // bool addedTuples = false;
        vector<Rule> vectorOfRules;

        for (int i : theSCC)
        {
            vectorOfRules.push_back(rules.at(i));
        }

        do
        {
            numPasses++;
            differentSizes = interpreter.evalRuleListOnce(vectorOfRules);

        } while (differentSizes && (theSCC.size() > 1 || interpreter.ruleDependsOnSelf(graphs.first, theSCC.front())));
        cout << numPasses << " passes: " << interpreter.sccToString(theSCC) << endl;
    }

    interpreter.evalQueries();

    return 0;
}
