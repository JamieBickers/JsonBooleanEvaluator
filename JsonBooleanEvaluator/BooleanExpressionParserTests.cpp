#include "stdafx.h"
#include "CppUnitTest.h"
#include "../JsonBooleanEvaluator/BooleanExpressionParser.h"
#include "../JsonBooleanEvaluator/json.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using json = nlohmann::json;

namespace JsonBooleanEvaluatorTests
{
	TEST_CLASS(BooleanExpressionParserTests)
	{
	public:
		TEST_METHOD(parseBooleansExpressionToTreeTest1)
		{
			json variables;
			std::string booleanexpression = "true";
			auto tree = parseBooleansExpressionToTree(booleanexpression);
			bool result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest2)
		{
			json variables;
			auto booleanExpression = "false";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest3)
		{
			json variables;
			auto booleanExpression = "(false)";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest4)
		{
			json variables;
			auto booleanExpression = "!true";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest5)
		{
			json variables;
			auto booleanExpression = "!(false)";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest6)
		{
			json variables;
			auto booleanExpression = "true || false";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest7)
		{
			json variables;
			auto booleanExpression = "true & false";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest8)
		{
			json variables;
			auto booleanExpression = "(true || false) && !(true)";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest9)
		{
			json variables;
			auto booleanExpression = "(true || false) && !true";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}

		TEST_METHOD(parseBooleansExpressionToTreeTest10)
		{
			json variables;
			auto booleanExpression = "((true) || (true || (false))) && (!(((true) || (false)) && (true)))";
			auto tree = parseBooleansExpressionToTree(booleanExpression);
			auto result = tree->evaluateNode(variables);
			Assert::IsFalse(result);
		}
	};
}