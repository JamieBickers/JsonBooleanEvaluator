#include "stdafx.h"
#include "ArithmeticCondition.h"

using json = nlohmann::json;

bool ArithmeticCondition::evaluateCondition(json variables)
{
	auto leftValue = left->evaluateNode(variables);
	auto rightValue = right->evaluateNode(variables);

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