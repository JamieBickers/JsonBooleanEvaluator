#include "stdafx.h"
#include "ArrayMethod.h"
#include "Utilities.h"
#include "parseArrayMethod.h"

#include <iostream>

using namespace std;

string getLambdaVariable(string expression)
{
	auto endOfLambdaVariable = 0;
	while (expression[endOfLambdaVariable] != '=') {
		endOfLambdaVariable++;
	}
	return expression.substr(1, endOfLambdaVariable - 1);
}

void runArrayMethodParsingTests()
{
	cout << "Running array method parsing test." << endl;
	auto method = "[\"a\"][\"b\"].count(x => x > 0)";
	auto result1 = parseArrayMethod<double>(method);
	cout << (result1->lambdaVariable == "x") << endl;
	cout << (result1->method == "count") << endl;
	cout << (result1->variable == "[\"a\"][\"b\"]") << endl;

	method = "[\"a\"][\"b\"].all(xyz => xyz >= 0 || xyz = -7)";
	auto result2 = parseArrayMethod<bool>(method);
	cout << (result2->lambdaVariable == "xyz") << endl;
	cout << (result2->method == "all") << endl;
	cout << (result1->variable == "[\"a\"][\"b\"]") << endl;
}