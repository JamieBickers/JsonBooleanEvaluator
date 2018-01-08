#include "stdafx.h"
#include "ArrayMethod.h"
#include "Utilities.h"
#include "parseArrayMethod.h"

#include <iostream>

std::string getLambdaVariable(std::string expression)
{
	auto endOfLambdaVariable = 0;
	while (expression[endOfLambdaVariable] != '=') {
		endOfLambdaVariable++;
	}
	return expression.substr(1, endOfLambdaVariable - 1);
}

void runArrayMethodParsingTests()
{
	std::cout << "Running array method parsing test." << std::endl;
	auto method = "[\"a\"][\"b\"].count(x => x > 0)";
	auto result1 = parseArrayMethod<double>(method);
	std::cout << (result1->lambdaVariable == "x") << std::endl;
	std::cout << (result1->method == "count") << std::endl;
	std::cout << (result1->variable == "[\"a\"][\"b\"]") << std::endl;

	method = "[\"a\"][\"b\"].all(xyz => xyz >= 0 || xyz = -7)";
	auto result2 = parseArrayMethod<bool>(method);
	std::cout << (result2->lambdaVariable == "xyz") << std::endl;
	std::cout << (result2->method == "all") << std::endl;
	std::cout << (result1->variable == "[\"a\"][\"b\"]") << std::endl;
}