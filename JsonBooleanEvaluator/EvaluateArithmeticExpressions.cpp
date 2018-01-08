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

void runArithmeticEvaluationTests()
{
	std::cout << "Arithmetic evaluation tests." << std::endl;

	auto expression = "43645tre+3265546yergdf";
	auto result1 = containsArithmeticCharacter(expression);
	std::cout << (result1 == true) << std::endl;

	expression = "43645tre3265546yergdf";
	result1 = containsArithmeticCharacter(expression);
	std::cout << (result1 == false) << std::endl;

	expression = "*43645tre3265546yergdf";
	result1 = containsArithmeticCharacter(expression);
	std::cout << (result1 == true) << std::endl;

	expression = "43645tre3265546yergdf-";
	result1 = containsArithmeticCharacter(expression);
	std::cout << (result1 == true) << std::endl;

	expression = "5";
	auto tree = parseArithmeticExpressionToTree(expression);
	auto variables = json();
	auto result2 = tree->evaluateNode(variables);
	std::cout << (result2 == 5) << std::endl;

	expression = "2+4";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == 6) << std::endl;

	expression = "234+90 + 21 + 16";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == 361) << std::endl;

	expression = "4-2";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == 2) << std::endl;

	expression = "-1";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == -1) << std::endl;

	expression = "-19-22+18+5-10";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == -28) << std::endl;

	expression = "-34534+3454-452-675-23+435-23454+2";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == -34534 + 3454 - 452 - 675 - 23 + 435 - 23454 + 2) << std::endl;

	expression = "4*3";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == 12) << std::endl;

	expression = "(1+2)";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == 3) << std::endl;

	expression = "4*(1+2)";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == 12) << std::endl;

	expression = "-19-4 * 6 + 19 - 5 + 7 * 2";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == -19 - 4 * 6 + 19 - 5 + 7 * 2) << std::endl;

	expression = "4*(1+2)-6";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == 6) << std::endl;

	expression = "4*(1+2)-6*(2*(1+1)-1)";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << (result2 == -6) << std::endl;

	expression = "-435 + 5*(435-564+324)/324 + 3454/3";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << ((-435 + 3454 / 3 - result2) < 0.001) << std::endl;

	expression = "-435 + 5*(435-564+324)/324 + 3454/3 - (324 - 345 * (3454 + (43*7))) - 9";
	tree = parseArithmeticExpressionToTree(expression);
	result2 = tree->evaluateNode(variables);
	std::cout << ((-435 + 5 * (435 - 564 + 324) / 324 + 3454 / 3 - (324 - 345 * (3454 + (43 * 7))) - 9 - result2) < 0.001) << std::endl;
}