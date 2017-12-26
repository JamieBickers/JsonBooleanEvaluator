#include "stdafx.h"
#include "BooleanTree.h"
#include "Utilities.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <regex>

using namespace std;

string getNextBooleanExpression(string expression, int position);
string getNextBooleanOperator(string expression, int position);
char parseBooleanOperator(string booleanOperator);

shared_ptr<BooleanTree> parseBooleansExpressionToTree(string expression)
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
		if ((expression[0] == '(') && (expression[expression.size() - 1] == ')')) {;
			return parseBooleansExpressionToTree(expression.substr(1, expression.size() - 2));
		}
		tree->condition = expression;
		return tree;
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
		int closingBracketPosition = indexOfClosingBracket(expression, position);
		auto betweenBrackets = expression.substr(position, closingBracketPosition - position + 1);
		regex booleanRegex("&|\||![^=]");
		if (regex_match(betweenBrackets, booleanRegex)) {
			return betweenBrackets;
		}
		else {
			return betweenBrackets + getNextBooleanExpression(expression, position + betweenBrackets.size());
		}
	}
	else {
		// find first boolean character
		int currentPosition = position;
		while (currentPosition < expression.size())
		{
			if (stringContainsCharacter("&|", expression[currentPosition])) {
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



void runBooleanExpressionParserTests() {

	cout << "Running BooleanExpressionParser tests." << endl;

	auto variables = map<string, double>();

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