#include "stdafx.h"
#include "Utilities.h"
#include "json.h"
#include "VariableEvaluator.h"
#include "ArithmeticTree.h"
#include "ArrayMethod.h"
#include "parseArrayMethod.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include <regex>

using json = nlohmann::json;

bool containsArithmeticCharacter(std::string expression);
std::shared_ptr<ArithmeticTree> handleNotPlusAtStart(std::shared_ptr<ArithmeticTree> startingNode,
	std::string expression, char arithmeticOperation);
std::string firstArithmeticExpressionForMultiplication(std::string expression);
std::string firstArithmeticExpression(std::string expression);
std::shared_ptr<ArithmeticTree> parseArithmeticExpressionToTreeWithOperationAtStart(
	std::shared_ptr<ArithmeticTree> startingNode, std::string expression);

std::shared_ptr<ArithmeticTree> parseArithmeticExpressionToTree(std::string expression)
{
	// remove whitespace
	expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

	if (expression == "") {
		auto zeroNode = ArithmeticTree();
		zeroNode.setValue(0);
		auto zeroNodePtr = std::make_shared<ArithmeticTree>(zeroNode);
		return zeroNodePtr;
	}
	auto firstExpression = firstArithmeticExpression(expression);
	auto firstNode = std::make_shared<ArithmeticTree>(ArithmeticTree());
	std::regex arrayMethodRegex("\\]\\.");
	if (firstExpression[0] == '(') {
		firstNode = parseArithmeticExpressionToTree(firstExpression.substr(1, firstExpression.size() - 2));
	}
	else if (isNumeric(firstExpression)) {
		firstNode->setValue(std::stod(firstExpression));
	}
	else if (containsArithmeticCharacter(firstExpression)) {
		firstNode = parseArithmeticExpressionToTree(firstExpression);
	}
	else if (std::regex_search(firstExpression, arrayMethodRegex)) {
		auto arrayMethod = parseArrayMethod<double>(expression);
		firstNode->setArrayMethod(arrayMethod);
	}
	else {
		firstNode->setVariable(firstExpression);
	}
	return parseArithmeticExpressionToTreeWithOperationAtStart(firstNode, expression.substr(firstExpression.size()));
}

std::shared_ptr<ArithmeticTree> parseArithmeticExpressionToTreeWithOperationAtStart(
	std::shared_ptr<ArithmeticTree> startingNode, std::string expression)
{
	if (expression == "") {
		return startingNode;
	}
	else if (expression[0] == '+') {
		auto initialNode = std::make_shared<ArithmeticTree>(ArithmeticTree());
		initialNode->setLeftChild(startingNode);
		initialNode->setRightChild(parseArithmeticExpressionToTree(expression.substr(1)));
		initialNode->setArithmeticOperation('+');
		return initialNode;
	}
	else if (expression[0] == '-') {
		return handleNotPlusAtStart(startingNode, expression, '-');
	}
	else if (expression[0] == '*') {
		return handleNotPlusAtStart(startingNode, expression, '*');
	}
	else if (expression[0] == '/') {
		return handleNotPlusAtStart(startingNode, expression, '/');
	}
	else {
		throw "Not a valid expression.";
	}
}

std::string firstArithmeticExpression(std::string expression)
{
	if (expression[0] == '(') {
		return getTermsBetweenBracketsIncludingBrackets(expression, 0);
	}
	else if (expression[0] == '-') {
		return '-' + firstArithmeticExpression(expression.substr(1));
	}
	else {
		int indexOfFirstOperation = expression.size();
		for (size_t i = 0; i < expression.size(); i++)
		{
			if (stringContainsCharacter("+-*/", expression[i])) {
				indexOfFirstOperation = i;
				break;
			}
		}
		return expression.substr(0, indexOfFirstOperation);
	}
}

std::string firstArithmeticExpressionForMultiplication(std::string expression)
{
	for (size_t i = 0; i < expression.size(); i++)
	{
		if (expression[i] == '(') {
			i = indexOfClosingBracket(expression, i);
		}
		else if ((expression[i] == '+') || (expression[i] == '-')) {
			return expression.substr(0, i);
		}
	}
	return expression;
}

std::shared_ptr<ArithmeticTree> handleNotPlusAtStart(std::shared_ptr<ArithmeticTree> startingNode,
	std::string expression, char arithmeticOperation)
{
	auto rightExpression = firstArithmeticExpressionForMultiplication(expression.substr(1));
	auto initialNode = std::make_shared<ArithmeticTree>(ArithmeticTree());
	initialNode->setLeftChild(startingNode);
	initialNode->setRightChild(parseArithmeticExpressionToTree(rightExpression));
	initialNode->setArithmeticOperation(arithmeticOperation);
	return parseArithmeticExpressionToTreeWithOperationAtStart(initialNode,
		expression.substr(1 + rightExpression.size()));
}

bool containsArithmeticCharacter(std::string expression)
{
	std::regex arithmeticCharacterRegex("\\+|-|\\*|\\/");
	return regex_search(expression, arithmeticCharacterRegex);
}