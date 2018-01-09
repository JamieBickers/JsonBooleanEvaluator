#include "stdafx.h"
#include "BooleanTree.h"
#include "Utilities.h"
#include "json.h"
#include "EvaluateArithmeticExpressions.h"
#include "parseArrayMethod.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <regex>

using json = nlohmann::json;

std::string getNextBooleanExpression(std::string expression, int position);
std::string getNextBooleanOperator(std::string expression, int position);
char parseBooleanOperator(std::string booleanOperator);
void setCondition(std::shared_ptr<BooleanTree> tree, std::string condition);
std::string getFullOperator(std::string expression, int startingPosition);
int findIndexOfComparisonOperator(std::string condition, int position);
std::string getConditionType(std::string condition);

std::shared_ptr<BooleanTree> parseBooleansExpressionToTree(std::string expression)
{
	// remove whitespace
	expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

	auto tree = std::make_shared<BooleanTree>();

	if (expression[0] == '!') {
		tree->setBooleanOperator('!');
		tree->setRightChild(parseBooleansExpressionToTree(expression.substr(1)));
		return tree;
	}

	std::string firstBooleanExpression = getNextBooleanExpression(expression, 0);

	if (expression.size() == firstBooleanExpression.size()) {
		if ((expression[0] == '(') && (expression[expression.size() - 1] == ')')) {
			;
			return parseBooleansExpressionToTree(expression.substr(1, expression.size() - 2));
		}
		setCondition(tree, expression);
		return tree;
	}
	std::string firstBooleanOperator = getNextBooleanOperator(expression, firstBooleanExpression.size());
	char parsedOperator = parseBooleanOperator(firstBooleanOperator);

	tree->setBooleanOperator(parsedOperator);

	tree->setLeftChild(parseBooleansExpressionToTree(firstBooleanExpression));

	std::string restOfExpression = expression.substr(firstBooleanExpression.size() + firstBooleanOperator.size());
	tree->setRightChild(parseBooleansExpressionToTree(restOfExpression));
	return tree;
}

void setCondition(std::shared_ptr<BooleanTree> tree, std::string condition)
{
	auto conditionType = getConditionType(condition);

	if (conditionType == "arithmetic") {
		auto indexOfComparisionOperator = findIndexOfComparisonOperator(condition, 0);
		auto comparisonOperator = getFullOperator(condition, indexOfComparisionOperator);
		auto left = condition.substr(0, indexOfComparisionOperator);
		auto right = condition.substr(indexOfComparisionOperator + comparisonOperator.size());
		auto arithmeticCondition = std::make_shared<ArithmeticCondition>();
		arithmeticCondition->comparison = comparisonOperator;
		arithmeticCondition->left = parseArithmeticExpressionToTree(left);
		arithmeticCondition->right = parseArithmeticExpressionToTree(right);
		tree->setArithmeticCondition(arithmeticCondition);
	}
	else if (conditionType == "array") {
		auto arrayMethod = parseArrayMethod<bool>(condition);
		tree->setArrayMethod(arrayMethod);
	}
	else if (conditionType == "boolean") {
		tree->setBooleanCondition(condition);
	}
	else {
		throw "Invalid condition type.";
	}

	std::regex arithmeticComparisonCharacters("<|>|!|=");
	std::regex arrayMethodRegex("\\]\\.");
}

std::string getConditionType(std::string condition)
{
	unsigned i = 0;
	while (i < condition.size())
	{
		if (condition[i] == '(') {
			i = indexOfClosingBracket(condition, i);
		}
		else if (stringContainsCharacter("<|>|!|=", condition[i])) {
			return "arithmetic";
		}
		else {
			i++;
		}
	}
	std::regex arrayMethodRegex("\\]\\.");
	if (regex_search(condition, arrayMethodRegex)) {
		return "array";
	}
	else {
		return "boolean";
	}
}

int findIndexOfComparisonOperator(std::string condition, int position)
{
	std::regex arrayMethodRegex("\\]\\.");
	std::smatch matches;
	auto restOfCondition = condition.substr(position);
	regex_search(restOfCondition, matches, arrayMethodRegex);
	if (matches.size() > 0) {
		auto indexOfTheOpeningBracket = indexOfNextOpeningBracket(condition, matches.position(0) + position);
		auto indexOfTheClosingBracket = indexOfClosingBracket(condition, indexOfTheOpeningBracket);
		return findIndexOfComparisonOperator(condition, indexOfTheClosingBracket);
	}
	int first = condition.find_first_of("<>!=", position);
	if ((condition[first] == '=') && condition[first + 1] == '>') {
		return findIndexOfComparisonOperator(condition, first + 2);
	}
	return first;
}

std::string getFullOperator(std::string expression, int startingPosition)
{
	auto operatorCharacters = "<>!=";
	if (!(stringContainsCharacter(operatorCharacters, expression[startingPosition]))) {
		throw "Not given a comparison operator.";
	}
	else if (expression.size() == startingPosition) {
		return std::string(1, expression[startingPosition]);
	}
	else if (stringContainsCharacter(operatorCharacters, expression[startingPosition + 1])) {
		return expression.substr(startingPosition, 2);
	}
	else {
		return std::string(1, expression[startingPosition]);
	}
}

std::string getNextBooleanExpression(std::string expression, int position)
{
	if (expression[position] == '(') {
		int closingBracketPosition = indexOfClosingBracket(expression, position);
		auto betweenBrackets = expression.substr(position, closingBracketPosition - position + 1);
		std::regex booleanRegex("&|\\||![^=]");
		if (regex_match(betweenBrackets, booleanRegex)) {
			return betweenBrackets;
		}
		else {
			return betweenBrackets + getNextBooleanExpression(expression, position + betweenBrackets.size());
		}
	}
	else {
		// find first boolean character
		unsigned currentPosition = position;
		while (currentPosition < expression.size())
		{
			if (expression[currentPosition] == '(') {
				currentPosition = indexOfClosingBracket(expression, currentPosition);
			}
			else if (stringContainsCharacter("&|", expression[currentPosition])) {
				break;
			}
			// do ! seperately as != is an arithmetic condition
			else if (stringContainsCharacter("!", expression[currentPosition])) {
				if (!((currentPosition < expression.size() - 1) && (expression[currentPosition + 1] == '!'))) {
					break;
				}
			}
			currentPosition++;
		}
		return expression.substr(position, currentPosition - position);
	}
}

std::string getNextBooleanOperator(std::string expression, int position)
{
	const std::string booleanCharacters = "&|!";
	if (!stringContainsCharacter(booleanCharacters, expression[position])) {
		throw "Not given a boolean character.";
	}

	std::string nextOperator;
	unsigned currentPosition = position;
	while ((expression[currentPosition] == expression[position]) && (currentPosition < expression.size()))
	{
		nextOperator.push_back(expression[currentPosition]);
		currentPosition++;
	}
	return nextOperator;
}

char parseBooleanOperator(std::string booleanOperator)
{
	return booleanOperator[0];
}