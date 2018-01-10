#include "stdafx.h"
#include "CppUnitTest.h"
#include "../JsonBooleanEvaluator/json.h"
#include "../JsonBooleanEvaluator/VariableEvaluator.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using json = nlohmann::json;

namespace JsonBooleanEvaluatorTests
{
	TEST_CLASS(VariableEvaluatorTests)
	{
	public:
		TEST_METHOD(VariableEvaluatorTest1)
		{
			auto myJson = "{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}";
			auto parsedJson = json::parse(myJson);
			auto variable = "[\"c\"][\"d\"]";
			auto result = evaluateVariable<int>(parsedJson, variable);
			Assert::AreEqual(10, result);
		}

		TEST_METHOD(VariableEvaluatorTest2)
		{
			auto myJson = "{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}";
			auto parsedJson = json::parse(myJson);
			auto variable = "[\"c\"][\"e\"][1]";
			auto result = evaluateVariable<int>(parsedJson, variable);
			Assert::AreEqual(2, result);
		}

		TEST_METHOD(VariableEvaluatorTest3)
		{
			auto myJson = "{\"a\": \"hello\", \"b\": 5, \"c\": {\"d\": 10, \"e\": [1, 2, 3], \"f\": true}}";
			auto parsedJson = json::parse(myJson);
			auto variable = "[\"c\"][\"f\"]";
			auto result = evaluateVariable<bool>(parsedJson, variable);
			Assert::IsTrue(result);
		}
	};
}