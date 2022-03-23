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

    interpreter.evalSchemes();
    interpreter.evalFacts();
    interpreter.evalRules();
    interpreter.evalQueries();

    return 0;
}