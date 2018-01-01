#pragma once

#include "stdafx.h"
#include "ArrayMethod.h"
#include "Utilities.h"
#include "parseArrayMethod.h"
#include "BooleanExpressionParser.h"

#include <memory>
#include <string>

void runArrayMethodParsingTests();

template <typename T>
std::shared_ptr<ArrayMethod<T>> parseArrayMethod(std::string method)
{
	// remove whitespace
	method.erase(std::remove(method.begin(), method.end(), ' '), method.end());

	auto indexOfStartOfMethod = method.find_first_of('.');
	auto arrayVariable = method.substr(0, indexOfStartOfMethod);
	auto indexOfTheOpeningeBracket = indexOfNextOpeningBracket(method, indexOfStartOfMethod);
	auto lambdaVariable = getLambdaVariable(method.substr(indexOfTheOpeningeBracket));
	auto condition = method.substr(indexOfTheOpeningeBracket + lambdaVariable.size() + 3);
	condition = condition.substr(0, condition.size() - 1);
	auto methodName = method.substr(indexOfStartOfMethod + 1, indexOfTheOpeningeBracket - indexOfStartOfMethod - 1);
	auto arithmeticCondition = parseBooleansExpressionToTree(condition);

	auto arrayMethod = shared_ptr<ArrayMethod<T>>(new ArrayMethod<T>);
	arrayMethod->variable = arrayVariable;
	arrayMethod->lambdaVariable = lambdaVariable;
	arrayMethod->method = methodName;
	arrayMethod->condition = arithmeticCondition;
	return arrayMethod;
}