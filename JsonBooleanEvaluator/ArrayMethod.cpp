#include "stdafx.h"
#include "BooleanTree.h"
#include "ArrayMethod.h"
#include "json.h"
#include "ArithmeticCondition.h"
#include "VariableEvaluator.h"

#include <string>

using json = nlohmann::json;

bool any(std::string arrayVariable, json variables, std::shared_ptr<BooleanTree> condition, std::string lambdaVariable)
{
	std::map<std::string, double> additionalVariables;
	auto evaluatedArray = evaluateVariable<std::vector<double>>(variables, arrayVariable);
	for (size_t i = 0; i < evaluatedArray.size(); i++)
	{
		additionalVariables[lambdaVariable] = evaluatedArray[i];
		auto satisfiesCondition = condition->evaluateNode(variables, additionalVariables);
		if (satisfiesCondition) {
			return true;
		}
	}
	return false;
}

bool all(std::string arrayVariable, json variables, std::shared_ptr<BooleanTree> condition, std::string lambdaVariable)
{
	std::map<std::string, double> additionalVariables;
	auto evaluatedArray = evaluateVariable<std::vector<double>>(variables, arrayVariable);
	for (size_t i = 0; i < evaluatedArray.size(); i++)
	{
		additionalVariables[lambdaVariable] = evaluatedArray[i];
		auto satisfiesCondition = condition->evaluateNode(variables, additionalVariables);
		if (!satisfiesCondition) {
			return false;
		}
	}
	return true;
}

int count(std::string arrayVariable, json variables, std::shared_ptr<BooleanTree> condition, std::string lambdaVariable)
{
	std::map<std::string, double> additionalVariables;
	auto evaluatedArray = evaluateVariable<std::vector<double>>(variables, arrayVariable);
	int countSoFar = 0;
	for (size_t i = 0; i < evaluatedArray.size(); i++)
	{
		additionalVariables[lambdaVariable] = evaluatedArray[i];
		auto satisfiesCondition = condition->evaluateNode(variables, additionalVariables);
		if (satisfiesCondition) {
			countSoFar++;
		}
	}
	return countSoFar;
}