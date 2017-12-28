#include "stdafx.h"
#include "EvaluateArithmeticExpressions.h"
#include "BooleanTree.h"
#include "BooleanExpressionParser.h"
#include <iostream>
#include <map>
#include <string>
#include "json.h"
#include "VariableEvaluator.h"

void runGeneralTests();
void runJsonBooleanEvaluatorTests();

using namespace std;
using json = nlohmann::json;

int main()
{
	// print formatted booleans
	cout << boolalpha;

	runArithmeticEvaluationTests();
	runBooleanTreeTests();
	runBooleanExpressionParserTests();
	runVariableParsingTests();
	runGeneralTests();
	runJsonBooleanEvaluatorTests();

	string end;
	cin >> end;

	return 0;
}

vector<bool> jsonBooleanEvaluator(vector<string> jsonObjects, string condition)
{
	auto tree = parseBooleansExpressionToTree(condition);
	vector<bool> results;
	for (size_t i = 0; i < jsonObjects.size(); i++)
	{
		json parsedJsonObject = json::parse(jsonObjects[i]);
		results.push_back(tree->evaluateNode(parsedJsonObject));
	}
	return results;
}

void runJsonBooleanEvaluatorTests()
{
	cout << "Running jsonBooleanEvaluator tests." << endl;
	auto jsonObject1 = "{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}";
	auto jsonObject2 = "{\"a\": \"hello\", \"b\": 10, \"c\": {\"d\": -6, \"e\": [4, 5, 6], \"f\": false}}";
	auto jsonObject3 = "{\"a\": \"hello\", \"b\": 51, \"c\": {\"d\": 2, \"e\": [1, 2, 3], \"f\": true, \"g\": \"world\"}}";
	vector<string> jsonObjects;
	jsonObjects.push_back(jsonObject1);
	jsonObjects.push_back(jsonObject2);
	jsonObjects.push_back(jsonObject3);
	auto condition = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"b\"] = 7) && [\"c\"][\"f\"]";
	auto results = jsonBooleanEvaluator(jsonObjects, condition);

	cout << (results[0] == true) << endl;
	cout << (results[1] == false) << endl;
	cout << (results[2] == false) << endl;
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
	expression = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"b\"] = 7) && true"; //[\"c\"][\"f\"]
	tree = parseBooleansExpressionToTree(expression);
	result = tree->evaluateNode(variables);
	cout << (result == true) << endl;
}