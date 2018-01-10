#include "stdafx.h"
#include "CppUnitTest.h"
#include "../JsonBooleanEvaluator/EvaluateArithmeticExpressions.h"
#include "../JsonBooleanEvaluator/BooleanTree.h"
#include "../JsonBooleanEvaluator/BooleanExpressionParser.h"
#include "../JsonBooleanEvaluator/json.h"
#include "../JsonBooleanEvaluator/VariableEvaluator.h"
#include "../JsonBooleanEvaluator/parseArrayMethod.h"
#include "../JsonBooleanEvaluator/JsonBooleanEvaluator.h"

#include <iostream>
#include <map>
#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using json = nlohmann::json;

namespace JsonBooleanEvaluatorTests
{
	TEST_CLASS(JsonBooleanEvaluatorOverallTests)
	{
	public:
		TEST_METHOD(JsonBooleanEvaluatorTest)
		{
			char* jsonObject1 = "{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}";
			char* jsonObject2 = "{\"a\": \"hello\", \"b\": 10, \"c\": {\"d\": -6, \"e\": [4, 5, 6], \"f\": false}}";
			char* jsonObject3 = "{\"a\": \"hello\", \"b\": 51, \"c\": {\"d\": 2, \"e\": [1, 2, 3], \"f\": true, \"g\": \"world\"}}";
			char** jsonObjects = (char**)malloc(3 * sizeof(char*));

			jsonObjects[0] = jsonObject1;
			jsonObjects[1] = jsonObject2;
			jsonObjects[2] = jsonObject3;

			char* condition = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"b\"] = 7) && [\"c\"][\"f\"]";
			auto results = jsonBooleanEvaluator(jsonObjects, 3, condition);

			Assert::AreEqual(3, results[0]);
			Assert::AreEqual(1, results[1]);
			Assert::AreEqual(0, results[2]);
			Assert::AreEqual(0, results[3]);

			free(jsonObjects);
			free(results);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest1)
		{
			json variables;
			auto expression = "5 < 10";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest2)
		{
			json variables;
			auto expression = "5 + (6*7) < 200 && 19 > 20";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest3)
		{
			json variables = json::parse("{\"x\": 5}");
			auto expression = "([\"x\"]-5)*100 < 10";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest4)
		{
			json variables = json::parse("{\"a\": 5, \"b\": 10, \"c\": 100 }");
			auto expression = "[\"a\"] < [\"b\"] || ([\"a\"] - [\"b\"] = 5 && [\"c\"] > [\"a\"]*[\"b\"]*20)";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest5)
		{
			json variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}");
			auto expression = "([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"b\"] = 7";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest6)
		{
			json variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}");
			auto expression = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"b\"] = 7) &&  [\"c\"][\"f\"]";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest7)
		{
			auto variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}");
			auto expression = "[\"c\"][\"e\"].any(x => x = 1)";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest8)
		{
			auto variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [-1, 2, 3], \"f\": true}}");
			auto expression = "[\"c\"][\"e\"].any(x => x = 1)";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest9)
		{
			auto variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 1, 1], \"f\": true}}");
			auto expression = "[\"c\"][\"e\"].all(x => x = 1)";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest10)
		{
			auto variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 1, -1], \"f\": true}}");
			auto expression = "[\"c\"][\"e\"].all(x => x = 1)";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest11)
		{
			auto variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 1, 1], \"f\": true}}");
			auto expression = "[\"c\"][\"e\"].count(x => x = 1) = 3";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest12)
		{
			auto variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, -1, 1], \"f\": true}}");
			auto expression = "[\"c\"][\"e\"].count(x => x = 1) = 2";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest13)
		{
			auto variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 1, 1], \"f\": true}}");
			auto expression = "[\"c\"][\"e\"].count(x => true) = 3";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(JsonBooleanEvaluatorGeneralTest14)
		{
			auto variables = json::parse("{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}");
			auto expression = "(([\"b\"] - 2 > 10 || [\"c\"][\"d\"] < 20) && [\"c\"][\"e\"][1] +  [\"c\"][\"e\"].count(x => true) = 5) &&  [\"c\"][\"f\"]";
			auto tree = parseBooleansExpressionToTree(expression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}
	};
}