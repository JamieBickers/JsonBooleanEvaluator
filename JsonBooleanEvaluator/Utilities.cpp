#include "stdafx.h"
#include "Utilities.h"

#include <string>
#include <regex>

int indexOfClosingBracket(std::string expression, int position)
{
	if (expression[position] != '(') {
		throw "Not given brackets.";
	}
	int bracketBalance = 1;
	int currentPosition = position + 1;
	while (bracketBalance != 0) {
		if (expression[currentPosition] == '(') {
			bracketBalance++;
		}
		else if (expression[currentPosition] == ')') {
			bracketBalance--;
		}
		currentPosition++;
	}
	return currentPosition - 1;
}

std::string getTermsBetweenBracketsIncludingBrackets(std::string expression, int position)
{
	if (expression[position] != '(') {
		throw "Not given brackets.";
	}
	int closingIndex = indexOfClosingBracket(expression, position);
	return expression.substr(position, closingIndex + 1 - position);
}

bool stringContainsCharacter(std::string expression, char character, int startingPosition)
{
	return expression.find(character, startingPosition) != std::string::npos;
}

int indexOfFirstChar(std::string expression, char character, int position)
{
	for (size_t i = 0; i < expression.size(); i++)
	{
		if (expression[i] == character) {
			return i;
		}
	}
	throw "Character is not in string.";
}

bool isNumeric(std::string possibleNumber)
{
	if (possibleNumber == "") {
		return false;
	}
	else {
		std::regex numericalRegex("-?[0-9]*\\.?[0-9]*");
		return regex_match(possibleNumber, numericalRegex);
	}
}
int indexOfNextOpeningBracket(std::string expression, int position)
{
	for (size_t i = position; i < expression.size(); i++)
	{
		if (expression[i] == '(') {
			return i;
		}
	}
	throw "No opening bracket.";
}
