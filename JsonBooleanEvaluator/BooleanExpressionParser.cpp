#include "stdafx.h"
#include "BooleanTree.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>

using namespace std;

string getNextBooleanExpression(string expression, int position);
string getNextBooleanOperator(string expression, int position);
char parseBooleanOperator(string booleanOperator);
bool stringContainsCharacter2(string expression, char character, int startingPosition = 0);
string getTermsBetweenBracketsIncludingBrackets2(string expression, int position);
int indexOfFirstChar2(string expression, char character, int position);
int indexOfClosingBracket2(string expression, int position);
unique_ptr<BooleanTree> parseBooleansExpressionToTree(string expression);

unique_ptr<BooleanTree> parseBooleansExpressionToTree(string expression)
{
	// remove whitespace
	expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

	auto tree = unique_ptr<BooleanTree>(new BooleanTree());

	if (expression[0] == '!') {
		tree->booleanOperator = '!';
		tree->rightChild = parseBooleansExpressionToTree(expression.substr(1));
		return tree;
	}

	string firstBooleanExpression = getNextBooleanExpression(expression, 0);

	if (expression.size() == firstBooleanExpression.size()) {
		if (expression[0] == '(') {
			return parseBooleansExpressionToTree(expression.substr(1, expression.size() - 2));
		}
		else {
			tree->condition = expression;
			return tree;
		}
	}
	string firstBooleanOperator = getNextBooleanOperator(expression, firstBooleanExpression.size());
	char parsedOperator = parseBooleanOperator(firstBooleanOperator);

	tree->booleanOperator = parsedOperator;
	
	tree->leftChild = parseBooleansExpressionToTree(firstBooleanExpression);
	
	string restOfExpression = expression.substr(firstBooleanExpression.size() + firstBooleanOperator.size());
	tree->rightChild = parseBooleansExpressionToTree(restOfExpression);
	return tree;
}

string getNextBooleanExpression(string expression, int position)
{
	if (expression[position] == '(') {
		int closingBracketPosition = indexOfClosingBracket2(expression, position);
		return expression.substr(position, closingBracketPosition - position + 1);
	}
	else {
		// find first boolean character
		int currentPosition = position;
		while (currentPosition < expression.size())
		{
			if (stringContainsCharacter2("&|", expression[currentPosition])) {
				break;
			}
			// do ! seperately as != is an arithmetic condition
			else if (stringContainsCharacter2("!", expression[currentPosition])) {
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
	if (!stringContainsCharacter2(booleanCharacters, expression[position])) {
		throw "Not given a boolean character.";
	}
	
	string nextOperator;
	int currentPosition = position;
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

int indexOfClosingBracket2(string expression, int position) {
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

string getTermsBetweenBracketsIncludingBrackets2(string expression, int position)
{
	if (expression[position] != '(') {
		throw "Not given brackets.";
	}
	int closingIndex = indexOfClosingBracket2(expression, position);
	return expression.substr(position, closingIndex + 1);
}

bool stringContainsCharacter2(string expression, char character, int startingPosition)
{
	return expression.find(character, startingPosition) != string::npos;
}

int indexOfFirstChar2(string expression, char character, int position)
{
	for (size_t i = 0; i < expression.size(); i++)
	{
		if (expression[i] == character) {
			return i;
		}
	}
	throw "Character is not in string.";
}

void runBooleanExpressionParserTests() {

	cout << "Running BooleanExpressionParser tests." << endl;

	string booleanExpression1 = "4y4y4yui&&(ewrejwid||false)";
	string result1 = getNextBooleanExpression(booleanExpression1, 0);
	cout << (result1 == "4y4y4yui") << endl;

	booleanExpression1 = "4y4y4yui&&(ewrejwid||false)";
	result1 = getNextBooleanExpression(booleanExpression1, 10);
	cout << (result1 == "(ewrejwid||false)") << endl;

	string booleanExpression2 = "true";
	auto tree2 = parseBooleansExpressionToTree(booleanExpression2);
	bool result2 = tree2->evaluateNode();
	cout << (result2 == true) << endl;

	booleanExpression2 = "false";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == false) << endl;

	booleanExpression2 = "(false)";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == false) << endl;

	booleanExpression2 = "!true";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == false) << endl;

	booleanExpression2 = "!(false)";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == true) << endl;

	booleanExpression2 = "true || false";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == true) << endl;

	booleanExpression2 = "true & false";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == false) << endl;

	booleanExpression2 = "(true || false) && !(true)";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == false) << endl;

	booleanExpression2 = "(true || false) && !true";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == false) << endl;

	booleanExpression2 = "((true) || (true || (false))) && (!(((true) || (false)) && (true)))";
	tree2 = parseBooleansExpressionToTree(booleanExpression2);
	result2 = tree2->evaluateNode();
	cout << (result2 == false) << endl;
}