#include "stdafx.h"
#include "CppUnitTest.h"
#include "../JsonBooleanEvaluator/json.h"
#include "../JsonBooleanEvaluator/EvaluateArithmeticExpressions.h"

using json = nlohmann::json;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JsonBooleanEvaluatorTests
{
	TEST_CLASS(EvaluateArithmeticExpressionsTests)
	{
	public:
		TEST_METHOD(EvaluateArithmeticExpressionsTest1)
		{
			auto expression = "5";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(5.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest2)
		{
			auto expression = "2+4";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(6.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest3)
		{
			auto expression = "234+90 + 21 + 16";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(361.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest4)
		{
			auto expression = "4-2";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(2.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest5)
		{
			auto expression = "-1";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(-1.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest6)
		{
			auto expression = "-19-22+18+5-10";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(-28.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest7)
		{
			auto expression = "-34534+3454-452-675-23+435-23454+2";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(-55247.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest8)
		{
			auto expression = "4*3";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(12.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest9)
		{
			auto expression = "(1+2)";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(3.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest10)
		{
			auto expression = "(1+2)";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(3.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest11)
		{
			auto expression = "4*(1+2)";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(12.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest12)
		{
			auto expression = "-19-4 * 6 + 19 - 5 + 7 * 2";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(-15.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest13)
		{
			auto expression = "4*(1+2)-6";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(6.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest14)
		{
			auto expression = "4*(1+2)-6*(2*(1+1)-1)";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::AreEqual(-6.0, result);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest15)
		{
			auto expression = "-435 + 5*(435-564+324)/324 + 3454/3";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(abs(77689.0 / 108.0 - result) < 0.001);
		}

		TEST_METHOD(EvaluateArithmeticExpressionsTest16)
		{
			auto expression = "-435 + 5*(435-564+324)/324 + 3454/3 - (324 - 345 * (3454 + (43*7))) - 9";
			auto tree = parseArithmeticExpressionToTree(expression);
			auto variables = json();
			auto result = tree->evaluateNode(variables);
			Assert::IsTrue(abs(139953025.0 / 108.0 - result) < 0.001);
		}
	};
}