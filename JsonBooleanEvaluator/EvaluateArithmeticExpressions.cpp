#include "stdafx.h"
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>

using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::boolalpha;
using std::vector;
using std::stod;
using std::to_string;

double evaluateArithmeticExpression(string expression);
void runTests();
double handleBrackets(std::string &expression);
double handleDivision(string expression);
double handleMultiplication(string expression);
string getNextToken(string expression, int position);
bool stringContainsCharacter(string expression, char character, int startingPosition = 0);
string getTermsBetweenBracketsIncludingBrackets(string expression, int position);
bool isNumeric(string possibleNumber);
double evaluateArithmeticExpressionWithPreComputedFirstTerm(string expression, double firstTerm);
string getNextArithmeticTerm(string expression, int position);
int indexOfFirstChar(string expression, char character, int position);
int initialIndexOfPreviousToken(string expression, int position);
int indexOfClosingBracket(string expression, int position);
int terminalIndexOfNextToken(string expression, int position);
int initialIndexOfPreviousToken(string expression, int position);
int indexOfNextOrPreviousToken(string expression, int position, int rightOrLeft);
double handleMultiplicationOrDivision(string expression, double(*operation)(double a, double b), char operationCharacter);

double evaluateArithmeticExpression(string expression)
{
	// remove whitespace
	expression.erase(std::remove(expression.begin(), expression.end(), ' '), expression.end());

	if (stringContainsCharacter(expression, '(', 0)) {
		return handleBrackets(expression);
	}
	else if (stringContainsCharacter(expression, '*', 0)) {
		return handleMultiplication(expression);
	}
	else if (stringContainsCharacter(expression, '/', 0)) {
		return handleDivision(expression);
	}
	else {
		string firstToken = getNextToken(expression, 0);
		if (firstToken == "-") {
			return evaluateArithmeticExpressionWithPreComputedFirstTerm(expression, 0);
		}
		else {
			return evaluateArithmeticExpressionWithPreComputedFirstTerm(expression.substr(firstToken.size()), stod(firstToken));
		}
	}
	return 0;
}

double handleBrackets(std::string &expression)
{
	int openingBracketPosition = indexOfFirstChar(expression, '(', 0);
	int closingBracketPosition = indexOfClosingBracket(expression, openingBracketPosition);
	string valueInsideBrackets = to_string(evaluateArithmeticExpression(expression.substr(openingBracketPosition + 1,
		closingBracketPosition - openingBracketPosition - 1)));

	expression = expression.replace(openingBracketPosition,
		closingBracketPosition - openingBracketPosition + 1,
		to_string(evaluateArithmeticExpression(expression.substr(openingBracketPosition + 1,
			closingBracketPosition - openingBracketPosition - 1))));
	return evaluateArithmeticExpression(expression);
}

double handleDivision(string expression)
{
	return handleMultiplicationOrDivision(expression, [](double a, double b) {return a / b; }, '/');
}

double handleMultiplication(string expression)
{
	return handleMultiplicationOrDivision(expression, [](double a, double b) {return a * b; }, '*');
}

double handleMultiplicationOrDivision(string expression, double(*operation)(double a, double b), char operationCharacter)
{
	int indexOfOperation = indexOfFirstChar(expression, operationCharacter, 0);
	int indexOfPreviousToken = initialIndexOfPreviousToken(expression, indexOfOperation);
	string previousToken = expression.substr(indexOfPreviousToken, indexOfOperation - indexOfPreviousToken);
	int indexOfNextToken = terminalIndexOfNextToken(expression, indexOfOperation);
	string nextToken = expression.substr(indexOfOperation + 1, indexOfNextToken - indexOfOperation);
	double multiplicationOfTerms = operation(stod(previousToken), stod(nextToken));
	string result = to_string(multiplicationOfTerms);
	expression = expression.replace(indexOfPreviousToken, indexOfNextToken - indexOfPreviousToken, result);
	return evaluateArithmeticExpression(expression);
}

double evaluateArithmeticExpressionWithPreComputedFirstTerm(string expression, double firstTerm)
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
		return firstTerm + evaluateArithmeticExpression(expression.substr(1));
	}
	else if (firstToken == "-") {
		double firstTwoTerms = firstTerm - evaluateArithmeticExpression(nextExpression);
		return evaluateArithmeticExpressionWithPreComputedFirstTerm(expression.substr(1 + nextExpression.size()), firstTwoTerms);
	}
}

string getNextArithmeticTerm(string expression, int position)
{
	string firstToken = getNextToken(expression, position);
	if (firstToken == "-") {
		return "-" + getNextArithmeticTerm(expression, position + 1);
	}
	else if (isNumeric(firstToken)) {
		return firstToken;
	}
	else if (firstToken == "(") {
		return getTermsBetweenBracketsIncludingBrackets(expression, position);
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
}

int terminalIndexOfNextToken(string expression, int position) {
	return indexOfNextOrPreviousToken(expression, position, 1);
}

int initialIndexOfPreviousToken(string expression, int position) {
	return indexOfNextOrPreviousToken(expression, position, -1);
}

// rightOrLeft = 1 for next token, -1 for previous
int indexOfNextOrPreviousToken(string expression, int position, int rightOrLeft) {
	const string numerals = "0123456789.";
	int currentPosition = position + rightOrLeft;
	while (currentPosition >= 0)
	{
		if (stringContainsCharacter(numerals, expression[currentPosition], 0)) {
			currentPosition += rightOrLeft;
		}
		else if (expression[currentPosition] == '-') {
			if (currentPosition > 0 && stringContainsCharacter("+-*/", expression[currentPosition - 1], 0)) {
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


int indexOfClosingBracket(string expression, int position) {
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

string getTermsBetweenBracketsIncludingBrackets(string expression, int position)
{
	if (expression[position] != '(') {
		throw "Not given brackets.";
	}
	int closingIndex = indexOfClosingBracket(expression, position);
	return expression.substr(position, closingIndex + 1);
}

bool isNumeric(string possibleNumber) {
	if (possibleNumber == "") {
		return false;
	}
	else {
		for (size_t i = 0; i < possibleNumber.size(); i++) {
			if (!isdigit(possibleNumber[i]) && !(possibleNumber[i] == '.')) {
				return false;
			}
		}
		return true;
	}
}

bool stringContainsCharacter(string expression, char character, int startingPosition)
{
	return expression.find(character, startingPosition) != string::npos;
}

int indexOfFirstChar(string expression, char character, int position)
{
	for (size_t i = 0; i < expression.size(); i++)
	{
		if (expression[i] == character) {
			return i;
		}
	}
	throw "Character is not in string.";
}

void runTests()
{
	// tests below

	// print formatted booleans
	cout << boolalpha;

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


	//evaluateArithmeticExpression1
	string exp = "2+4";
	double result = evaluateArithmeticExpression(exp);
	cout << (6 == result) << endl;

	// evaluateArithmeticExpression2
	exp = "2";
	result = evaluateArithmeticExpression(exp);
	cout << (2 == result) << endl;

	// evaluateArithmeticExpression3
	exp = "234+90 + 21 + 16";
	result = evaluateArithmeticExpression(exp);
	cout << (361 == result) << endl;

	// evaluateArithmeticExpression4
	exp = "4-2";
	result = evaluateArithmeticExpression(exp);
	cout << (2 == result) << endl;

	// evaluateArithmeticExpression5
	exp = "-1";
	result = evaluateArithmeticExpression(exp);
	cout << (-1 == result) << endl;

	// evaluateArithmeticExpression6
	exp = "-19-22+18+5-10";
	result = evaluateArithmeticExpression(exp);
	cout << (-28 == result) << endl;

	// evaluateArithmeticExpression7
	exp = "-34534+3454-452-675-23+435-23454+2";
	result = evaluateArithmeticExpression(exp);
	cout << (-34534 + 3454 - 452 - 675 - 23 + 435 - 23454 + 2 == result) << endl;

	// evaluateArithmeticExpression8
	exp = "-19-4 * 6 + 19 - 5 + 7 * 2";
	result = evaluateArithmeticExpression(exp);
	cout << ((-19 - 4 * 6 + 19 - 5 + 7 * 2 - result) < 0.001) << endl;

	// evaluateArithmeticExpression9
	exp = "4*(1+2)";
	result = evaluateArithmeticExpression(exp);
	cout << (result - 12 < 0.001) << endl;

	// evaluateArithmeticExpression10
	exp = "4*(1+2)-6*(2*(1+1)-1)";
	result = evaluateArithmeticExpression(exp);
	cout << (result - -6 < 0.001) << endl;

	// evaluateArithmeticExpression11
	exp = "-435 + 5*(435-564+324)/324 + 3454/3";
	result = evaluateArithmeticExpression(exp);
	cout << ((-435 + 3454 / 3 - result) < 0.001) << endl;

	// evaluateArithmeticExpression10
	exp = "-435 + 5*(435-564+324)/324 + 3454/3 - (324 - 345 * (3454 + (43*7))) - 9";
	result = evaluateArithmeticExpression(exp);
	cout << ((-435 + 5 * (435 - 564 + 324) / 324 + 3454 / 3 - (324 - 345 * (3454 + (43 * 7))) - 9 - result) < 0.001) << endl;

	// pause before ending
	string end;
	cin >> end;
}

//string getNextVariable(string expression, int startingPosition)
//{
//	string squareBrackets = "[]";
//	string validVariableNameCharacters = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
//	if (!stringContainsCharacter(validVariableNameCharacters, expression[startingPosition])) {
//		throw "Not given a variable";
//	}
//	string variable;
//	int currentPosition = startingPosition;
//	while (currentPosition < expression.size()) {
//		if (stringContainsCharacter(validVariableNameCharacters, expression[currentPosition])) {
//			variable.push_back(expression[currentPosition]);
//			currentPosition++;
//		}
//		else {
//			break;
//		}
//	}
//	if (expression[currentPosition] == '[') {
//		variable += getTermsInSquareBrackets(expression, currentPosition);
//	}
//	return variable;
//}

//string getTermsInSquareBrackets(string expression, int position)
//{
//	if (!expression[position] == '[') {
//		throw "Not given square brackets.";
//	}
//	int currentPosition = position;
//	string bracketedTerms;
//	while (currentPosition < expression.size()) {
//		bracketedTerms.push_back(expression[currentPosition]);
//		if (expression[currentPosition] == ']') {
//			break;
//		}
//		currentPosition++;
//	}
//	if (currentPosition + 1 < expression.size() && expression[currentPosition + 1] == '[') {
//		bracketedTerms += getTermsInSquareBrackets(expression, currentPosition + 1);
//	}
//	return bracketedTerms;
//}