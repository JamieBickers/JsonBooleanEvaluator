#include "stdafx.h"
#include "Utilities.h"
#include "json.h"
#include "VariableEvaluator.h"

#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
using json = nlohmann::json;

double evaluateArithmeticExpression(string expression, json jsonObject);
void runArithmeticEvaluationTests();
double handleBrackets(string expression, json jsonObject);
double handleDivision(string expression, json jsonObject);
double handleMultiplication(string expression, json jsonObject);
string getNextToken(string expression, int position);
string getTermsBetweenBracketsIncludingBrackets(string expression, int position);
double evaluateArithmeticExpressionWithPreComputedFirstTerm(string expression, double firstTerm, json jsonObject);
string getNextArithmeticTerm(string expression, int position);
int initialIndexOfPreviousToken(string expression, int position);
int terminalIndexOfNextToken(string expression, int position);
int initialIndexOfPreviousToken(string expression, int position);
int indexOfNextOrPreviousToken(string expression, int position, int rightOrLeft);
double handleMultiplicationOrDivision(string expression, double(*operation)(double a, double b), char operationCharacter, json jsonObject);
string getFullOperator(string expression, int startingPosition);

bool evaluateArithmeticCondition(string condition, json jsonObject)
{
	// remove whitespace
	condition.erase(std::remove(condition.begin(), condition.end(), ' '), condition.end());

	auto indexOfComparisionOperator = condition.find_first_of("<>!=", 0);
	auto comparisonOperator = getFullOperator(condition, indexOfComparisionOperator);
	auto left = condition.substr(0, indexOfComparisionOperator);
	auto right = condition.substr(indexOfComparisionOperator + comparisonOperator.size());

	auto leftValue = evaluateArithmeticExpression(left, jsonObject);
	auto rightValue = evaluateArithmeticExpression(right, jsonObject);

	if (comparisonOperator == "<") {
		return leftValue < rightValue;
	}
	else if (comparisonOperator == ">") {
		return leftValue > rightValue;
	}
	else if (comparisonOperator == "<=") {
		return leftValue <= rightValue;
	}
	else if (comparisonOperator == ">=") {
		return leftValue >= rightValue;
	}
	else if (comparisonOperator == "=" || comparisonOperator == "==" || comparisonOperator == "===") {
		return leftValue == rightValue;
	}
	else if (comparisonOperator == "!=") {
		return leftValue != rightValue;
	}
	else {
		throw "Invalid comparrison operation.";
	}
}

double evaluateArithmeticExpression(string expression, json jsonObject)
{
	// remove whitespace
	expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

	if (stringContainsCharacter(expression, '(', 0)) {
		return handleBrackets(expression, jsonObject);
	}
	else if (stringContainsCharacter(expression, '*', 0)) {
		return handleMultiplication(expression, jsonObject);
	}
	else if (stringContainsCharacter(expression, '/', 0)) {
		return handleDivision(expression, jsonObject);
	}
	else {
		string firstToken = getNextToken(expression, 0);
		if (firstToken == "-") {
			return evaluateArithmeticExpressionWithPreComputedFirstTerm(expression, 0, jsonObject);
		}
		else {
			double firstValue = isNumeric(firstToken) ? stod(firstToken) : evaluateVariable<int>(jsonObject, firstToken);
			return evaluateArithmeticExpressionWithPreComputedFirstTerm(expression.substr(firstToken.size()), firstValue, jsonObject);
		}
	}
	return 0;
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

double handleBrackets(string expression, json jsonObject)
{
	int openingBracketPosition = indexOfFirstChar(expression, '(', 0);
	int closingBracketPosition = indexOfClosingBracket(expression, openingBracketPosition);
	string valueInsideBrackets = to_string(evaluateArithmeticExpression(expression.substr(openingBracketPosition + 1,
		closingBracketPosition - openingBracketPosition - 1), jsonObject));

	expression = expression.replace(openingBracketPosition,
		closingBracketPosition - openingBracketPosition + 1,
		to_string(evaluateArithmeticExpression(expression.substr(openingBracketPosition + 1,
			closingBracketPosition - openingBracketPosition - 1), jsonObject)));
	return evaluateArithmeticExpression(expression, jsonObject);
}

double handleDivision(string expression, json jsonObject)
{
	return handleMultiplicationOrDivision(expression, [](double a, double b) {return a / b; }, '/', jsonObject);
}

double handleMultiplication(string expression, json jsonObject)
{
	return handleMultiplicationOrDivision(expression, [](double a, double b) {return a * b; }, '*', jsonObject);
}

double handleMultiplicationOrDivision(string expression, double(*operation)(double a, double b), char operationCharacter, json jsonObject)
{
	int indexOfOperation = indexOfFirstChar(expression, operationCharacter, 0);
	int indexOfPreviousToken = initialIndexOfPreviousToken(expression, indexOfOperation);
	string previousToken = expression.substr(indexOfPreviousToken, indexOfOperation - indexOfPreviousToken);
	int indexOfNextToken = terminalIndexOfNextToken(expression, indexOfOperation);
	string nextToken = expression.substr(indexOfOperation + 1, indexOfNextToken - indexOfOperation);
	double previousNumber = isNumeric(previousToken) ?
		stod(previousToken) : evaluateVariable<int>(jsonObject, previousToken);
	double nextNumber = isNumeric(nextToken) ?
		stod(nextToken) : evaluateVariable<int>(jsonObject, previousToken);
	double multiplicationOfTerms = operation(previousNumber, nextNumber);
	string result = to_string(multiplicationOfTerms);
	expression = expression.replace(indexOfPreviousToken, indexOfNextToken - indexOfPreviousToken + 1, result);
	return evaluateArithmeticExpression(expression, jsonObject);
}

double evaluateArithmeticExpressionWithPreComputedFirstTerm(string expression, double firstTerm, json jsonObject)
{
	if (expression == "") {
		return firstTerm;
	}
	string operations = "+-";
	string firstToken = getNextToken(expression, 0);
	if (!stringContainsCharacter(operations, firstToken[0])) {
		throw "Expression should start with a + or - operation.";
	}
	string nextExpression = getNextArithmeticTerm(expression, 1);

	if (firstToken == "+") {
		return firstTerm + evaluateArithmeticExpression(expression.substr(1), jsonObject);
	}
	else if (firstToken == "-") {
		double firstTwoTerms = firstTerm - evaluateArithmeticExpression(nextExpression, jsonObject);
		return evaluateArithmeticExpressionWithPreComputedFirstTerm(expression.substr(1 + nextExpression.size()), firstTwoTerms, jsonObject);
	}
}

string getNextArithmeticTerm(string expression, int position)
{
	string firstToken = getNextToken(expression, position);
	if (firstToken == "-") {
		return "-" + getNextArithmeticTerm(expression, position + 1);
	}
	else if (firstToken == "(") {
		return getTermsBetweenBracketsIncludingBrackets(expression, position);
	}
	else {
		return firstToken;
	}
}

string getNextToken(string expression, int position)
{
	string operationalTokens = "()+-*/=";
	string numbers = "0123456789.";

	if (position == expression.size()) {
		return "";
	}
	else if (stringContainsCharacter(operationalTokens, expression[position])) {
		return string(1, expression[position]);
	}
	else if (stringContainsCharacter(numbers, expression[position])) {
		string numericalToken;
		int currentPosition = position;
		while (currentPosition < expression.size()) {
			if (stringContainsCharacter(numbers, expression[currentPosition])) {
				numericalToken.push_back(expression[currentPosition]);
				currentPosition++;
			}
			else {
				break;
			}
		}
		return numericalToken;
	}
	else {
		string token;
		int currentPosition = position;
		while (currentPosition < expression.size()) {
			if (!stringContainsCharacter(operationalTokens, expression[currentPosition])) {
				token.push_back(expression[currentPosition]);
				currentPosition++;
			}
			else {
				break;
			}
		}
		return token;
	}
}

int terminalIndexOfNextToken(string expression, int position) {
	return indexOfNextOrPreviousToken(expression, position, 1);
}

int initialIndexOfPreviousToken(string expression, int position) {
	return indexOfNextOrPreviousToken(expression, position, -1);
}

// rightOrLeft = 1 for next token, -1 for previous
int indexOfNextOrPreviousToken(string expression, int position, int rightOrLeft) {
	const string operationalCharaceters = "()+-*/";
	int currentPosition = position + rightOrLeft;
	while (currentPosition >= 0 && currentPosition < expression.size())
	{
		if (!stringContainsCharacter(operationalCharaceters, expression[currentPosition], 0)) {
			currentPosition += rightOrLeft;
		}
		else if (expression[currentPosition] == '-') {
			if (currentPosition > 0 && stringContainsCharacter(operationalCharaceters, expression[currentPosition - 1], 0)) {
				currentPosition += rightOrLeft;
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}
	return currentPosition - rightOrLeft;
}

void runArithmeticEvaluationTests()
{
	cout << "Arithmetic evaluation tests." << endl;

	// getNextToken1
	string exp1 = "-3242+2-233*(343-6)/3";
	string result1 = getNextToken(exp1, 0);
	cout << ("-" == result1) << endl;

	// getNextToken2
	exp1 = "-3242+2-233*(343-6)/3";
	result1 = getNextToken(exp1, 1);
	cout << ("3242" == result1) << endl;

	// getNextToken3
	exp1 = "-3242+2-233*(343-6)/3";
	result1 = getNextToken(exp1, 12);
	cout << ("(" == result1) << endl;

	// getNextToken4
	exp1 = "-3242+2-233*(343-6)/3";
	result1 = getNextToken(exp1, 13);
	cout << ("343" == result1) << endl;

	// stringContainsCharacter1
	string exp2 = "342y8hu9ft8734yugfi";
	bool result2 = stringContainsCharacter(exp2, 'h', 3);
	cout << (true == result2) << endl;

	// stringContainsCharacter2
	exp2 = "342y8hu9ft8734yugfi";
	result2 = stringContainsCharacter(exp2, 'h', 6);
	cout << (false == result2) << endl;

	// stringContainsCharacter3
	exp2 = "342y8hu9ft8734yugfi";
	result2 = stringContainsCharacter(exp2, 'h', 5);
	cout << (true == result2) << endl;

	// getTermsBetweenBracketsIncludingBrackets1
	string exp5 = "()";
	string result5 = getTermsBetweenBracketsIncludingBrackets(exp5, 0);
	cout << ("()" == result5) << endl;

	// getTermsBetweenBracketsIncludingBrackets2
	exp5 = "(aergdsfhgdf)";
	result5 = getTermsBetweenBracketsIncludingBrackets(exp5, 0);
	cout << ("(aergdsfhgdf)" == result5) << endl;

	// getTermsBetweenBracketsIncludingBrackets3
	exp5 = "qwert(aergd(fsdfd(gfd))s((((dfds)f)f)s)fhgdf)";
	result5 = getTermsBetweenBracketsIncludingBrackets(exp5, 5);
	cout << ("(aergd(fsdfd(gfd))s((((dfds)f)f)s)fhgdf)" == result5) << endl;

	// isNumeric1
	string exp6 = "124546345426453";
	bool result6 = isNumeric(exp6);
	cout << (true == result6) << endl;

	// isNumeric2
	exp6 = "124546r345426453";
	result6 = isNumeric(exp6);
	cout << (false == result6) << endl;

	// isNumeric3
	exp6 = "3254.546245";
	result6 = isNumeric(exp6);
	cout << (true == result6) << endl;

	// getNextArithmeticTerm1
	string exp7 = "1+5";
	string result7 = getNextArithmeticTerm(exp7, 2);
	cout << ("5" == result7) << endl;

	// getNextArithmeticTerm1
	exp7 = "19+456-(45*5)";
	result7 = getNextArithmeticTerm(exp7, 7);
	cout << ("(45*5)" == result7) << endl;

	// initialIndexOfPreviousToken1
	string exp8 = "124-348*342+9";
	int result8 = initialIndexOfPreviousToken(exp8, 7);
	cout << (4 == result8) << endl;

	// initialIndexOfPreviousToken2
	exp8 = "14+-348*342+9";
	result8 = initialIndexOfPreviousToken(exp8, 7);
	cout << (3 == result8) << endl;

	auto jsonObject = map<string, double>();

	//evaluateArithmeticExpression1
	string exp = "2+4";
	double result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (6 == result) << endl;

	// evaluateArithmeticExpression2
	exp = "2";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (2 == result) << endl;

	// evaluateArithmeticExpression3
	exp = "234+90 + 21 + 16";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (361 == result) << endl;

	// evaluateArithmeticExpression4
	exp = "4-2";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (2 == result) << endl;

	// evaluateArithmeticExpression5
	exp = "-1";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (-1 == result) << endl;

	// evaluateArithmeticExpression6
	exp = "-19-22+18+5-10";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (-28 == result) << endl;

	// evaluateArithmeticExpression7
	exp = "-34534+3454-452-675-23+435-23454+2";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (-34534 + 3454 - 452 - 675 - 23 + 435 - 23454 + 2 == result) << endl;

	// evaluateArithmeticExpression8
	exp = "-19-4 * 6 + 19 - 5 + 7 * 2";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << ((-19 - 4 * 6 + 19 - 5 + 7 * 2 - result) < 0.001) << endl;

	// evaluateArithmeticExpression9
	exp = "4*(1+2)";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (result - 12 < 0.001) << endl;

	// evaluateArithmeticExpression10
	exp = "4*(1+2)-6*(2*(1+1)-1)";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << (result - -6 < 0.001) << endl;

	// evaluateArithmeticExpression11
	exp = "-435 + 5*(435-564+324)/324 + 3454/3";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << ((-435 + 3454 / 3 - result) < 0.001) << endl;

	// evaluateArithmeticExpression10
	exp = "-435 + 5*(435-564+324)/324 + 3454/3 - (324 - 345 * (3454 + (43*7))) - 9";
	result = evaluateArithmeticExpression(exp, jsonObject);
	cout << ((-435 + 5 * (435 - 564 + 324) / 324 + 3454 / 3 - (324 - 345 * (3454 + (43 * 7))) - 9 - result) < 0.001) << endl;

	// evaluateArithmeticCondition1
	string exp9 = "123 < 456";
	bool result9 = evaluateArithmeticCondition(exp9, jsonObject);
	cout << (true == result9) << endl;

	// evaluateArithmeticCondition1
	exp9 = "(5+6)*17-5 - 182 <= 0.01";
	result9 = evaluateArithmeticCondition(exp9, jsonObject);
	cout << (true == result9) << endl;

	// evaluateArithmeticCondition1
	exp9 = "16 > 17";
	result9 = evaluateArithmeticCondition(exp9, jsonObject);
	cout << (false == result9) << endl;
}