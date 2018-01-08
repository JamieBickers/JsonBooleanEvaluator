#include "stdafx.h"
#include "ArithmeticCondition.h"

using json = nlohmann::json;

bool ArithmeticCondition::evaluateCondition(json variables, std::map<std::string, double> additonalVariables)
{
	auto leftValue = left->evaluateNode(variables, additonalVariables);
	auto rightValue = right->evaluateNode(variables, additonalVariables);

	if (comparison == "=" || comparison == "==" || comparison == "===") {
		return leftValue == rightValue;
	}
	else if (comparison == "<") {
		return leftValue < rightValue;
	}
	else if (comparison == ">") {
		return leftValue > rightValue;
	}
	else if (comparison == "<=") {
		return leftValue <= rightValue;
	}
	else if (comparison == ">=") {
		return leftValue >= rightValue;
	}
	else {
		throw "Invalid comparison.";
	}
}