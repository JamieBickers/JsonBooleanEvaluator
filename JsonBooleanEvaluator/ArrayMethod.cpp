#include "stdafx.h"
#include "ArrayMethod.h"
#include "json.h"
#include "ArithmeticCondition.h"
#include "VariableEvaluator.h"

#include <string>

using namespace std;
using json = nlohmann::json;

bool any(string arrayVariable, json variables, shared_ptr<ArithmeticCondition> condition, string lambdaVariable)
{
	map<string, double> additionalVariables;
	auto evaluatedArray = evaluateVariable<vector<double>>(variables, arrayVariable);
	for (size_t i = 0; i < evaluatedArray.size(); i++)
	{
		additionalVariables[lambdaVariable] = evaluatedArray[i];
		auto satisfiesCondition = condition->evaluateCondition(variables, additionalVariables);
		if (satisfiesCondition) {
			return true;
		}
	}
	return false;
}

bool all(string arrayVariable, json variables, shared_ptr<ArithmeticCondition> condition, string lambdaVariable)
{
	map<string, double> additionalVariables;
	auto evaluatedArray = evaluateVariable<vector<double>>(variables, arrayVariable);
	for (size_t i = 0; i < evaluatedArray.size(); i++)
	{
		additionalVariables[lambdaVariable] = evaluatedArray[i];
		auto satisfiesCondition = condition->evaluateCondition(variables, additionalVariables);
		if (!satisfiesCondition) {
			return false;
		}
	}
	return true;
}

int count(string arrayVariable, json variables, shared_ptr<ArithmeticCondition> condition, string lambdaVariable)
{
	map<string, double> additionalVariables;
	auto evaluatedArray = evaluateVariable<vector<double>>(variables, arrayVariable);
	int countSoFar = 0;
	for (size_t i = 0; i < evaluatedArray.size(); i++)
	{
		additionalVariables[lambdaVariable] = evaluatedArray[i];
		auto satisfiesCondition = condition->evaluateCondition(variables, additionalVariables);
		if (satisfiesCondition) {
			countSoFar++;
		}
	}
	return countSoFar;
}