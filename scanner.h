#pragma once

#include "token.h"
#include <map>

bool fileReadCompleted = false;
unsigned int tokenCount = 0;

class Scanner
{
private:
	string input;
	unsigned int lineNum = 1;

public:
	Scanner(const string &input) : input(input) {}
	Token scanToken()
	{
		string value = "";
		TokenType type = TokenType::UNDEFINED;
		int size = 0;

		if (input.size() <= 1)
		{
			fileReadCompleted = true;
			tokenCount++;
			return Token("", TokenType::END_OF_FILE, lineNum);
		}
		
		while (isspace(input.at(0)))
		{

			if (input.at(0) == '\n')
			{
				lineNum++;
			}

			if (input.size() <= 1)
			{
				fileReadCompleted = true;
				tokenCount++;
				return Token("", TokenType::END_OF_FILE, lineNum);
			}
			//cout << input.size() << endl;
			if (input.size() <= 1)
			{
				input = input.substr(1, input.size());
				//\t \n \r \s
			}
			input = input.substr(1, input.size() - 1);
		}

		if (input.size() <= 1)
		{
			fileReadCompleted = true;
			tokenCount++;
			return Token("", TokenType::END_OF_FILE, lineNum);
		}

		if (input.at(0) == ':' && input.at(1) == '-')
		{
			size = 2;
			type = TokenType::COLON_DASH;
		}

		else if ((input.at(0) == 'S' && input.at(1) == 'c' && input.at(2) == 'h' && input.at(3) == 'e' && input.at(4) == 'm' && input.at(5) == 'e' && input.at(6) == 's') && ((input.at(7) == '\n') || (input.at(7) == ':')))
		{
			size = 7;
			type = TokenType::SCHEMES;
		}

		else if (input.at(0) == 'F' && input.at(1) == 'a' && input.at(2) == 'c' && input.at(3) == 't' && input.at(4) == 's' && ((input.at(5) == '\n') || (input.at(5) == ':')))
		{
			size = 5;
			type = TokenType::FACTS;
		}

		else if (input.at(0) == 'R' && input.at(1) == 'u' && input.at(2) == 'l' && input.at(3) == 'e' && input.at(4) == 's' && ((input.at(5) == '\n') || (input.at(5) == ':')))
		{
			size = 5;
			type = TokenType::RULES;
		}

		else if (input.at(0) == 'Q' && input.at(1) == 'u' && input.at(2) == 'e' && input.at(3) == 'r' && input.at(4) == 'i' && input.at(5) == 'e' && input.at(6) == 's' && ((input.at(7) == '\n') || (input.at(7) == ':')))
		{
			size = 7;
			type = TokenType::QUERIES;
		}

		else if (isalpha(input.at(0)))
		{
			int count = 0;
			while (isalnum(input.at(count)))
			{
				count++;
			}
			value = input.substr(0, count);
			input = input.substr(count, input.size() - count);
			type = TokenType::ID;
			tokenCount++;
			return Token(value, type, lineNum);
		}

		else if (input.at(0) == '#')
		{
			int count = 0;
			while (input.at(count) != '\n')
			{
				count++;
			}
			value = input.substr(0, count);
			input = input.substr(count, input.size() - count);
			type = TokenType::COMMENT;
			tokenCount++;
			return Token(value, type, lineNum);
		}

		else if (input.at(0) == '\'')
		{
			int count = 1;
			int previousLineNum = lineNum;
			while (input.at(count) != '\'')
			{
				if (input.at(count) == '\n')
				{
					lineNum++;

				}
				count++;
				if (input.length() == (unsigned int)count)
				{
					value = input.substr(0, count);
					input = "";
					tokenCount++;
					return Token(value, TokenType::UNDEFINED, previousLineNum);
				}
			}
			count++;
			value = input.substr(0, count);
			input = input.substr(count, input.size() - count);
			type = TokenType::STRING;
			tokenCount++;
			return Token(value, type, lineNum);
		}

		else if (input.at(0) != '\\')
		{
			switch (input.at(0))
			{
			case ',':
				size = 1;
				type = TokenType::COMMA;
				break;
			case '.':
				size = 1;
				type = TokenType::PERIOD;
				break;
			case '?':
				size = 1;
				type = TokenType::Q_MARK;
				break;
			case '(':
				size = 1;
				type = TokenType::LEFT_PAREN;
				break;
			case ')':
				size = 1;
				type = TokenType::RIGHT_PAREN;
				break;
			case ':':
				size = 1;
				type = TokenType::COLON;
				break;
			case '*':
				size = 1;
				type = TokenType::MULTIPLY;
				break;
			case '+':
				size = 1;
				type = TokenType::ADD;
				break;
			default:
				size = 1;
				type = TokenType::UNDEFINED;
				value = input.at(0);
			}
		}

		value = input.substr(0, size);
		input = input.substr(size, input.size() - 1);
		tokenCount++;
		return Token(value, type, lineNum);
	}
};

