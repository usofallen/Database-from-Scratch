#pragma once

using namespace std;
#include "token.h"
#include "predicate.h"
#include "parameter.h"
#include "rule.h"
#include "datalogProgram.h"
#include <vector>

class Parser
{
private:
    vector<Token> tokens;
    unsigned int index = 0;
    datalogProgram dp = datalogProgram();

public:
    string match(TokenType type)
    {
        if (tokens.at(index).getType() == type)
        {
            string returnVal = "";
            returnVal = tokens.at(index).getValue();
            index++;
            return returnVal;
        }
        else
        {
            throw tokens.at(index);
        }
        return "";
    }

    Parser(const vector<Token> &tokens) : tokens(tokens)
    {
    }

    void parse()
    {
        vector<Predicate> vectorOfSchemes;
        match(SCHEMES);
        match(COLON);
        vectorOfSchemes.push_back(parseScheme());
        parseSchemeList(vectorOfSchemes);
        dp.setSchemes(vectorOfSchemes);

        vector<Predicate> vectorOfFacts;
        match(FACTS);
        match(COLON);
        parseFactList(vectorOfFacts);
        dp.setFacts(vectorOfFacts);

        vector<Rule> vectorOfRules;
        match(RULES);
        match(COLON);
        parseRuleList(vectorOfRules);
        dp.setRules(vectorOfRules);

        vector<Predicate> vectorOfQueries;
        match(QUERIES);
        match(COLON);
        vectorOfQueries.push_back(parseQuery());
        parseQueryList(vectorOfQueries);
        dp.setQueries(vectorOfQueries);

        match(END_OF_FILE);

        /// sag
    }

    // scheme -> ID LEFT_PAREN ID idList RIGHT_PAREN
    Predicate parseScheme()
    {
        if (tokens.at(index).getType() == TokenType::ID)
        {
            Predicate schemeList(match(ID)); // SNAP
            match(LEFT_PAREN);
            schemeList.addValue(match(ID)); // A
            parseIdList(schemeList);
            match(RIGHT_PAREN);
            return schemeList;
        }
        else
        {
            throw tokens.at(index);
        }
    }
    // schemeList -> scheme schemeList | lambda
    void parseSchemeList(vector<Predicate> &vectorOfCool)
    {
        if (tokens.at(index).getType() == TokenType::ID)
        {
            vectorOfCool.push_back(parseScheme());
            parseSchemeList(vectorOfCool);
        }

        else
        {
            return;
        }
    }

    // fact -> ID LEFT_PAREN STRING stringList RIGHT_PAREN PERIOD
    Predicate parseFact()
    {
        if (tokens.at(index).getType() == TokenType::ID)
        {
            Predicate fact(match(ID));
            match(LEFT_PAREN);
            fact.addValue(match(STRING));
            parseStringList(fact);
            match(RIGHT_PAREN);
            match(PERIOD);
            return fact;
        }
        else
        {
            throw tokens.at(index);
        }
    }

    // Predicate parsePredicate()
    // {
    //     if (tokens.at(index).getType() == TokenType::ID)
    //     {
    //         Predicate bodyPredicate(match(ID));
    //         match(LEFT_PAREN);
    //         bodyPredicate.addValue(match(STRING));
    //         parseStringList(bodyPredicate);
    //         match(RIGHT_PAREN);
    //         return bodyPredicate;
    //     }
    //     else
    //     {
    //         throw tokens.at(index);
    //     }
    // }

    // factList	->	fact factList | lambda
    void parseFactList(vector<Predicate> &vector3)
    {
        if (tokens.at(index).getType() == TokenType::ID)
        {
            vector3.push_back(parseFact());
            parseFactList(vector3);
        }
        else
        {
            return;
        }
    }

    // stringList	-> 	COMMA STRING stringList | lambda
    void parseStringList(Predicate &stringVector)
    {
        if (tokens.at(index).getType() == TokenType::COMMA)
        {
            match(TokenType::COMMA);
            stringVector.addValue(match(TokenType::STRING));
            parseStringList(stringVector);
        }
        else
        {
            return;
        }
    }

    // ruleList	->	rule ruleList | lambda
    void parseRuleList(vector<Rule> &vectorOfRule)
    {
        if (tokens.at(index).getType() == TokenType::ID)
        {
            vectorOfRule.push_back(parseRule());
            parseRuleList(vectorOfRule);
        }
        else
        {
            return;
        }
    }
    // rule -> headPredicate COLON_DASH predicate predicateList PERIOD
    Rule parseRule()
    {
        vector<Predicate> predicateList;
        Predicate headPredicate = parseHeadPredicate();
        Rule rule = Rule();
        match(COLON_DASH);
        predicateList.push_back(parsePredicate());
        parsePredicateList(predicateList);
        match(PERIOD);
        rule.setHeadPredicate(headPredicate);
        rule.setBodyPredicates(predicateList);
        return rule;
    }

    void parsePredicateList(vector<Predicate> &vectorOfSwag)
    {
        if (tokens.at(index).getType() == TokenType::COMMA)
        {
            match(TokenType::COMMA);
            vectorOfSwag.push_back(parsePredicate());
            parsePredicateList(vectorOfSwag);
        }
        else
        {
            return;
        }
    }

    // parameterList->COMMA parameter parameterList | lambda
    void parseParameterList(Predicate &p)
    {
        if (tokens.at(index).getType() == TokenType::COMMA)
        {
            match(TokenType::COMMA);
            p.addValue(parseParameter());
            parseParameterList(p);
        }
        else
        {
            return;
        }
    }

    // idList -> COMMA ID idList | lambda
    void parseIdList(Predicate &p)
    {
        if (tokens.at(index).getType() == TokenType::COMMA)
        {
            match(TokenType::COMMA);
            p.addValue(match(TokenType::ID));
            parseIdList(p);
        }
        else
        {
            return;
        }
    }

    // headPredicate	->	ID LEFT_PAREN ID idList RIGHT_PAREN
    Predicate parseHeadPredicate()
    {
        if (tokens.at(index).getType() == TokenType::ID)
        {
            Predicate head(match(TokenType::ID));
            match(TokenType::LEFT_PAREN);
            head.addValue(match(TokenType::ID));
            parseIdList(head);
            match(TokenType::RIGHT_PAREN);
            return head;
        }

        else
        {
            throw tokens.at(index);
        }
    }

    // predicate	->	ID LEFT_PAREN parameter parameterList RIGHT_PAREN
    Predicate parsePredicate()
    {
        if (tokens.at(index).getType() == TokenType::ID)
        {
            Predicate tail(match(TokenType::ID));
            match(TokenType::LEFT_PAREN);
            tail.addValue(parseParameter());
            parseParameterList(tail);
            match(TokenType::RIGHT_PAREN);

            return tail;
        }
        else
        {
            throw tokens.at(index);
        }
    }

    // parameter	->	STRING | ID
    Parameter parseParameter()
    {
        if (tokens.at(index).getType() == TokenType::STRING || tokens.at(index).getType() == TokenType::ID)
        {
            // string returnVal;
            Parameter returnParameter;
            // returnParameter = tokens.at(index).getType();
            returnParameter.setValue(tokens.at(index).getValue());
            index++;
            return returnParameter;
        }
        else
        {
            throw tokens.at(index);
        }
    }
    // queryList	->	query queryList | lambda
    void parseQueryList(vector<Predicate> &vectorOfQueries)
    {
        if (tokens.at(index).getType() == TokenType::ID)
        {
            vectorOfQueries.push_back(parseQuery());
            parseQueryList(vectorOfQueries);
        }
        else
        {
            return;
        }
    }

    // query -> predicate Q_MARK
    Predicate parseQuery()
    {
        Predicate query(parsePredicate());
        match(Q_MARK);
        return query;
    }

    datalogProgram getDatalogProgram()
    {
        return dp;
    }
};