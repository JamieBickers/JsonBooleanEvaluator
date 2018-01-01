#include "stdafx.h"
#include "BooleanTree.h"
#include "Utilities.h"
#include "json.h"
#include "EvaluateArithmeticExpressions.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <regex>

using namespace std;
using json = nlohmann::json;

string getNextBooleanExpression(string expression, int position);
string getNextBooleanOperator(string expression, int position);
char parseBooleanOperator(string booleanOperator);
void setCondition(shared_ptr<BooleanTree> tree, string condition);
string getFullOperator(string expression, int startingPosition);
int findIndexOfComparisonOperator(string condition, int position);

shared_ptr<BooleanTree> parseBooleansExpressionToTree(string expression)
{
	// remove whitespace
	expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

	auto tree = shared_ptr<BooleanTree>(new BooleanTree());

	if (expression[0] == '!') {
		tree->setBooleanOperator('!');
		tree->setRightChild(parseBooleansExpressionToTree(expression.substr(1)));
		return tree;
	}

	string firstBooleanExpression = getNextBooleanExpression(expression, 0);

	if (expression.size() == firstBooleanExpression.size()) {
		if ((expression[0] == '(') && (expression[expression.size() - 1] == ')')) {;
			return parseBooleansExpressionToTree(expression.substr(1, expression.size() - 2));
		}
		setCondition(tree, expression);
		return tree;
	}
	string firstBooleanOperator = getNextBooleanOperator(expression, firstBooleanExpression.size());
	char parsedOperator = parseBooleanOperator(firstBooleanOperator);

	tree->setBooleanOperator(parsedOperator);
	
	tree->setLeftChild(parseBooleansExpressionToTree(firstBooleanExpression));
	
	string restOfExpression = expression.substr(firstBooleanExpression.size() + firstBooleanOperator.size());
	tree->setRightChild(parseBooleansExpressionToTree(restOfExpression));
	return tree;
}

void setCondition(shared_ptr<BooleanTree> tree, string condition)
{
	// remove whitespace
	condition.erase(std::remove(condition.begin(), condition.end(), ' '), condition.end());

	regex arithmeticComparisonCharacters("<|>|!|=");
	if (!regex_search(condition, arithmeticComparisonCharacters)) {
		tree->setBooleanCondition(condition);
		return;
	}

	auto indexOfComparisionOperator = findIndexOfComparisonOperator(condition, 0);
	auto comparisonOperator = getFullOperator(condition, indexOfComparisionOperator);
	auto left = condition.substr(0, indexOfComparisionOperator);
	auto right = condition.substr(indexOfComparisionOperator + comparisonOperator.size());
	auto arithmeticCondition = shared_ptr<ArithmeticCondition>(new ArithmeticCondition());
	arithmeticCondition->comparison = comparisonOperator;
	arithmeticCondition->left = parseArithmeticExpressionToTree(left);
	arithmeticCondition->right = parseArithmeticExpressionToTree(right);
	tree->setArithmeticCondition(arithmeticCondition);
}

int findIndexOfComparisonOperator(string condition, int position)
{
	regex arrayMethodRegex("\\]\\.");
	smatch matches;
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

string getFullOperator(string expression, int startingPosition)
{
	auto operatorCharacters = "<>!=";
	if (!(stringContainsCharacter(operatorCharacters, expression[startingPosition]))) {
		throw "Not given a comparison operator.";
	}
	else if (expression.size() == startingPosition) {
		return string(1, expression[startingPosition]);
	}
	else if (stringContainsCharacter(operatorCharacters, expression[startingPosition + 1])) {
		return expression.substr(startingPosition, 2);
	}
	else {
		return string(1, expression[startingPosition]);
	}
}

string getNextBooleanExpression(string expression, int position)
{
	if (expression[position] == '(') {
		int closingBracketPosition = indexOfClosingBracket(expression, position);
		auto betweenBrackets = expression.substr(position, closingBracketPosition - position + 1);
		regex booleanRegex("&|\\||![^=]");
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

string getNextBooleanOperator(string expression, int position)
{
	const string booleanCharacters = "&|!";
	if (!stringContainsCharacter(booleanCharacters, expression[position])) {
		throw "Not given a boolean character.";
	}
	
	string nextOperator;
	unsigned currentPosition = position;
	while ((expression[currentPosition] == expression[position]) && (currentPosition < expression.size()))
	{
		nextOperator.push_back(expression[currentPosition]);
		currentPosition++;
	}
	return nextOperator;
}

char parseBooleanOperator(string booleanOperator)
{
	return booleanOperator[0];
}



void runBooleanExpressionParserTests() {

	cout << "Running BooleanExpressionParser tests." << endl;

	json variables;

	string booleanExpression1 = "4y4y4yui&&(ewrejwid||false)";
	string result1 = getNextBooleanExpression(booleanExpression1, 0);
	cout << (result1 == "4y4y4yui") << endl;

	booleanExpression1 = "4y4y4yui&&(ewrejwid||false)";
	result1 = getNextBooleanExpression(booleanExpression1, 10);
	cout << (result1 == "(ewrejwid||false)") << endl;

	string booleanExpression2 = "true";
	auto tree2 = parseBooleansExpressionToTree(booleanExpression2);
	bool result2 = tree2->evaluateNode(variables);
	cout << (result2 == true) << endl;

	booleanExpression2 = "false";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == false) << endl;

	booleanExpression2 = "(false)";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == false) << endl;

	booleanExpression2 = "!true";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == false) << endl;

	booleanExpression2 = "!(false)";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == true) << endl;

	booleanExpression2 = "true || false";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == true) << endl;

	booleanExpression2 = "true & false";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == false) << endl;

	booleanExpression2 = "(true || false) && !(true)";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == false) << endl;

	booleanExpression2 = "(true || false) && !true";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == false) << endl;

	booleanExpression2 = "((true) || (true || (false))) && (!(((true) || (false)) && (true)))";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode(variables);
	cout << (result2 == false) << endl;
}