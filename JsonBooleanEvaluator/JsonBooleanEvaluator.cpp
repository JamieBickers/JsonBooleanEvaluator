#include "stdafx.h"
#include "EvaluateArithmeticExpressions.h"
#include "BooleanTree.h"
#include "BooleanExpressionParser.h"
#include <iostream>
#include <map>
#include <string>
#include "json.h"
#include "VariableEvaluator.h"
#include "parseArrayMethod.h"

using json = nlohmann::json;

int* parseBooleanVectorToIntArray(std::vector<bool> bools);

int main()
{
	return 0;
}

void runTests()
{
	// print formatted booleans
	std::cout << std::boolalpha;

	runVariableParsingTests();
	runArrayMethodParsingTests();

	std::string end;
	std::cin >> end;
}

int* jsonBooleanEvaluator(char** jsonObjects, int numberOfJsonObjects, char* condition)
{
	std::string parsedCondition(condition);
	auto tree = parseBooleansExpressionToTree(parsedCondition);
	std::vector<std::string> parsedJsonObjects(jsonObjects, jsonObjects + numberOfJsonObjects);
	std::vector<bool> results;
	for (size_t i = 0; i < parsedJsonObjects.size(); i++)
	{
		json parsedJsonObject = json::parse(parsedJsonObjects[i]);
		results.push_back(tree->evaluateNode(parsedJsonObject));
	}
	return parseBooleanVectorToIntArray(results);
}

int* parseBooleanVectorToIntArray(std::vector<bool> bools)
{
	auto length = bools.size();
	int* asInts = (int*)malloc(sizeof(int) * (length + 1));
	asInts[0] = length;
	for (unsigned i = 0; i < length; i++)
	{
		if (bools[i]) {
			asInts[i + 1] = 1;
		}
		else {
			asInts[i + 1] = 0;
		}
	}
	return asInts;
}