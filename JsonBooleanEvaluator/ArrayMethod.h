#pragma once

class BooleanTree;
#include "ArithmeticCondition.h"
#include "json.h"

#include <string>

bool any(std::string arrayVariable, nlohmann::json variables, std::shared_ptr<BooleanTree> condition, std::string lambdaVariable);
bool all(std::string arrayVariable, nlohmann::json variables, std::shared_ptr<BooleanTree> condition, std::string lambdaVariable);
int count(std::string arrayVariable, nlohmann::json variables, std::shared_ptr<BooleanTree> condition, std::string lambdaVariable);

template <typename T>
class ArrayMethod
{
public:
	std::string variable;
	std::string lambdaVariable;
	std::string method;
	std::shared_ptr<BooleanTree> condition;

	T evaluateMethod(nlohmann::json variables)
	{
		if (std::is_same<T, bool>::value) {
			return evaluateBooleanMethod(variables);
		}
		else if (std::is_same<T, double>::value) {
			return evaluateNumericalMethod(variables);
		}
		else {
			throw "Invalid array method return type.";
		}
	}

private:
	bool evaluateBooleanMethod(nlohmann::json variables)
	{
		if (method == "any") {
			return any(variable, variables, condition, lambdaVariable);
		}
		else if (method == "all") {
			return all(variable, variables, condition, lambdaVariable);
		}
		else {
			throw "Invalid method.";
		}
	}

	double evaluateNumericalMethod(nlohmann::json variables)
	{
		if (method == "count") {

		}
		else {
			throw "Invalid method.";
		}
	}
};