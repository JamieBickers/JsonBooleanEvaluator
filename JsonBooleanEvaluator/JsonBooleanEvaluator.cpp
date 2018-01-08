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

void runGeneralTests();
void runJsonBooleanEvaluatorTests();

using namespace std;
using json = nlohmann::json;

int* parseBooleanVectorToIntArray(vector<bool> bools);
void runTests();

int main()
{
	runTests();
	_CrtDumpMemoryLeaks();
	return 0;
}

void runTests()
{
	// print formatted booleans
	cout << boolalpha;

	runArithmeticEvaluationTests();
	runBooleanTreeTests();
	runBooleanExpressionParserTests();
	runVariableParsingTests();
	runArrayMethodParsingTests();
	runGeneralTests();
	runJsonBooleanEvaluatorTests();

	string end;
	cin >> end;
}

int* jsonBooleanEvaluator(char** jsonObjects, int numberOfJsonObjects, char* condition)
{
	string parsedCondition(condition);
	auto tree = parseBooleansExpressionToTree(parsedCondition);
	vector<string> parsedJsonObjects(jsonObjects, jsonObjects + numberOfJsonObjects);
	vector<bool> results;
	for (size_t i = 0; i < parsedJsonObjects.size(); i++)
	{
		json parsedJsonObject = json::parse(parsedJsonObjects[i]);
		results.push_back(tree->evaluateNode(parsedJsonObject));
	}
	return parseBooleanVectorToIntArray(results);
}

int* parseBooleanVectorToIntArray(vector<bool> bools)
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

void runJsonBooleanEvaluatorTests()
{
	cout << "Running jsonBooleanEvaluator tests." << endl;
	char* jsonObject1 = "{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}";
	char* jsonObject2 = "{\"a\": \"hello\", \"b\": 10, \"c\": {\"d\": -6, \"e\": [4, 5, 6], \"f\": false}}";
	char* jsonObject3 = "{\"a\": \"hello\", \"b\": 51, \"c\": {\"d\": 2, \"e\": [1, 2, 3], \"f\": true, \"g\": \"world\"}}";
	char** jsonObjects = (char**)malloc(3 * sizeof(char*));
	jsonObjects[0] = jsonObject1;
	jsonObjects[1] = jsonObject2;
	jsonObjects[2] = jsonObject3;
	char* condition = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"b\"] = 7) && [\"c\"][\"f\"]";
	auto results = jsonBooleanEvaluator(jsonObjects, 3, condition);
	free(jsonObjects);

	cout << (results[0] == 3) << endl;
	cout << (results[1] == 1) << endl;
	cout << (results[2] == 0) << endl;
	cout << (results[2] == 0) << endl;
	free(results);
}

void runGeneralTests()
{
	cout << "Running general tests." << endl;

	json variables;
	auto expression = "5 < 10";
	auto tree = parseBooleansExpressionToTree(expression);
	auto result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	expression = "5 + (6*7) < 200 && 19 > 20";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == false) << endl;

	variables = json::parse("{\"x\": 5}");
	expression = "[\"x\"] < 10";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"x\": 5}");
	expression = "([\"x\"]-5)*100 < 10";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": 5, \"b\": 10, \"c\": 100 }");
	expression = "[\"a\"] < [\"b\"] || ([\"a\"] - [\"b\"] = 5 && [\"c\"] > [\"a\"]*[\"b\"]*20)";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}");
	expression = "([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"b\"] = 7";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	expression = "((false || true) && true) && true";
	tree = parseBooleansExpressionToTree(expression);
	bool res1 = tree->getLeftChild()->evaluateNode(variables);
	bool res2 = tree->getRightChild()->evaluateNode(variables);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}");
	expression = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"b\"] = 7) &&  [\"c\"][\"f\"]";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}");
	expression = "[\"c\"][\"e\"].any(x => x = 1)";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [-1, 2, 3], \"f\": true}}");
	expression = "[\"c\"][\"e\"].any(x => x = 1)";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == false) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 1, 1], \"f\": true}}");
	expression = "[\"c\"][\"e\"].all(x => x = 1)";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 1, -1], \"f\": true}}");
	expression = "[\"c\"][\"e\"].all(x => x = 1)";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == false) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 1, 1], \"f\": true}}");
	expression = "[\"c\"][\"e\"].count(x => x = 1) = 3";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, -1, 1], \"f\": true}}");
	expression = "[\"c\"][\"e\"].count(x => x = 1) = 2";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, -1, 1], \"f\": true}}");
	expression = "[\"c\"][\"e\"].count(x => true) = 3";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;

	variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}");
	expression = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"c\"][\"e\"].count(x => true) = 5) &&  [\"c\"][\"f\"]";
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;
}